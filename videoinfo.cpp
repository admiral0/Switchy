/*
    Vga Switcher plasmoid
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


#include "videoinfo.h"
#include <QString>

VideoInfo::VideoInfo(){
  extra=new QString;
}

VideoInfo::~VideoInfo(){
  delete extra;
}

VideoInfo& VideoInfo::operator=(const VideoInfo& other)
{
    this->id=other.id;
    this->extra=other.extra;
    this->used=other.used;
    this->power=other.power;
    return *this;
}

bool VideoInfo::operator==(const VideoInfo& other) const
{
  if(other.id == this->id)
    return TRUE;
  return FALSE;
}
QString* VideoInfo::getExtra()
{
  return extra;
}
int VideoInfo::getId()
{
  return id;
}
bool VideoInfo::isPowered()
{
  return power;
}
bool VideoInfo::isUsed()
{
  return used;
}
void VideoInfo::setExtra(QString* val)
{
  extra->clear();
  extra->append(val);
}
void VideoInfo::setId(int val)
{
  id=val;
}
void VideoInfo::setPowered(bool val)
{
  power=val;
}
void VideoInfo::setUsed(bool val)
{
  used=val;
}



