#ifndef FASTSIM_LINEAR_CAMERA_HPP_
#define FASTSIM_LINEAR_CAMERA_HPP_

#include <boost/shared_ptr.hpp>
#include "posture.hpp"
#include "map.hpp"

namespace fastsim
{
  class LinearCamera
  {
    public:
    LinearCamera() : _angular_range(M_PI / 2), _pixels(12)
    { std::fill(_pixels.begin(), _pixels.end(), -1); }
    LinearCamera(float angular_range, int nb_pixels) :
      _angular_range(angular_range), _pixels(nb_pixels)
    { std::fill(_pixels.begin(), _pixels.end(), -1); }
   
    void update(const Posture& pos,
	       const boost::shared_ptr<Map>& map);
    const std::vector<int>& pixels() const { return _pixels; }
    float get_angular_range() const { return _angular_range; }
  protected:
    float _angular_range;
    std::vector<int> _pixels;
  };
}
#endif
