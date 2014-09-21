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

#ifndef   	LASER_SCANNER_HH_
# define   	LASER_SCANNER_HH_

#include "laser.hpp"

namespace fastsim
{
  class LaserScanner
  {
  public:
    LaserScanner(float angle_min, float angle_max,
		 float angle_increment,
		 float range_max) :
      _angle_min(angle_min),
      _angle_max(angle_max),
      _angle_increment(angle_increment),
      _range_max(range_max) {
      for (float a = angle_min; a <= angle_max; a += angle_increment)
	_lasers.push_back(Laser(a, range_max));
    }
    void update(const Posture& pos,
		 const boost::shared_ptr<Map>& map){
      for (size_t i = 0; i < _lasers.size(); ++i)
	_lasers[i].update(pos, map);
    }    
    float get_range_max() const { return _range_max; }
    float get_angle_max() const { return _angle_max; }
    float get_angle_min() const { return _angle_min; }
    float get_angle_increment() const { return _angle_increment; }
    const std::vector<Laser>& get_lasers() const { return _lasers; }
  protected:
    float _angle_min;
    float _angle_max;
    float _angle_increment;
    float _range_max;
    std::vector<Laser> _lasers;
  };
}


#endif	    /* !LASER_HH_ */
