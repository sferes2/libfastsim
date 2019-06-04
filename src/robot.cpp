/*
** robot.cc
** Login : <mouret@asuncion.lip6.fr>
** Started on  Mon Jan 14 16:40:38 2008 Jean-Baptiste MOURET
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

#include <iostream>
#include <list>
#include <cmath>

#include "robot.hpp"

namespace fastsim
{
  void Robot :: move(float v1, float v2, const std::shared_ptr<Map>& m, bool sticky_walls)
  {
    Posture prev = _pos;
    _pos.move(v1, v2, _radius * 2);
    _update_bb();
    // update bumpers & go back if there is a collision
    if(_check_collision(m))
      {
	float theta = _pos.theta();
	_pos = prev;
	if (!sticky_walls) // activate if you want to turn when in collision
		_pos.set_theta(theta);
	_collision = true;
      }
    _vx = _pos.x() - prev.x();
    _vy = _pos.y() - prev.y();
    _va = _pos.theta() - prev.theta();

    // update lasers
    for (size_t i = 0; i < _lasers.size(); ++i)
      _lasers[i].update(_pos, m);
    for (size_t i = 0; i < _laser_scanners.size(); ++i)
      _laser_scanners[i].update(_pos, m);

    // update radars
    for (size_t i = 0; i < _radars.size(); ++i)
      _radars[i].update(_pos, m);
    // update light sensors
    for (size_t i = 0; i < _light_sensors.size(); ++i)
      _light_sensors[i].update(_pos, m);
    // update camera
    if (_use_camera)
      _camera.update(_pos, m);
  }

  void Robot :: _update_bb()
  {
    // robot bb
    _bb.x = _pos.x() - _radius - 4;
    _bb.y = _pos.y() - _radius - 4;
  }

  bool Robot :: _check_collision(const std::shared_ptr<Map>& m)
  {
    // pixel wise
    int rp = m->real_to_pixel(_radius);
    int r = rp * rp;
    int x = m->real_to_pixel(_pos.x());
    int y = m->real_to_pixel(_pos.y());
    int bbx = m->real_to_pixel(_bb.x);
    int bby = m->real_to_pixel(_bb.y);
    int bbw = m->real_to_pixel(_bb.x + _bb.w);
    int bbh = m->real_to_pixel(_bb.y + _bb.h);

    typedef std::pair<int, int> p_t;
    std::list<p_t > coll_points;
    for (int i = bbx; i < bbw; ++i)
      for (int j = bby; j < bbh; ++j)
	if (m->get_pixel(i, j) == Map::obstacle)
	  {
	    float d1 = (i - x);
	    float d2 = (j - y);
	    if (d1 * d1 + d2 * d2 <= r)
	      coll_points.push_back(p_t(i, j));
	  }
    _left_bumper = false;
    _right_bumper = false;
    if (coll_points.empty())
      return false;
    else
      {
	// bumpers
	for (std::list<p_t>::const_iterator it = coll_points.begin();
	     it != coll_points.end(); ++it)
	  {
	    float a = Posture::normalize_angle(atan2(it->second - y,
						     it->first - x) - _pos.theta());
	    if (a > 0 && a < M_PI / 2.0)
	      _right_bumper = true;
	    if (a < 0 && a > -M_PI / 2.0)
	      _left_bumper = true;
	  }
	return true;
      }
  }
}
