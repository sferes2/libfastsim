#include "fastsim.hpp"

namespace fastsim {
  Settings::Settings(const std::string& config_file) {
    using boost::property_tree::ptree;
    ptree pt;
    read_xml(config_file, pt);
        
    ptree &n_map = pt.get_child("fastsim.map");
    _map = boost::shared_ptr<Map>
      (new Map(_get_a<std::string>(n_map, "name").c_str(),
	       _get_a<float>(n_map, "size")));

    ptree &n_robot = pt.get_child("fastsim.robot");
    _robot = boost::shared_ptr<Robot>
      (new Robot(_get_a<float>(n_robot, "diameter"),
		 Posture(_get_a<float>(n_robot, "x"),
			 _get_a<float>(n_robot, "y"),
			 _get_a<float>(n_robot, "theta") / 180.0 * M_PI)));

    ptree &n_asv = pt.get_child("fastsim.asv");
    // mass of rigid body
    float mass = _get_a<float>(n_asv, "mass");
    float iz   = _get_a<float>(n_asv, "moment_z");
    // added mass coefficients
    float xu1  = _get_a<float>(n_asv, "addedmass_xu1");
    float yv1  = _get_a<float>(n_asv, "addedmass_yv1");
    float nr1  = _get_a<float>(n_asv, "addedmass_nr1");
    float yr1  = _get_a<float>(n_asv, "addedmass_yr1");
    // linear drag coefficients
    float xu2  = _get_a<float>(n_asv, "draglinear_xu2");
    float yv2  = _get_a<float>(n_asv, "draglinear_yv2");
    float nr2  = _get_a<float>(n_asv, "draglinear_nr2");
    // quadratic drag coefficients
    float xu3  = _get_a<float>(n_asv, "dragquad_xu3");
    float yv3  = _get_a<float>(n_asv, "dragquad_yv3");
    float nr3  = _get_a<float>(n_asv, "dragquad_nr3");
    // Distance from CoM of boat to the propellers
    float distx  = _get_a<float>(n_asv, "com_propeller_distx");
    float disty  = _get_a<float>(n_asv, "com_propeller_disty");
    // Integration time step
    float deltaT  = _get_a<float>(n_asv, "deltaT");

     Matrix<float, 3, 3> mrb, ma, dl, dq; Vector2f pos_left, pos_right;
     mrb << mass, 0.0f, 0.0f,
            0.0f, mass, 0.0f,
            0.0f, 0.0f, iz;
     ma <<  xu1, 0.0f, 0.0f,
            0.0f, yv1, yr1,
            0.0f, yr1, nr1;
     dl <<  xu2, 0.0f, 0.0f,
            0.0f, yv2, 0.0f,
            0.0f, 0.0f, nr2;
     dq <<  xu3, 0.0f, 0.0f,
            0.0f, yv3, 0.0f,
            0.0f, 0.0f, nr3;
     pos_left  << distx, -disty;
     pos_right << distx,  disty;

    _asv = boost::shared_ptr<ASV>
      (new ASV(mass, iz, mrb, ma, dl, dq, pos_left, pos_right, deltaT));


    ptree& n_display = pt.get_child("fastsim.display");
    _display = _get_a_bool(n_display, "enable");
    std::cout<<"display=" << _display << std::endl;

    BOOST_FOREACH(ptree::value_type &v, pt.get_child("fastsim")) {
      if (v.first == "goal")
	_map->add_goal
	  (Goal(_get_a<float>(v.second, "x"),
		_get_a<float>(v.second, "y"),
		_get_a<float>(v.second, "diameter"),
		_get_a<int>(v.second, "color")));
      else if (v.first == "laser")	 
	_robot->add_laser
	  (Laser(_get_a<float>(v.second, "angle") / 180 * M_PI, 
		 _get_a<float>(v.second, "range")));	
      else if (v.first == "radar")
	_robot->add_radar
	  (Radar(_get_a<float>(v.second, "color"),
		 _get_a<int>(v.second, "slices")));
      else if (v.first == "illuminated_switch")
	_map->add_illuminated_switch
	  (Map::ill_sw_t
	   (new IlluminatedSwitch
	    (_get_a<int>(v.second, "color"),
	     _get_a<float>(v.second, "radius"),
	     _get_a<float>(v.second, "x"),
	     _get_a<float>(v.second, "y"),
	     _get_a_bool(v.second, "on"))));
      else if (v.first == "laser_scanner")
	_robot->add_laser_scanner
	  (LaserScanner(_get_a<float>(v.second, "angle_min") / 180 * M_PI, 
			_get_a<float>(v.second, "angle_max") / 180 * M_PI,
			_get_a<float>(v.second, "angle_inc") / 180 * M_PI,
			_get_a<float>(v.second, "range")));
      else if (v.first == "light_sensor")
	_robot->add_light_sensor
	  (LightSensor(_get_a<int>(v.second, "color"),
		       _get_a<float>(v.second, "angle") / 180 * M_PI,
		       _get_a<float>(v.second, "angular_range") / 180 * M_PI));
    }
  }
}
