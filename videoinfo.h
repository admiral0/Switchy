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


#ifndef VIDEOINFO_H
#define VIDEOINFO_H

class QString;
class VideoInfo
{

public:
    VideoInfo();
    virtual ~VideoInfo();
    virtual VideoInfo& operator=(const VideoInfo& other);
    virtual bool operator==(const VideoInfo& other) const;
    int getId();
    bool isUsed();
    bool isPowered();
    QString* getExtra();
    void setId(int val);
    void setUsed(bool val);
    void setPowered(bool val);
    void setExtra(QString *val);
private:
  int id;
  bool used;
  bool power;
  //PCI bus etc.
  QString *extra;
  
};

#endif // VIDEOINFO_H
