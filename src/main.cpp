/*
** main.cc
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

int main(int argc, char* argv[]) {
  using namespace fastsim;
  if (argc != 2) {
    std::cerr<<"This program cannot run: you need to provide a XML file (e.g.," << argv[0] << " worlds/example.xml)" << std::endl;
    exit(1);
  }
  fastsim::Settings settings(argv[1]);
  std::shared_ptr<Map> map = settings.map();
  std::shared_ptr<Robot> robot = settings.robot();

  Display d(map, robot);
  
  for (int i = 0; i < 10000; ++i)
    {
      std::cout << "Step " << i << " robot pos: x = "<< robot->get_pos().x() <<"    y = "<< robot->get_pos().y() <<"    theta = "<< robot->get_pos().theta() << std::endl;
      d.update();
      robot->move(1.0, 1.1, map);
      usleep(1000);
    }
  return 0;
}
// Example of a main without a config file
/*
int main()
{
  try
    {
      using namespace fastsim; 
      std::shared_ptr<Map> m = 
	std::shared_ptr<Map>(new Map("cuisine.pbm", 600));
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
*/
