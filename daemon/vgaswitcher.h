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


#ifndef VGASWITCHER_H
#define VGASWITCHER_H

#include <QCoreApplication>

class VgaDbus;
class VgaSwitcherooAdaptor;

class VgaSwitcher : public QCoreApplication
{
  Q_OBJECT
public:
    VgaSwitcher(int& argc, char** argv);
    virtual ~VgaSwitcher();
    VgaDbus* getDbus();
private:
    VgaDbus *dbus;
    VgaSwitcherooAdaptor *adaptor;
    
};

#endif // VGASWITCHER_H
