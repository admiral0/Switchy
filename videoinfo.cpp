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
#include <qfile.h>
#include <QDebug>
#include <QStringList>

VideoInfo::VideoInfo(){
  extra=new QString;
  type=new QString;
}

VideoInfo::~VideoInfo(){
  delete extra;
  delete type;
}

VideoInfo& VideoInfo::operator=(const VideoInfo& other)
{
    this->id=other.id;
    this->extra=other.extra;
    this->used=other.used;
    this->power=other.power;
    this->type=other.type;
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
void VideoInfo::setExtra(const QString* val)
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
QString* VideoInfo::getType()
{
  return type;
}
void VideoInfo::setType(const QString* type)
{
  this->type->clear();
  this->type->append(type);
}




QList< VideoInfo*>* VideoInfo::getInfo(QString path)
{
  QList<VideoInfo*> *data=new QList<VideoInfo*>;
  QFile *f=new QFile(path);
  f->open(QIODevice::ReadOnly);
  QString out=f->readAll();
//   qDebug()<<out;
  f->close();
  QStringList cards=out.split("\n",QString::SkipEmptyParts);
  foreach(QString card,cards){
    QStringList l= card.split(":",QString::SkipEmptyParts);
//     qDebug()<<"Aia"<<l;
    VideoInfo *i=new VideoInfo();
    i->setId(l.at(0).toInt());
    i->setType(&(l.at(1)));
    if(l.at(2)=="+")
      i->setUsed(TRUE);
    else
      i->setUsed(FALSE);
    if(l.at(3)=="Pwr")
      i->setPowered(TRUE);
    else
      i->setPowered(FALSE);
    QString e(l[4]+l[5]+l[6]);
    i->setExtra(&e);
//     qDebug()<<"Data to append:"<<i->getId()<<i->isUsed()<<i->isPowered()<<i->getExtra();
    data->append(i);
  }
  delete f;
   qDebug()<<data->size();
   qDebug()<<data->at(0)->getId()<<data->at(0)->isPowered();
    qDebug()<<data->at(1)->getId()<<data->at(1)->isPowered();
  return data;
}

