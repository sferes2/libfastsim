#ifndef         FASTSIM_ILLUMINATED_HHP_
# define        FASTSIM_ILLUMINATED_HHP_

#include <vector>
#include "posture.hpp"

namespace fastsim
{
   // an illimuninated light switch
  class IlluminatedSwitch
  {
    public:
       // in WORLD coordinate (NOT pixel)
      IlluminatedSwitch(int color, float radius, float x, float y, bool on) :
        _color(color),
        _radius(radius),
        _x(x), _y(y),
        _on(on), _activated(false)
      {
      }

      void try_to_activate(const Posture& robot_pos)
      {
        float d = robot_pos.dist_to(_x, _y);
        if (_on && d < _radius)
          activate();
      }
      void activate()
      {
        _activated = true;
        for (size_t i = 0; i < _linked_lights.size(); ++i)
          _linked_lights[i]->set_on(true);
      }
      void deactivate()
      {
        _activated = false;
        for (size_t i = 0; i < _linked_lights.size(); ++i)
          _linked_lights[i]->set_on(false);
      }
      void set_on(bool x = true) { _on = x; }
      void set_off() { set_on(false); }
      bool get_on() const { return _on;  }
      bool get_off() const { return !_on; }
      int  get_color() const { return _color; }
      float get_radius() const { return _radius; }
      float get_x() const { return _x; }
      float get_y() const { return _y; }
      void set_pos(float x, float y) { _x = x; _y = y; }
      bool get_activated() const { return _activated; }
      void link(boost::shared_ptr<IlluminatedSwitch> o) { _linked_lights.push_back(o); }
    protected:
      int _color;
      float _radius;
      float _x, _y;
      bool _on;
      bool _activated;
      std::vector<boost::shared_ptr<IlluminatedSwitch> > _linked_lights;
  };

  struct ClosestSwitch_f
  {
    ClosestSwitch_f(float x, float y) : _x(x), _y(y) {}
    bool operator()(const boost::shared_ptr<IlluminatedSwitch> i1,
		    const boost::shared_ptr<IlluminatedSwitch> i2)
    {
      float x1 = i1->get_x() - _x;
      float y1 = i1->get_y() - _y;
      float x2 = i2->get_x() - _x;
      float y2 = i2->get_y() - _y;
      float d1 = sqrtf(x1 * x1 + y1 * y1);
      float d2 = sqrtf(x2 * x2 + y2 * y2);
      return d1 < d2;
    }
    float _x, _y;
  };

}

#endif
