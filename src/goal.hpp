#ifndef FASTSIM_GOAL_HPP_
#define FASTSIM_GOAL_HPP_

namespace fastsim
{
  class Goal
  { 
  public:
    Goal(float x, float y, float diam, int color) :
      _x(x), _y(y), _diam(diam), _color(color) {}
    float get_x() const { return _x; }
    float get_y() const { return _y; }
    float get_diam() const { return _diam; }
    int   get_color() const { return _color; }
  private:
    float _x, _y;
    float _diam;
    int _color; 
  };
}

#endif
