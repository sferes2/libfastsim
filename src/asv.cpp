/*
** asv.cpp
*/

#include "asv.hpp"

namespace fastsim
{
    void ASV :: init()
    {
        // assert propeller positions are aligned along y-axis and equidistant from x-coordinate CoM
        assert(_prop_pos_left(0) ==  _prop_pos_right(0));
        assert(_prop_pos_left(1) == -_prop_pos_right(1));

        // Compute _com_prop_distx and _com_prop_disty from _prop_pos_left and _prop_pos_right
        _com_prop_distx =  _prop_pos_left(0);
        _com_prop_disty =  _prop_pos_left(1);

        // verify mass matrix
        assert(_mass_rigidbody(0,0) == _mass);
        assert(_mass_rigidbody(1,1) == _mass);
        assert(_mass_rigidbody(2,2) == _moment_z);

        // verify added mass matrix coefficients
        assert(_mass_added(1,2) == _mass_added(2,1));

        // Compute total mass
         _mass_total = _mass_rigidbody + _mass_added;

         // initialize position, velocity and acceleration vectors
         _acceleration << 0.0f, 0.0f, 0.0f;
         _velocity     << 0.0f, 0.0f, 0.0f;
         _position     << 0.0f, 0.0f, 0.0f;
    }

    void ASV :: inverseDynamics()
    {
        //added mass matrix coefficients
        float Xu1 = _mass_added(0,0);
        float Yv1 = _mass_added(1,1);
        float Yr1 = _mass_added(1,2);

        // Compute Coriolis force for rigid body
        //CRB = [0 -m*r 0; m*r 0 0; 0 0 0];
        _coriolis_rigidbody << 0.0f, -_mass*_velocity(2), 0.0f,
                               -_mass*_velocity(2), 0.0f, 0.0f,
                               0.0f, 0.0f, 0.0f;

        // Compute Coriolis force for added mass
        _coriolis_added << 0.0f, 0.0f, -(Yv1*_velocity(1) + Yr1*_velocity(2)),
                           0.0f, 0.0f, Xu1*_velocity(0),
                           (Yv1*_velocity(1)+Yr1*_velocity(2)), -Xu1*_velocity(0), 0.0f;

        // compute drag force from quadractic drag coefficients
        Matrix<float, 3, 3> drag_quadratic;
        drag_quadratic << _drag_quadratic(0,0)*std::abs(_velocity(0)), 0.0f, 0.0f,
                           0.0f, _drag_quadratic(1,1)*std::abs(_velocity(1)), 0.0f,
                           0.0f, 0.0f, _drag_quadratic(2,2)*std::abs(_velocity(2));

        _acceleration = _mass_total.inverse() * (_thrust - _coriolis_rigidbody*_velocity - _coriolis_added*_velocity - _drag_linear*_velocity - drag_quadratic*_velocity);
    }

    void ASV :: kinematics()
    {
        _velocity = integral(_acceleration, _velocity);
        Matrix<float, 3, 3> jacobian;
        jacobian << cos(_position(2)), -sin(_position(2)), 0.0,
                    sin(_position(2)),  cos(_position(2)), 0.0,
                    0.0, 0.0, 1.0;

        Vector3f velocity_inertialframe = jacobian * _velocity;
        _position = integral(velocity_inertialframe, _position);
    }

    Vector3f ASV :: integral(Vector3f & x_dot, Vector3f & x)
    {
        return x + _delta_t * x_dot;
    }

    void ASV :: set_thrust(float fl, float fr, float angle)
    {
        _angle_prop = angle;

        /*COMPUTE THE THRUST MATRIX FROM THE PROPELLER ANGLE, PROPELLER FORCES AND PROPELLER POSITION WRT CoM*/
        _thrust << (fl + fr) * cos(angle), (fl + fr) * sin(angle), (-fl + fr) * _com_prop_disty * cos(angle) + (fl + fr) * _com_prop_distx * sin(angle);
    }
}
