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
#include <Plasma/PopupApplet>
#include <Plasma/Svg>
#include "videoinfo.h"


#define VGA_SWITCHEROO "/proc/sys/debug/vgaswitcheroo/switch"

class QSettings;
class OrgKdeKSMServerInterfaceInterface;
class QDBusConnection;
class QDBusConnection;
class QTimer;
namespace Ui {
class vgaswitcheroo;
class Startup;
class Appearance;
}

class QWidget;
namespace Plasma {
class PushButton;
class Slider;
class Label;
class IconWidget;
}
class QSizeF;
class OrgAdmiral0VgaSwitcherooInterface;
class Switchy : public Plasma::PopupApplet
{
    Q_OBJECT
    public:
        Switchy(QObject *parent, const QVariantList &args);
        ~Switchy();

        void init();
	void createConfigurationInterface ( KConfigDialog* parent );
	virtual QGraphicsWidget* graphicsWidget();
private slots:
    void confAccepted();
    void updateApplet();
    void apply();
    void powerIGD();
    void powerBoth();
    void powerDIS();
    
//     void unusedOff();

private:
  
	QList<VideoInfo*> *cards;
	VideoInfo* igd;
	VideoInfo* dis;
	QString name_igd;
	QString name_dis;
	QString vgapath;
	QString pending;
	
	QTimer *tmr;
	//First Tab
	QWidget *vgaswitcheroo;
	Ui::vgaswitcheroo *ui_vga;
	//Second Tab
	QWidget *startup;
	Ui::Startup *ui_startup;
	
	OrgAdmiral0VgaSwitcherooInterface *dbus;
	OrgKdeKSMServerInterfaceInterface *kde;
	//External settings
	QSettings *clientSettings;
	
	//Widget
	QGraphicsWidget *widget;
	Plasma::Slider *slider;
	Plasma::PushButton *btn_apply;
	Plasma::IconWidget *lbl_igd;
	Plasma::IconWidget *lbl_dis;
	
	
	bool block;
};

K_EXPORT_PLASMA_APPLET(switchy, Switchy)
#endif
