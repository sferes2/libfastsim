#include <iostream>
#include "linear_camera.hpp"


namespace fastsim
{
  
  void LinearCamera :: update(const Posture& pos,
                             const std::shared_ptr<Map>& map)
  {
    float inc = _angular_range / _pixels.size();
    float r = -_angular_range / 2.0f;
    for (size_t i = 0; i < _pixels.size(); ++i, r += inc)
      {
	float alpha = r + pos.theta();
	float xr = cos(alpha) * 10000 + pos.x();//range = 10000
	float yr = sin(alpha) * 10000 + pos.x();
	assert(i < _pixels.size());
	_pixels[i] = map->check_inter_is(pos.x(), pos.y(), xr, yr);
      }
  }
}
