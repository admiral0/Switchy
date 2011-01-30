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

QList< VideoInfo*>* VideoInfo::getInfo(QString path)
{
  QList<VideoInfo*> *data=new QList<VideoInfo*>;
  QFile *f=new QFile(path);
  f->open(QIODevice::ReadOnly);
  QString out=f->readAll();
  qDebug()<<out;
  f->close();
  QStringList cards=out.split("\n",QString::SkipEmptyParts);
  qDebug()<<cards;
  foreach(QString card,cards){
    qDebug()<<"Card:"<<card;
    QStringList l= card.split(":",QString::SkipEmptyParts);
    qDebug()<<"Parts:"<<l;
    VideoInfo *i=new VideoInfo();
    i->setId(l.at(0).toInt());
    if(l.at(1)=="+")
      i->setUsed(TRUE);
    else
      i->setUsed(FALSE);
    if(l.at(2)=="Pwr")
      i->setPowered(TRUE);
    else
      i->setPowered(FALSE);
    QString e(l[3]+l[4]+l[5]);
    i->setExtra(&e);
    qDebug()<<"Data to append:"<<i->getId()<<i->isUsed()<<i->isPowered()<<i->getExtra();
    data->append(i);
  }
  delete f;
  qDebug()<<data->size();
  qDebug()<<data->at(0)->getId()<<data->at(0)->isPowered();
  qDebug()<<data->at(1)->getId()<<data->at(1)->isPowered();
  return data;
}

