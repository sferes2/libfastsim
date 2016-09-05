/*
** asv.hpp
*/

#ifndef   	ASV_HH_
# define   	ASV_HH_

//#include <vector>

/*#include "map.hpp"
#include "posture.hpp"
#include "laser.hpp"
#include "radar.hpp"
#include "light_sensor.hpp"
#include "linear_camera.hpp"
#include "laser_scanner.hpp"*/

#include <eigen3/Eigen/Dense>

using namespace Eigen;

namespace fastsim
{
  class ASV
  {
  public:
    /*struct BoundingBox { float x, y, w, h; };*/

    ASV()
    {
       ;
    }

    ASV(float m,
        float iz,
        Matrix<float, 3, 3> mrb,
        Matrix<float, 3, 3> ma,
        //Matrix<float, 3, 3> crb,
        //Matrix<float, 3, 3> ca,
        Matrix<float, 3, 3> dl,
        Matrix<float, 3, 3> dq,
        Vector2f pos_left,
        Vector2f pos_right,
        float dt
        ) :
      _mass(m),
      _moment_z(iz),
      _mass_rigidbody(mrb),
      _mass_added(ma),
      //_coriolis_rigidbody(crb),
      //_coriolis_added(ca),
      _drag_linear(dl),
      _drag_quadratic(dq),
      _prop_pos_left(pos_left),
      _prop_pos_right(pos_right),
      _delta_t(dt)
    {
        init();
      /*_bb.w = _radius * 2 + 8;
      _bb.h = _radius * 2 + 8;
      _update_bb();*/
    }

    /*Robot(float radius, const Posture& pos) :
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
    float get_va() const { return _va; }*/

    /*const BoundingBox& get_bb() const { return _bb; }
    float get_radius() const { return _radius; }
    bool get_collision() const { return _collision;}*/
    //
    /*bool get_left_bumper() const { return _left_bumper; }
    bool get_right_bumper() const { return _right_bumper; }*/

    // lasers
    /*void add_laser(const Laser& l) { _lasers.push_back(l); }
    const std::vector<Laser>& get_lasers() const { return _lasers; }*/

    // laser scanners
    /*void add_laser_scanner(const LaserScanner& l) { _laser_scanners.push_back(l); }
    const std::vector<LaserScanner>& get_laser_scanners() const { return _laser_scanners; }*/

    // radars
    /*void add_radar(const Radar& r){ _radars.push_back(r); }
    const std::vector<Radar>& get_radars() const { return _radars; }*/

    // light sensors
    /*void add_light_sensor(const LightSensor& l) { _light_sensors.push_back(l); }
    const std::vector<LightSensor>& get_light_sensors() const { return _light_sensors; }
    void set_color(unsigned int color) {_color=color;}
    unsigned int color() const {return _color;}*/

    // camera
    /*void use_camera(const LinearCamera& c) { _camera = c; _use_camera = true; }
    void use_camera() { _use_camera = true; }
    const LinearCamera& get_camera() const { return _camera; }
    bool use_camera() const { return _use_camera; }*/

    void init();
    void inverseDynamics();
    Vector3f integral(Vector3f & x_dot, Vector3f & x);
    void kinematics();


    Matrix<float, 3, 3> & get_mass_rigidbody() {return _mass_rigidbody;}
    Matrix<float, 3, 3> & get_mass_added() {return _mass_added;}
    Matrix<float, 3, 3> & get_coriolis_rigidbody() {return _coriolis_rigidbody;}
    Matrix<float, 3, 3> & get_coriolis_added() {return _coriolis_added;}
    Matrix<float, 3, 3> & get_drag_linear() {return _drag_linear;}
    Matrix<float, 3, 3> & get_drag_quadratic() {return _drag_quadratic;}
    Vector3f & get_pos() {return _position;}
    Vector3f & get_thrust() {return _thrust;}
    Vector3f & get_acceleration() {return _acceleration;}
    Vector3f & get_velocity() {return _velocity;}


    void set_mass_rigidbody(Matrix<float, 3, 3> MRB) {_mass_rigidbody = MRB;}
    void set_mass_added(Matrix<float, 3, 3> MA) {_mass_added = MA;}
    void set_coriolis_rigidbody(Matrix<float, 3, 3> CRB) {_coriolis_rigidbody = CRB;}
    void set_coriolis_added(Matrix<float, 3, 3> CA) {_coriolis_added = CA;}
    void set_drag_linear(Matrix<float, 3, 3> DL) {_drag_linear = DL;}
    void set_drag_quadratic(Matrix<float, 3, 3> DQ) {_drag_quadratic = DQ;}
    void set_pos( Vector3f POS) {_position = POS;}
    void set_thrust(float fl, float fr, float angle);

  protected:

    float _mass, _moment_z;
    Matrix<float, 3, 3> _mass_rigidbody, _mass_added, _mass_total;
    Matrix<float, 3, 3> _coriolis_rigidbody, _coriolis_added;
    Matrix<float, 3, 3> _drag_linear, _drag_quadratic;
    Vector3f _thrust; float _angle_prop;
    Vector3f _acceleration, _velocity, _position;

    Vector2f _prop_pos_left, _prop_pos_right; float _com_prop_distx, _com_prop_disty;

    float _delta_t;


    /*bool _check_collision(const boost::shared_ptr<Map>& m);
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
    float _vx, _vy, _va;*/
  };
}


#endif	    /* !ASV_HH_ */
