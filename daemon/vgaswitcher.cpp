/*
    GPU switching daemon
    Copyright (C) 2011  Radu Andries <admiral0@tuxfamily.org>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "vgaswitcher.h"
#include "vgadbus.h"
#include "vgadadaptor.h"

VgaSwitcher::VgaSwitcher(int& argc, char** argv): QCoreApplication(argc, argv)
{
  dbus=new VgaDbus();
  adaptor=new VgaSwitcherooAdaptor(dbus);
  
}

VgaSwitcher::~VgaSwitcher()
{

}
VgaDbus* VgaSwitcher::getDbus()
{
  return dbus;
}



#include "vgaswitcher.moc"