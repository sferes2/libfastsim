
/*
** posture.hh
** Login : <mouret@asuncion.lip6.fr>
** Started on  Mon Jan 14 16:39:35 2008 Jean-Baptiste MOURET
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

#ifndef   	POSTURE_HH_
# define   	POSTURE_HH_
#include <cmath>

namespace fastsim
{
  class Posture
  {
  public:
    Posture(float x, float y, float theta) : 
      _x(x), _y(y),
      _theta(theta)
    { }
    Posture(){}
    Posture(const Posture& p) : 
      _x(p._x), 
      _y(p._y), 
      _theta(p._theta)
    {}
    Posture& operator=(const Posture& o)
    {
      _x = o._x;
      _y = o._y;
      _theta = o._theta;
      return *this;
    }
    //
    float theta() const { return _theta;}
    void set_theta(float t) { _theta = t; }
    float x() const { return _x; }
    float y() const { return _y; }
    float get_x() const { return _x; }
    float get_y() const { return _y; }
    
    //
    float dist_to(const Posture& p) const
    {
      return dist_to(p.x(), p.y());
    }
    float dist_to(float x, float y) const
    {
      float xx = _x - x;
      float yy = _y - y;
      return sqrtf(xx * xx + yy * yy);
    }

    // 
    const Posture& rotate(float theta)
    {
      float x_ = cos(theta) * x() - sin(theta) * y();
      float y_ = cos(theta) * y() + sin(theta) * x();
      _x = x_;
      _y = y_;
      _theta += theta;
      return *this;
    }
    Posture operator+(const Posture& o) const
    {
      Posture p;
      p._x = _x + o._x;
      p._y = _y + o._y;
      p._theta   = normalize_angle(_theta + o._theta);
      return p;
    }
    const Posture& move(float d_l, float d_r, 
			float wheels_dist)
    {
      Posture old_pos = *this;
      float alpha = (d_r - d_l) / wheels_dist;
      Posture p;

      if (fabs(alpha) > 1e-10)
	{
	  float	r = (d_l / alpha) +  wheels_dist / 2;
	  float	d_x = (cos(alpha) - 1) * r;
	  float	d_y = sin(alpha) * r;
	  p = Posture(d_x, d_y, alpha);
	  p.rotate(old_pos.theta() - M_PI / 2);
	  p.set_theta(normalize_angle(alpha));
	}
      else
	p = Posture(d_l * cos(old_pos.theta()),
		    d_l * sin(old_pos.theta()),
		    0);
      *this = p + old_pos;
      return *this;
    }

    template<typename T>
    static inline T normalize_angle(T a)
    {
      while (a > M_PI)
	a -= 2*M_PI;
      while (a < -M_PI)
	a += 2*M_PI;
      return a;
    }  
  protected:
    float _x, _y;    
    float _theta;
  };

}


#endif	    /* !POSTURE_HH_ */
