/*
** misc.hh
** Login : <mouret@asuncion.lip6.fr>
** Started on  Mon Jan 14 16:38:42 2008 Jean-Baptiste MOURET
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

#ifndef   	FASTSIM_MISC_HH_
# define   	FASTSIM_MISC_HH_

namespace fastsim
{
  class Exception
  {
  public:
    Exception(const char* msg) : _msg(msg) {};
    Exception(const std::string& str) : _msg(str) {}
    const std::string& get_msg() const { return _msg; }
  protected:
    std::string _msg;
  };

  template<typename T>
  inline T normalize_angle(T a)
  {
     while (a > M_PI)
       a -= 2*M_PI;
     while (a < -M_PI)
       a += 2*M_PI;
     return a;
  }  
  
  template<typename T>
  inline T normalize_angle_2pi(T a)
  {
     while (a > 2 * M_PI)
       a -= 2 * M_PI;
     while (a < 0)
       a += 2 * M_PI;
     return a;
  }  

}

#endif	    /* !MISC_HH_ */
