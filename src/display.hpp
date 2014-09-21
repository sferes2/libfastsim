/*
** sdl.hh
** Login : <mouret@asuncion.lip6.fr>
** Started on  Mon Jan 14 16:42:14 2008 Jean-Baptiste MOURET
** $Id$
** 
** Copyright (C) 2008 Jean-Baptiste MOURET
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

#ifndef   	SDL_HH_
# define   	SDL_HH_

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <valarray>
#include <stdio.h>
#include <unistd.h>

#ifdef USE_SDL
#include <SDL.h>
#endif

#include "map.hpp"
#include "robot.hpp"
#include <boost/shared_ptr.hpp>

namespace fastsim
{
  class Display
  {

  public:
#ifdef USE_SDL
    Display(const boost::shared_ptr<Map>& m, const Robot& r);
    ~Display()
    {
      SDL_FreeSurface(_map_bmp);
      SDL_FreeSurface(_screen);
      SDL_Quit();
    }    
    void update();
    void update_map() 
    {
      _blit_map();
    }
#else
    Display(const boost::shared_ptr<Map>& m, const Robot& r) : _map(m), _robot(r) {}
    ~Display() {}
    void update(){}
    void update_map(){}
#endif
  protected:
    const boost::shared_ptr<Map>& _map;
    const Robot& _robot;
#ifdef USE_SDL
    void _events();
    void _bb_to_sdl(const Robot::BoundingBox& bb,
		    SDL_Rect *r)
    {
      r->x = (Sint16) _map->real_to_pixel(bb.x);
      r->y = (Sint16) _map->real_to_pixel(bb.y);
      r->w = (Sint16) _map->real_to_pixel(bb.w);
      r->h = (Sint16) _map->real_to_pixel(bb.h);
    }

    void _quit()
    {
      SDL_Quit();
      exit(0);
    }
    void _put_pixel(SDL_Surface* surf,
		    Uint32 color, int x, int y)
    {
      if (x >= surf->w || x < 0 || y >= surf->h || y < 0)
	return;
      Uint32 *bufp = (Uint32*)surf->pixels  + y * surf->pitch / 4 + x;
      *bufp = color;
    }
    void _put_pixel(SDL_Surface* surf,
		    int x, int y,
		    Uint8 r, Uint8 g, Uint8 b)
    {  _put_pixel(surf, SDL_MapRGB(surf->format, r, g, b), x, y); }
    
    void _blit_map();

    // used by _circle
    void _circle_points(SDL_Surface* surf,
			int cx, int cy, int x, int y, Uint32 color);
    void _circle(SDL_Surface* surf,
		 int x_center, int y_center, int radius,
		 Uint8 r, Uint8 g, Uint8 b)
    { _circle(surf, x_center, y_center, radius, SDL_MapRGB(surf->format, r, g, b)); }
    void _circle(SDL_Surface* surf,
		 int x_center, int y_center, int radius,
		 Uint32 color);
    //
    void _disc(SDL_Surface* surf,
	       int x_center, int y_center, int radius,
	       Uint8 r, Uint8 g, Uint8 b)
    { _disc(surf, x_center, y_center, radius, SDL_MapRGB(surf->format, r, g, b)); }
    void _disc(SDL_Surface* surf,
	       int x_center, int y_center, int radius,
	       Uint32 color);
    //
    void _line(SDL_Surface* surf, int x0, int y0, int x1, int y1, 
	       Uint8 r, Uint8 g, Uint8 b)
    { _line(surf, x0, y0, x1, y1, SDL_MapRGB(surf->format, r, g, b)); }
    void _line(SDL_Surface* surf, int x0, int y0, int x1, int y1, 
	       Uint32 color);
  
    void _try_pixel(bool& res,
		    SDL_Surface* surf,
		    Uint32 color,
		    int x, int y);
    
    //
    Uint32 _color_from_id(SDL_Surface* surf, int id);
    // disp sensor values
    void _disp_bb();
    void _disp_goals();
    void _disp_switches();
    void _disp_radars();
    void _disp_bumpers();
    void _disp_lasers(const std::vector<Laser>& lasers);
    void _disp_lasers() { 
      _disp_lasers(_robot.get_lasers()); 
      for (size_t i = 0; i < _robot.get_laser_scanners().size(); ++i)
	_disp_lasers(_robot.get_laser_scanners()[i].get_lasers());
    }
    void _disp_light_sensors();
    void _disp_camera();
    // 
    int _w, _h;
    SDL_Surface* _screen, *_map_bmp;
    SDL_Rect _prev_bb;
#endif
  };
}
#endif	    /* !SDL_HH_ */
