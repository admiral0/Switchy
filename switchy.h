/***************************************************************************
 *   Copyright (C) 2010-2011 by admiral0 <admiral0@tuxfamily.org>               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#ifndef SWITCHY_HEADER
#define SWITCHY_HEADER

#include <KIcon>
#include <Plasma/Applet>
#include <Plasma/Svg>
#include "videoinfo.h"
#include <qdbusconnection.h>


#define VGA_SWITCHEROO "/proc/sys/debug/vgaswitcheroo/switch"

class QDBusConnection;
class QDBusConnection;
class QTimer;
namespace Ui {
class vgaswitcheroo;
}

class QWidget;
namespace Plasma {
class PushButton;
class ComboBox;
class Label;
class IconWidget;
}
class QSizeF;
class OrgAdmiral0VgaSwitcherooInterface;
class Switchy : public Plasma::Applet
{
    Q_OBJECT
    public:
        Switchy(QObject *parent, const QVariantList &args);
        ~Switchy();

        void init();
	void createConfigurationInterface ( KConfigDialog* parent );
private slots:
    void confAccepted();
    void updateApplet();
    void statusChange(int index);
    void unusedOff();

private:
	QList<VideoInfo*>* getInfo(); 
	QList<VideoInfo*> *cards;
	QString card1name;
	QString card2name;
	QString vgapath;
	Plasma::ComboBox *status;
	Plasma::IconWidget *both;
	QWidget *vgaswitcheroo;
	Ui::vgaswitcheroo *ui;
	QTimer *tmr;
	OrgAdmiral0VgaSwitcherooInterface *dbus;
	QDBusConnection *conn;
	

};

K_EXPORT_PLASMA_APPLET(switchy, Switchy)
#endif
