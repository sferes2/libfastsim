#ifndef FASTSIM_SETTINGS_HPP_
#define FASTSIM_SETTINGS_HPP_

#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp> 


namespace fastsim {
  class Settings {
  public:
    // parse a xml file to instantiate the map & the robot
    Settings(const std::string& xml_file);
    boost::shared_ptr<Map> map() { return _map; }
    boost::shared_ptr<Robot> robot() { return _robot; }
    bool display() const { return _display; }
  protected:
    boost::shared_ptr<Map> _map;
    boost::shared_ptr<Robot> _robot;
    bool _display;

    template<typename R>
    R _get_a(const boost::property_tree::ptree& v, const char *a) const {
      std::string s("<xmlattr>.");
      return v.get<R>(s + std::string(a));
    }
    bool _get_a_bool(const boost::property_tree::ptree& v, 
		     const char* a) const {
      std::string s("<xmlattr>.");
      std::string r = v.get<std::string>(s + std::string(a));
      if (r == "true")
	return true;
      return false;
    }

  };
}

#endif
