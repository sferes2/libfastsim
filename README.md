fastsim
=======


Fastsim is a fast, lightweight simulator of a wheeled robot (khepera-like).

This is the library version. A Sferes2 module and ros module are available in separate repositories. The ROS module currently uses this library, but not the sferes one (not yet).

*If you use this software in an academic article, please cite:*

Mouret, J.-B. and Doncieux, S. (2012). Encouraging Behavioral Diversity in Evolutionary Robotics: an Empirical Study. Evolutionary Computation. Vol 20 No 1 Pages 91-133.


Usage & installation
--------------------
- run `./waf configure` and `./waf build`

- running: `build/src/test_fastsim worlds/example.xml`

### Depedencies: 
- SDL 1.2 (can be deactivated)

Academic papers that use faststim
----------------------------------
- Jingyu Li, Jed Storie, and Jeff Clune. (2014). Encouraging creative thinking in robots improves their ability to solve challenging problems. In Proceedings of the 2014 conference on Genetic and evolutionary computation (GECCO '14). ACM, New York, NY, USA, 193-200. 
- Doncieux, S. (2014). Knowledge Extraction from Learning Traces in Continuous Domains.
AAAI 2014 fall Symposium ''Knowledge, Skill, and Behavior Transfer in Autonomous Robots''. Pages 1-8.
- Koos, S. and Mouret, J.-B. and Doncieux, S. (2013). The Transferability Approach: Crossing the Reality Gap in Evolutionary Robotics. IEEE Transactions on Evolutionary Computation. Vol 17 No 1 Pages 122 - 145 
- Doncieux, S. (2013). Transfer Learning for Direct Policy Search: A Reward Shaping Approach. Proceedings of ICDL-EpiRob conference. Pages 1-6.
- Doncieux, S. and Mouret, J.B. (2013). Behavioral Diversity with Multiple Behavioral Distances. Proc. of IEEE Congress on Evolutionary Computation, 2013 (CEC 2013). Pages 1-8.
- Mouret, J.-B. and Doncieux, S. (2012). Encouraging Behavioral Diversity in Evolutionary Robotics: an Empirical Study. Evolutionary Computation. Vol 20 No 1 Pages 91-133.
-  Mouret, J.-B. (2011). Novelty-based Multiobjectivization. New Horizons in Evolutionary Robotics: Extended Contributions from the 2009 EvoDeRob Workshop, Springer, publisher. Pages 139--154.
- Pinville, T. and Koos, S. and Mouret, J-B. and Doncieux, S. (2011). How to Promote Generalisation in Evolutionary Robotics: the ProGAb Approach. GECCO'11: Proceedings of the 13th annual conference on Genetic and evolutionary computation ACM, publisher . Pages 259--266.
-  Koos, S. and Mouret, J.-B. and Doncieux, S. (2010). Crossing the Reality Gap in Evolutionary Robotics by Promoting Transferable Controllers.
GECCO'10: Proceedings of the 12th annual conference on Genetic and evolutionary computation ACM, publisher . Pages 119--126.
- Doncieux, S. and Mouret, J.-B. (2010). Behavioral diversity measures for Evolutionary Robotics. WCCI 2010 IEEE World Congress on Computational Intelligence, Congress on Evolutionary Computation (CEC). Pages 1303--1310.
- Mouret, J.-B. and Doncieux, S. (2009). Overcoming the bootstrap problem in evolutionary robotics using behavioral diversity. IEEE Congress on Evolutionary Computation, 2009 (CEC 2009). Pages 1161 - 1168.

Minimal documentation
---------------------
Fastsim uses a bitmap (a pbm file) as an environment. It uses pixel-wise collision detection and interesection tests (lasers). There are two coordinate systems: world coordinates, and pixel coordinates. In general, everything is expressed in world coordinate and a scaling is automatically applied by fastsim to get the pixel coordinates.

Units:
- everything is expressed in world coordinates
- angles are in radians *except in the XML file*, where they are in degrees.

Main classes:
- `Map(const char* fname, float real_w)`: the main object of fastsim. It contains the robot, the goals, and the illuminated switches. `real_w` corresponds to the width of the map in world coordinates. Do not forget to call Map::update() at each time step
- `Robot(float radius)`: a khepera-like robot with a differential drive system. You need to add sensors to customize it (lasers, camera, radars, etc.). Bumper are added automatically.
- `Display(const boost::shared_ptr<Map>& m, const Robot& r)`: a SDL buffer (window) that can be used to visualize the robot, the environment, etc. (depends on SDL)
- `Settings(const std::string& xml_file)`: a simple XML parser to easily implement a configuration file for fastsim (see below)

Objects:
- `Goal(float x, float y, float diam, int color)`: goals are omnidirectional beacons that can be seen by `Radar` sensors. Depending on the configuration of the radar, the robot can see or not see goals through walls. The activated slice is -1 if the goal is not visible.
- `IlluminatedSwitch(int color, float radius, float x, float y, bool on)`: illuminated switches are omnidirectional beacons that can be hidden by walls. They can be switched on or off (`activate()`/`deactivate()`). An `IlluminatedSwitch` can be linked to other switches (`link()`) to create "circuits". Once put in a map, the illuminated switches are switched on if a robot touches it.

Sensors:
- `Laser(float angle, float range, float gap_dist = 0.0f, float gap_angle = 0.0f)`: a laser telemeter. The direction (relative to the robot frame) is controlled by the `angle`. If the robot does not see anything (no object in the range), then the distance is -1
- `LightSensor(int color, float angle, float range)`: A LightSensor senses an IlluminatedSwitch of the same color. The range is an angular range (there is no distance range), the angle is the orientation of the sensor relative to the robot. If there are several IlluminatedSwitches of the same color, the exact behavior is undefined.
- `Radar(int color, int nb_slices, bool through_walls = true)`: A radar sensor senses a Goal object of the same color. It is a circular sensor divided into slices. If a slices "see" a Goal of the right color, then this slice is activated. There can be only one slice activated at a time. If there are seveval goals of the same color, the behavior of the radar is undefined.

Example with a configuration file
---------------------------------
```C++

#include <iostream>
#include "fastsim.hpp"

int main(int argc, char** argv) {
  using namespace fastsim;
  assert(argc == 2);
  fastsim::Settings settings(argv[1]);
  boost::shared_ptr<Map> map = settings.map();
  boost::shared_ptr<Robot> robot = settings.robot();

  Display d(map, *robot);
  
  for (int i = 0; i < 10000; ++i)
    {
      d.update();
      robot->move(1.0, 1.1, map);
      usleep(1000);
    }
  return 0;
}
```

Configuration file:
```XML
<?xml version="1.0"?>
<fastsim>
  <display enable="true"/>
  <!-- world -->
  <map name="worlds/cuisine.pbm" size="600"/>
  <robot x="300" y="300" theta="0" diameter="30"/>
  <illuminated_switch x="250" y="450" color="0" radius="10" on="true"/>
  <light_sensor angle="100" color="0" angular_range="50"/>
  <goal x="100" y="100" color="0" diameter="10"/>
  
  <!-- sensors (optionals) -->
  <laser range="100" angle="45"/>
  <laser range="100" angle="-45"/>
  <!-- <laser_scanner angle_min="-100" angle_max="100" angle_inc="2" range="300"/> -->
  <radar slices="4" color="0"/>
</fastsim>
```

Example without a configuration file
-----------------------------------

```C++
#include <iostream>
#include "fastsim.hpp"

int main()
{
  try
    {
      using namespace fastsim; 
      boost::shared_ptr<Map> m = boost::shared_ptr<Map>(new Map("cuisine.pbm", 600));
      m->add_goal(Goal(100, 100, 10, 0));
      Robot r(20.0f, Posture(200, 200, 0));
      r.add_laser(Laser(M_PI / 4.0, 100.0f));
      r.add_laser(Laser(-M_PI / 4.0, 100.0f));
      r.add_laser(Laser(0.0f, 100.0f));
      r.add_radar(Radar(0, 4));
      Display d(m, r);

      float x = 30;
      for (int i = 0; i < 10000; ++i)
	{
	  d.update();
	  r.move(1.0, 1.1, m);
	}
    }
  catch (fastsim::Exception e)
    {
      std::cerr<<"error : "<<e.get_msg()<<std::endl;
    }
  return 0;
}
```


