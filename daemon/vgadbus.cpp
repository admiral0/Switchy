/*
    <one line to give the program's name and a brief idea of what it does.>
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


#include "vgadbus.h"
#include <qfile.h>
#include <qfileinfo.h>


VgaDbus::VgaDbus() : QObject()
{
  devpath="/proc/sys/debug/vgaswitcheroo/switch";
}

VgaDbus::~VgaDbus()
{

}
void VgaDbus::CardsOff()
{
  QFile f(devpath);
  f.open(QIODevice::WriteOnly);
  f.write("OFF");
  f.close();
}
void VgaDbus::CardsOn()
{
  QFile f(devpath);
  f.open(QIODevice::WriteOnly);
  f.write("ON");
  f.close();
}
void VgaDbus::Discrete()
{
  QFile f(devpath);
  f.open(QIODevice::WriteOnly);
  f.write("DDIS");
  f.close();
}
void VgaDbus::Integrated()
{
  QFile f(devpath);
  f.open(QIODevice::WriteOnly);
  f.write("DIGD");
  f.close();
}
QString VgaDbus::GetVgaSwitcherooPath()
{
  return devpath;
}
void VgaDbus::SetVgaSwitcherooPath(QString path)
{
  //Some nab checks...
  QString realPath;
  QFileInfo f(path);
  realPath=f.canonicalFilePath();
  if(!f.exists())
    return;
  if(f.isSymLink())
    realPath=f.symLinkTarget();
  if(!realPath.startsWith("/dev") && !realPath.startsWith("/proc") && !realPath.startsWith("/sys"))
    return;
  devpath=realPath;
}


#include "vgadbus.moc"