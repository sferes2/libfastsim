/*
** robot.hh
** Login : <mouret@asuncion.lip6.fr>
** Started on  Mon Jan 14 16:40:06 2008 Jean-Baptiste MOURET
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

#ifndef   	ROBOT_HH_
# define   	ROBOT_HH_

#include <vector>

#include "map.hpp"
#include "posture.hpp"
#include "laser.hpp"
#include "radar.hpp"
#include "light_sensor.hpp"
#include "linear_camera.hpp"
#include "laser_scanner.hpp"

namespace fastsim
{
  class Robot
  {
  public:
    struct BoundingBox { float x, y, w, h; };
    Robot(float radius) : 
      _radius(radius),
      _pos(0, 0, 0),
      _left_bumper(false),
      _right_bumper(false),
      _use_camera(false),
      _collision(false),
      _color(1),
      _vx(0), _vy(0), _va(0)
    {
      _bb.w = _radius * 2 + 8;
      _bb.h = _radius * 2 + 8;      
      _update_bb();
    }
    Robot(float radius, const Posture& pos) :
      _radius(radius), _pos(pos), 
      _left_bumper(false),
      _right_bumper(false),
      _use_camera(false),
      _collision(false),
      _vx(0), _vy(0), _va(0)
    {
      _bb.w = _radius * 2 + 8;
      _bb.h = _radius * 2 + 8;      
      _update_bb();
    }
    //
    void reinit()
    {
      _collision = false;
      _left_bumper = false;
      _right_bumper = false;
    }
    void move(float v1, float v2, const boost::shared_ptr<Map>& m);
    const Posture& get_pos() const { return _pos; }
    void set_pos(const Posture& pos) { _pos = pos; }
    float get_vx() const { return _vx; }
    float get_vy() const { return _vy; }
    float get_va() const { return _va; }

    const BoundingBox& get_bb() const { return _bb; }
    float get_radius() const { return _radius; }
    bool get_collision() const { return _collision;}
    //
    bool get_left_bumper() const { return _left_bumper; }
    bool get_right_bumper() const { return _right_bumper; }

    // lasers
    void add_laser(const Laser& l) { _lasers.push_back(l); }
    const std::vector<Laser>& get_lasers() const { return _lasers; }

    // laser scanners
    void add_laser_scanner(const LaserScanner& l) { _laser_scanners.push_back(l); }
    const std::vector<LaserScanner>& get_laser_scanners() const { return _laser_scanners; }

    // radars
    void add_radar(const Radar& r){ _radars.push_back(r); }
    const std::vector<Radar>& get_radars() const { return _radars; }

    // light sensors
    void add_light_sensor(const LightSensor& l) { _light_sensors.push_back(l); }
    const std::vector<LightSensor>& get_light_sensors() const { return _light_sensors; }
    void set_color(unsigned int color) {_color=color;}
    unsigned int color() const {return _color;}

    // camera
    void use_camera(const LinearCamera& c) { _camera = c; _use_camera = true; }
    void use_camera() { _use_camera = true; }
    const LinearCamera& get_camera() const { return _camera; }
    bool use_camera() const { return _use_camera; }
  protected:
    bool _check_collision(const boost::shared_ptr<Map>& m);
    void _update_bb();
    float _radius;
    Posture _pos;
    BoundingBox _bb;
    bool _left_bumper, _right_bumper;
    std::vector<Laser> _lasers;
    std::vector<Radar> _radars;
    std::vector<LightSensor> _light_sensors;
    std::vector<LaserScanner> _laser_scanners;
    LinearCamera _camera;
    bool _use_camera;
    bool _collision;
    unsigned int _color;
    float _vx, _vy, _va;
  };
}


#endif	    /* !ROBOT_HH_ */
