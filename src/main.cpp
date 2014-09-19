/*
** main.cc
** Login : <mouret@asuncion.lip6.fr>
** Started on  Mon Jan 14 16:44:52 2008 Jean-Baptiste MOURET
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

#include <iostream>
#include "fastsim.hpp"

int main()
{
  try
    {
      using namespace fastsim;
      boost::shared_ptr<Map> m = 
	boost::shared_ptr<Map>(new Map("cuisine.pbm", 600));
      m->add_goal(Goal(100, 100, 10, 0));
      Robot r(20.0f, Posture(60, 600-60, 0));
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

