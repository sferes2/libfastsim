/*
** laser.hh
** Login : <mandor@ithaqua>
** Started on  Wed Jun  3 00:39:20 2009 mandor
** $Id$
** 
** Copyright (C) 2009 mandor
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

#ifndef   	LASER_HH_
# define   	LASER_HH_
#include "map.hpp"
#include "posture.hpp"
#include <boost/shared_ptr.hpp>

namespace fastsim
{
  class Laser
  {
  public:
    Laser(float angle, float range, float gap_dist = 0.0f, float gap_angle = 0.0f) :
      _angle(angle), _range(range),
      _gap_dist(gap_dist), _gap_angle(gap_angle),
      _x(0), _y(0), 
      _x_pixel(0), _y_pixel(0),
      _dist(-1) {}
    float update(const Posture& pos,
		 const boost::shared_ptr<Map>& map);
    float get_dist() const { return _dist; }
    float get_angle() const { return _angle; }
    float get_range() const { return _range; }
    float get_gap_dist() const { return _gap_dist; }
    float get_gap_angle() const { return _gap_angle; }
    // intersection point in world coordinates
    float get_x() const { return _x; }
    float get_y() const { return _y; }
    // intersection point in pixel coordinates
    int get_x_pixel() const { return _x_pixel; }
    int get_y_pixel() const { return _y_pixel; }
  protected:
    float _angle;
    float _range;
    float _gap_dist, _gap_angle;//polar coordinates of the laser (reference = robot)
    float _x, _y;
    int _x_pixel, _y_pixel;
    float _dist;
    // 
    bool _try_pixel(const boost::shared_ptr<Map>& m, int x, int y);
    bool _line_inter(const boost::shared_ptr<Map>& m,
		     int y1, int x1, // src
		     int y2, int x2, // dest
		     int& x_res, int& y_res //res
		     );

  };
}


#endif	    /* !LASER_HH_ */
