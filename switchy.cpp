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

#include "switchy.h"

//Deps
#include <KMessageBox>
#include <KConfigDialog>
#include <QGraphicsLinearLayout>
#include <plasma/widgets/slider.h>
#include <plasma/widgets/iconwidget.h>
#include <plasma/widgets/pushbutton.h>

// DBUS stuff
#include "vgad.h"
#include "ksmserver.h"

// Uis
#include "ui_startup.h"
#include "ui_vgaswitcheroo.h"

//VGA Switcheroo utils
#include "videoinfo.h"



 
Switchy::Switchy(QObject *parent, const QVariantList &args)
    : Plasma::PopupApplet(parent, args)
{
     setPopupIcon("cpu");
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    setHasConfigurationInterface(true);
    
    ui_vga=new Ui::vgaswitcheroo;
    ui_startup=new Ui::Startup;
    vgaswitcheroo=new QWidget;
    startup=new QWidget;
    
    widget= new QGraphicsWidget;
    widget->setMinimumSize(100,90);
    QGraphicsLinearLayout *layout=new QGraphicsLinearLayout(Qt::Vertical);
    QGraphicsLinearLayout *inner=new QGraphicsLinearLayout(Qt::Horizontal);
    lbl_dis=new Plasma::IconWidget();
    lbl_dis->setIcon(KIcon("audio-card"));
    lbl_igd=new Plasma::IconWidget();
    lbl_igd->setIcon(KIcon("cpu"));
    btn_apply=new Plasma::PushButton();
    btn_apply->setText(i18n("Apply"));
    slider=new Plasma::Slider();
    slider->setOrientation(Qt::Horizontal);
    slider->setMaximum(2);
    inner->addItem(lbl_igd);
    inner->addItem(slider);
    inner->addItem(lbl_dis);
    inner->setAlignment(slider,Qt::AlignVCenter);
    layout->addItem(inner);
    layout->addItem(btn_apply);
    widget->setLayout(layout);
    connect(btn_apply,SIGNAL(clicked()),this,SLOT(apply()));
    
    block=true;
    pending=-1;
    cards=NULL;
    
    tmr=new QTimer;
    dbus=new OrgAdmiral0VgaSwitcherooInterface("org.admiral0.VgaSwitcheroo","/org/admiral0/VgaSwitcheroo", QDBusConnection::systemBus());
    kde=new OrgKdeKSMServerInterfaceInterface("org.kde.KSMServerInterface","/KSMServer",QDBusConnection::sessionBus());
    
    clientSettings=new QSettings("org.admiral0","vgad");
    
    vgapath=clientSettings->value("vgapath",VGA_SWITCHEROO).toString();
    if(vgapath=="default"){
      vgapath=VGA_SWITCHEROO;
    }
}

Switchy::~Switchy()
{
  if(cards)
    delete cards;
  delete ui_startup; delete ui_vga;
  //delete startup; delete vgaswitcheroo; delete appearance;
  delete dbus; delete kde;
  //delete widget;delete slider;delete lbl_dis;delete lbl_igd;
  delete clientSettings;
}

void Switchy::createConfigurationInterface(KConfigDialog* parent)
{

  ui_vga->setupUi(vgaswitcheroo);
  ui_startup->setupUi(startup);
  parent->addPage(vgaswitcheroo,i18n("Vga Switcheroo"));
  parent->addPage(startup,i18n("Startup"));
  connect(parent,SIGNAL(applyClicked()),this,SLOT(confAccepted()));
  connect(parent,SIGNAL(okClicked()),this,SLOT(confAccepted()));
}

void Switchy::confAccepted()
{
    KConfigGroup cfg = config();
  if(ui_vga->vgaDefaultPath->isChecked()){
    cfg.writeEntry("vgapath","default");
    vgapath=VGA_SWITCHEROO;
  }else{
    cfg.writeEntry("vgapath",ui_vga->vgaPath->text());
    vgapath=ui_vga->vgaPath->text();
  }
  
  cfg.writeEntry("atiIntelQuirk",ui_vga->ati_intel_quirk->isChecked());
  
  if(ui_startup->startupALL->isChecked()){
    clientSettings->setValue("startup","ALL");
  }else if(ui_startup->startupDIS->isChecked()){
    clientSettings->setValue("startup","DIS");
  }else{
    clientSettings->setValue("startup","IGD");
  }
  updateApplet();
}

void Switchy::updateApplet()
{
  if(cards)
    delete cards;
  cards=VideoInfo::getInfo(vgapath);
  if(cards->size()!=2){
    return;
  }
  VideoInfo *first=cards->first();
  VideoInfo *last=cards->last();
  if(first->getType()==QString("IGD")){
    //first IGD last DIS
    igd=first;
    dis=last;
  }else{
    igd=last;
    dis=first;
  }
  if(igd->isUsed()){
    if(dis->isPowered()){
      slider->setValue(1);
    }else{
      slider->setValue(0);
    }
  }else{
    if(igd->isPowered()){
      slider->setValue(1);
    }else{
      slider->setValue(2);
    }
  }
  
  
}




void Switchy::init()
{
  updateApplet();
  tmr->setInterval(31000);
  connect(tmr,SIGNAL(timeout()),this,SLOT(updateApplet()));
  tmr->start();
}

QGraphicsWidget* Switchy::graphicsWidget()
{
    
    return widget;
}
void Switchy::powerBoth()
{
  if(igd->isPowered() && dis->isPowered()){
    return;
  }else{
    dbus->CardsOn();
  }
}

void Switchy::powerDIS()
{
  if(dis->isUsed()){
    if(igd->isPowered()){
      dbus->CardsOff();
    }else{
      return;
    }
  }else{
    dbus->Discrete();
    int res=KMessageBox::questionYesNo(0,i18n("Would you like to logout to switch to discrete GPU?"),"Switchy",KStandardGuiItem::yes(),KStandardGuiItem::no(),QString("switchy"));
    if(res==KMessageBox::Yes){
      kde->logout(0,0,0);
    }
  }
}

void Switchy::powerIGD()
{
  qDebug()<<"powerIGD"<<igd->isUsed()<<igd->isPowered()<<"DIS:"<<dis->isUsed()<<dis->isPowered();
  if(igd->isUsed()){
    if(dis->isPowered()){
      dbus->CardsOff();
    }else{
      return;
    }
  }else{
    dbus->Integrated();
    int res=KMessageBox::questionYesNo(0,i18n("Would you like to logout to switch to integrated GPU?"),"Switchy",KStandardGuiItem::yes(),KStandardGuiItem::no(),QString("switchy"));
    if(res==KMessageBox::Yes){
      kde->logout(0,0,0);
    }
  }
}
void Switchy::apply()
{
  int val=slider->value();
  updateApplet();
  if(val==0){
    powerIGD();
  }else if(val==1){
    powerBoth();
  }else{
    powerDIS();
  }
  QTimer::singleShot(2000,this,SLOT(updateApplet()));
}



#include "switchy.moc"
