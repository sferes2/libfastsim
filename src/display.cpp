/*
** sdl.cc
** Login : <mouret@mexico>
** Started on  Sat Jan 12 20:40:12 2008 Jeanbaptiste MOURET
** $Id$
** 
** Copyright (C) 2008 Jeanbaptiste MOURET
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/
#include "display.hpp"
#include <bitset>

#ifdef USE_SDL

namespace fastsim
{

  void Display :: _events()
  {
    SDL_Event event;
    while (SDL_PollEvent(&event))
      {
	switch (event.type) 
	  {
	  case SDL_QUIT:
	    _quit();
	  case SDL_KEYDOWN:
	    if (event.key.keysym.sym == SDLK_ESCAPE)
	      _quit();
	    break;
	  }
      }
  }

  void Display :: _blit_map()
  {
    for (unsigned i = 0; i < _map->get_pixel_w(); ++i)
      for (unsigned j = 0; j < _map->get_pixel_h(); ++j)
	if (_map->get_pixel(i, j) == Map::obstacle)
	  _put_pixel(_map_bmp, i, j, 0, 0, 0);
	else
	  _put_pixel(_map_bmp, i, j, 255, 255, 255);
    SDL_BlitSurface(_map_bmp, 0, _screen, 0);
    SDL_UpdateRect(_screen, 0, 0, _w, _h);
  }

  Display :: Display(std::shared_ptr<Map> m, std::shared_ptr<Robot> r) : 
    _map(m), _robot(r)
  {
    _w = _map->get_pixel_w();
    _h = _map->get_pixel_h();
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
      throw Exception(SDL_GetError());
    _screen = SDL_SetVideoMode(_w, _h, 
			       32, SDL_SWSURFACE);
    if (!_screen)
      throw Exception(SDL_GetError());

    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    _map_bmp = SDL_CreateRGBSurface(SDL_SWSURFACE, _w, _h, 32, 
				    rmask, gmask, bmask, amask);
    _blit_map();
    _bb_to_sdl(_robot->get_bb(), &_prev_bb);
  }
  
  void Display :: _line(SDL_Surface* surf,
			int x0, int y0, int x1, int y1, 
			Uint32 color)
  {
    x0 = std::max(x0, 0); x0 = std::min(x0, surf->w - 1);
    x1 = std::max(x1, 0); x1 = std::min(x1, surf->w - 1);
    y0 = std::max(y0, 0); y0 = std::min(y0, surf->h - 1);
    y1 = std::max(y1, 0); y1 = std::min(y1, surf->h - 1);
    int dy = y1 - y0;
    int dx = x1 - x0;
    int stepx, stepy;

    if (dy < 0) { dy = -dy;  stepy = -1; } else { stepy = 1; }
    if (dx < 0) { dx = -dx;  stepx = -1; } else { stepx = 1; }
    dy <<= 1; // dy is now 2*dy
    dx <<= 1; // dx is now 2*dx

    _put_pixel(surf, color, x0, y0);
    if (dx > dy) 
      {
	int fraction = dy - (dx >> 1); // same as 2*dy - dx
	while (x0 != x1)
	  {
	    if (fraction >= 0)
	      {
		y0 += stepy;
		fraction -= dx; // -= 2*dx
	      }
	    x0 += stepx;
	    fraction += dy; //  -= 2*dy
	    _put_pixel(surf, color, x0, y0);
	  }
      } 
    else 
      {
	int fraction = dx - (dy >> 1);
	while (y0 != y1)
	  {
	    if (fraction >= 0)
	      {
		x0 += stepx;
		fraction -= dy;
	      }
	    y0 += stepy;
	    fraction += dx;
	    _put_pixel(surf, color, x0, y0);
	  }
      }
  }

  void Display :: _try_pixel(bool& res,
			     SDL_Surface* surf,
			     Uint32 color, int x, int y)
  {

    if (x >= 0 && y >= 0 
	&& x < (int)_map->get_pixel_w() 
	&& y < (int)_map->get_pixel_h() 
	&& _map->get_pixel(x, y) == Map::free)
      {
	_put_pixel(surf, color, x, y);
	res = false;
      }
    else
      res = true;
  }

  void Display :: _circle_points(SDL_Surface* surf,
				 int cx, int cy, int x, int y, Uint32 color)
  {

    if (x == 0) {
      _put_pixel(surf, color, cx, cy + y);
      _put_pixel(surf, color, cx, cy - y);
      _put_pixel(surf, color, cx + y, cy);
      _put_pixel(surf, color, cx - y, cy);
    }
    else if (x == y) {
      _put_pixel(surf, color, cx + x, cy + y);
      _put_pixel(surf, color, cx - x, cy + y);
      _put_pixel(surf, color, cx + x, cy - y);
      _put_pixel(surf, color, cx - x, cy - y);
    } else if (x < y) {
      _put_pixel(surf, color, cx + x, cy + y);
      _put_pixel(surf, color, cx - x, cy + y);
      _put_pixel(surf, color, cx + x, cy - y);
      _put_pixel(surf, color, cx - x, cy - y);
      _put_pixel(surf, color, cx + y, cy + x);
      _put_pixel(surf, color, cx - y, cy + x);
      _put_pixel(surf, color, cx + y, cy - x);
      _put_pixel(surf, color, cx - y, cy - x);
    }
  }

  void Display :: _circle(SDL_Surface *surf,
			  int x_center, int y_center, int radius,
			  Uint32 color)
  {    
    int x = 0;
    int y = radius;
    int p = (5 - radius * 4) / 4;
    
    _circle_points(surf, x_center, y_center, x, y, color);
    while (x < y) 
      {
	x++;
	if (p < 0) 
	  p += 2 * x + 1;
	else
	  {
	    y--;
	    p += 2 * (x - y) + 1;
	  }
	_circle_points(surf, x_center, y_center, x, y, color);
      }
  }
  /// see http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
  // transform setPixel(x0 + x, y0 + y); setPixel(x0 - x, y0 + y);
  // to  _line(x0 - x,, y0+y, x0 + x, y0 + y)
  void Display :: _disc(SDL_Surface *surf,
			int x_center, int y_center, int radius,
			Uint32 color)
  {
    int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;
    int x0 = x_center;
    int y0 = y_center;

    _line(surf, x0, y0 - radius, x0, y0 + radius, color);
    _line(surf, x0 - radius, y0, x0 + radius, y0, color);    
    while(x < y)
      {
	// ddF_x == 2 * x + 1;
	// ddF_y == -2 * y;
	// f == x*x + y*y - radius*radius + 2*x - y + 1;
	if(f >= 0) 
	  {
	    y--;
	    ddF_y += 2;
	    f += ddF_y;
	  }
	x++;
	ddF_x += 2;
	f += ddF_x;   
	_line(surf, x0 - x, y0 + y, x0 + x, y0 + y, color);
	_line(surf, x0 - x, y0 - y, x0 + x, y0 - y, color);
	_line(surf, x0 - y, y0 + x, x0 + y, y0 + x, color);
	_line(surf, x0 - y, y0 - x, x0 + y, y0 - x, color);
      }
  }



  void Display :: _disp_bb()
  {
    unsigned x = _map->real_to_pixel(_robot->get_bb().x);
    unsigned y = _map->real_to_pixel(_robot->get_bb().y);
    unsigned w = _map->real_to_pixel(_robot->get_bb().w);
    unsigned h = _map->real_to_pixel(_robot->get_bb().h);
    
    assert(x >= 0);
    assert(y >= 0);
    assert(x + w < (unsigned)_screen->w);
    assert(y + h < (unsigned)_screen->h);
    _line(_screen, x, y, x + w, y, 0);
    _line(_screen, x + w, y, x + w, y + h, 0);
    _line(_screen, x + w, y + h, x, y + h, 0);
    _line(_screen, x, y + h, x, y, 0);    
  }

  void Display :: _disp_goals()
  {
    for (size_t i = 0; i < _map->get_goals().size(); ++i)
      {
	const Goal& goal = _map->get_goals()[i];
	unsigned x = _map->real_to_pixel(goal.get_x());
	unsigned y = _map->real_to_pixel(goal.get_y());
	unsigned diam = _map->real_to_pixel(goal.get_diam());
	Uint8 r = 0, g = 0, b = 0;
	switch (goal.get_color())
	  {
	  case 0:
	    r = 255;
	    break;
	  case 1:
	    g = 255;
	    break;
	  case 2:
	    b = 255;
	    break;
	  default:
	    assert(0);
	  }
	_circle(_screen, x, y, diam, r, g, b);
      }    
  }

  void Display :: _disp_radars()
  {
    unsigned r = _map->real_to_pixel(_robot->get_radius())  / 2;
    unsigned x = _map->real_to_pixel(_robot->get_pos().x());
    unsigned y = _map->real_to_pixel(_robot->get_pos().y());

    for (size_t i = 0; i < _robot->get_radars().size(); ++i)
      {
	const Radar& radar = _robot->get_radars()[i];
	if (radar.get_activated_slice() != -1)
	  {
	    float a1 = _robot->get_pos().theta() + radar.get_inc() * radar.get_activated_slice();
	    float a2 = _robot->get_pos().theta() + radar.get_inc() * (radar.get_activated_slice() + 1);
	    _line(_screen,
		  cos(a1) * r + x, sin(a1) * r + y,
		  cos(a2) * r + x, sin(a2) * r + y,
		  0x0000FF);
	    assert(radar.get_color() < (int)_map->get_goals().size());
	    const Goal& g = _map->get_goals()[radar.get_color()];
	    unsigned gx = _map->real_to_pixel(g.get_x());
	    unsigned gy = _map->real_to_pixel(g.get_y());
	    _line(_screen, x, y, gx, gy, 0x0000FF);
	  }

      }
      
  }
  void Display :: _disp_bumpers()
  {
    // convert to pixel
    unsigned x = _map->real_to_pixel(_robot->get_pos().x());
    unsigned y = _map->real_to_pixel(_robot->get_pos().y());
    unsigned r = _map->real_to_pixel(_robot->get_radius());
    float theta = _robot->get_pos().theta();
    Uint32 cb_left = SDL_MapRGB(_screen->format, _robot->get_left_bumper() ? 255 : 0, 0, 0);
    Uint32 cb_right = SDL_MapRGB(_screen->format, _robot->get_right_bumper() ? 255 : 0, 0, 0);
    _line(_screen,
	  (int) (r * cosf(theta + M_PI / 2.0f) + x),
	  (int) (r * sinf(theta + M_PI / 2.0f) + y),
	  (int) (r * cosf(theta) + x),
	  (int) (r * sinf(theta) + y),
	  cb_right);
    _line(_screen,
	  (int) (r * cosf(theta - M_PI / 2.0f) + x),
	  (int) (r * sinf(theta - M_PI / 2.0f) + y),
	  (int) (r * cosf(theta) + x),
	  (int) (r * sinf(theta) + y),
	  cb_left);
  }

  Uint32 Display :: _color_from_id(SDL_Surface* surf, int x)
  {
    std::bitset<4> bs = x;	
    Uint8 k = 1 - bs[4];
    Uint8 r = bs[0] * 128 + 127 * k;
    Uint8 g = bs[1] * 128 + 127 * k;
    Uint8 b = bs[2] * 128 + 127 * k;
    
    return SDL_MapRGB(surf->format, r, g, b);
  }
  
  void Display :: _disp_switches()
  {
    for (size_t i = 0; i < _map->get_illuminated_switches().size(); ++i)
      {
	const IlluminatedSwitch& sw = *_map->get_illuminated_switches()[i];
	unsigned x = _map->real_to_pixel(sw.get_x());
	unsigned y = _map->real_to_pixel(sw.get_y());
	unsigned rad = _map->real_to_pixel(sw.get_radius());
	Uint32 color = _color_from_id(_screen, sw.get_color());

	_circle(_screen, x, y, rad, color);
	if (sw.get_on())
	  _disc(_screen, x, y, rad, color);
      }
  }


  void Display :: _disp_lasers(const std::vector<Laser>& lasers)
  {
    for (size_t i = 0; i < lasers.size(); ++i)
      {
	unsigned x_laser = _map->real_to_pixel(_robot->get_pos().x() 
					       + lasers[i].get_gap_dist() 
					       * cosf(_robot->get_pos().theta() 
						      + lasers[i].get_gap_angle()));
	unsigned y_laser = _map->real_to_pixel(_robot->get_pos().y() 
					       + lasers[i].get_gap_dist() 
					       * sinf(_robot->get_pos().theta()
						      + lasers[i].get_gap_angle()));
	_line(_screen, x_laser, y_laser,
	      lasers[i].get_x_pixel(),
	      lasers[i].get_y_pixel(),
	      0xFF00000);
      }
  }

  void Display :: _disp_light_sensors()
  {
    for (size_t i = 0; i < _robot->get_light_sensors().size(); ++i)
      {
	const LightSensor& ls = _robot->get_light_sensors()[i];
	unsigned x_ls = _map->real_to_pixel(_robot->get_pos().x());
	unsigned y_ls = _map->real_to_pixel(_robot->get_pos().y());
	unsigned x_ls1 = _map->real_to_pixel(_robot->get_pos().x() 
					     + 200./((float)ls.get_color() + 1) 
					     * cosf(_robot->get_pos().theta() 
						    + ls.get_angle()-ls.get_range()/2.0));
	unsigned y_ls1 = _map->real_to_pixel(_robot->get_pos().y() 
					     + 200./((float)ls.get_color() + 1)
					     * sinf(_robot->get_pos().theta()
						    + ls.get_angle()-ls.get_range()/2.0));	
	_line(_screen, x_ls, y_ls, x_ls1, y_ls1, _color_from_id(_screen, ls.get_color()));
	unsigned x_ls2 = _map->real_to_pixel(_robot->get_pos().x() 
					     + 200./((float)ls.get_color() + 1)
					     * cosf(_robot->get_pos().theta() 
						    + ls.get_angle()+ls.get_range()/2.0));
	unsigned y_ls2 = _map->real_to_pixel(_robot->get_pos().y() 
					     + 200./((float)ls.get_color() + 1)
					     * sinf(_robot->get_pos().theta()
						    + ls.get_angle()+ls.get_range()/2.0));	
	_line(_screen, x_ls, y_ls, x_ls2, y_ls2, _color_from_id(_screen, ls.get_color()));
       	_line(_screen, x_ls1, y_ls1, x_ls2, y_ls2, _color_from_id(_screen, ls.get_color()));

	if (ls.get_activated())
	  {
	    const IlluminatedSwitch& is = 
	      *_map->get_illuminated_switches()[ls.get_num()];
	    unsigned x_is = _map->real_to_pixel(is.get_x());
	    unsigned y_is = _map->real_to_pixel(is.get_y());
	    _line(_screen, x_ls, y_ls, x_is, y_is, _color_from_id(_screen, is.get_color()));
	  }
      }
  }

  void Display :: _disp_camera()
  {
    static const int pw = 20;
    if (!_robot->camera_enabled())
      return;
    unsigned x_ls = _map->real_to_pixel(_robot->get_pos().x());
    unsigned y_ls = _map->real_to_pixel(_robot->get_pos().y());
    float a1 = _robot->get_pos().theta() + _robot->get_camera().get_angular_range() / 2.0;
    _line(_screen, x_ls, y_ls, cos(a1) * 200 + x_ls, 
	  sin(a1) * 200 + y_ls, 0x0000ff);
    float a2 = _robot->get_pos().theta() - _robot->get_camera().get_angular_range() / 2.0;
    _line(_screen, x_ls, y_ls, cos(a2) * 200 + x_ls, 
	  sin(a2) * 200 + y_ls, 0x0000ff);

    for (size_t i = 0; i < _robot->get_camera().pixels().size(); ++i)
      {
	int pix = _robot->get_camera().pixels()[i];	
	Uint32 color = pix == -1 ? 0xffffff : _color_from_id(_screen, pix);
	SDL_Rect r; r.x = i * pw; r.y = 0; r.w = pw; r.h = pw;
	SDL_FillRect(_screen, &r, color);
      }

  }

  void Display :: update()
  {
    _events();
    // convert to pixel
    unsigned x = _map->real_to_pixel(_robot->get_pos().x());
    unsigned y = _map->real_to_pixel(_robot->get_pos().y());
    unsigned r = _map->real_to_pixel(_robot->get_radius());
    float theta = _robot->get_pos().theta();
    
    // erase robot
    SDL_BlitSurface(_map_bmp, &_prev_bb, _screen, &_prev_bb);
    // erase all
    SDL_BlitSurface(_map_bmp, 0, _screen, 0);

    // lasers
    _disp_lasers();

    // goals
    _disp_goals();
    
    
    // light sensor
    _disp_light_sensors();

    // radars
    _disp_radars();
    
    // camera
    _disp_camera();

    // draw the circle again (robot)
	unsigned int col=_robot->color();
    _disc(_screen, x, y, r, _color_from_id(_screen,col));
	_circle(_screen,x,y,r,255,0,0);
    // direction
    Uint32 color = SDL_MapRGB(_screen->format, 0, 255, 0);
    _line(_screen, x, y, (int) (r * cosf(theta) + x), (int)(r * sinf(theta) + y), color);

    // bumpers
    _disp_bumpers();

    // illuminated switches
    _disp_switches();

        
    SDL_Rect rect;
    _bb_to_sdl(_robot->get_bb(), &rect);
    using namespace std;
    rect.x = max(0, min((int)rect.x, (int)_prev_bb.x));
    rect.y = max(0, min((int)rect.y, (int)_prev_bb.y));
    rect.w = max(rect.w, _prev_bb.w);
    rect.h = max(rect.h, _prev_bb.h);
    
    if (rect.x + rect.w > _w) rect.w = _w;
    if (rect.y + rect.h > _h) rect.h = _h;
    
    // the fast one
    //SDL_UpdateRect(_screen, rect.x, rect.y, rect.w, rect.h);
    // the slow one (needed when we have more than a circle to draw...)
    SDL_UpdateRect(_screen, 0, 0, _screen->w, _screen->h);
    _bb_to_sdl(_robot->get_bb(), &_prev_bb);

  }



}

#endif
