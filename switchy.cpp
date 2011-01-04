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
#include <QPainter>
#include <QFontMetrics>
#include <QSizeF>
#include <KLocale>
#include <QDebug>
#include <plasma/svg.h>
#include <plasma/theme.h>
#include <QFormLayout>
#include <QLabel>
#include <kfiledialog.h>
#include <QLineEdit>
#include <kconfigdialog.h>
#include "ui_vgaswitcheroo.h"
#include <plasma/widgets/combobox.h>
#include <plasma/widgets/iconwidget.h>
#include <QGraphicsLinearLayout>
#include <QTimer>
#include "vgad.h"
#include "ksmserver.h"
#include <KMessageBox>
Switchy::Switchy(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args)
{
    setBackgroundHints(DefaultBackground);
    setHasConfigurationInterface(true);
    cards=NULL;
    status=new Plasma::ComboBox(this);
    both=new Plasma::IconWidget(this);
    ui=new Ui::vgaswitcheroo;
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    tmr=new QTimer();
    dbus=new OrgAdmiral0VgaSwitcherooInterface("org.admiral0.VgaSwitcheroo","/org/admiral0/VgaSwitcheroo", QDBusConnection::systemBus());
    kde=new OrgKdeKSMServerInterfaceInterface("org.kde.KSMServerInterface","/KSMServer",QDBusConnection::sessionBus());
}


Switchy::~Switchy()
{
    if(cards)
      delete cards;
    delete status;
    delete ui;delete tmr;
}

void Switchy::init()
{
  KConfigGroup cfg = config();
  card1name=cfg.readEntry("card1name","Integrated");
  card2name=cfg.readEntry("card2name","Discrete");
  vgapath=cfg.readEntry("vgapath","default");
  if(vgapath=="default")
     vgapath=VGA_SWITCHEROO;
  cards=getInfo();
  if(!cards || cards->size()==0){
    qDebug()<< "No hardware found!!!";
    setFailedToLaunch(TRUE,i18n("Vga switcheroo not found!"));
  }
  
  
  //Drawing widget
  QGraphicsLinearLayout *box=new QGraphicsLinearLayout(Qt::Horizontal,this);
  //status.addItem(i18n("Both"));
  status->addItem(card1name);
  status->addItem(card2name);
  both->setIcon(QIcon::fromTheme("dialog-warning"));
  both->setMaximumWidth(18);
  both->setToolTip(i18n("Both graphics cards are powered.\nClick on the icon to power off the unused one"));
  box->addItem(status);
  box->addItem(both);
  this->setLayout(box);
  updateApplet();
  tmr->setInterval(3000);
  connect(tmr,SIGNAL(timeout()),this,SLOT(updateApplet()));
  connect(both,SIGNAL(clicked()),this,SLOT(unusedOff()));
  connect(status,SIGNAL(currentIndexChanged(int)),this,SLOT(statusChange(int)));
  tmr->start();
}

void Switchy::createConfigurationInterface(KConfigDialog* parent)
{
  vgaswitcheroo=new QWidget;
  ui->setupUi(vgaswitcheroo);
  ui->name1->setText(card1name);
  ui->name2->setText(card2name);
  parent->addPage(vgaswitcheroo,i18n("Vga Switcheroo"));
  connect(parent,SIGNAL(applyClicked()),this,SLOT(confAccepted()));
  connect(parent,SIGNAL(okClicked()),this,SLOT(confAccepted()));
}

void Switchy::confAccepted()
{
  KConfigGroup cfg = config();
  card1name=ui->name1->text();
  cfg.writeEntry("card1name",ui->name1->text());
  card2name=ui->name2->text();
  cfg.writeEntry("card2name",ui->name2->text());
  if(ui->vgaDefaultPath->isChecked()){
    cfg.writeEntry("vgapath","default");
    vgapath=VGA_SWITCHEROO;
  }else{
    cfg.writeEntry("vgapath",ui->vgaPath->text());
    vgapath=ui->vgaPath->text();
  }
  updateApplet();
}
QList< VideoInfo*>* Switchy::getInfo()
{
  QList<VideoInfo*> *data=new QList<VideoInfo*>;
  QFile *f=new QFile(vgapath);
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
void Switchy::updateApplet()
{
  cards=getInfo();
  qDebug()<<cards->size()<<"So sad";
  status->clear();
  status->addItem(card1name);
  status->addItem(card2name);
  cards=getInfo();
  if(cards->at(1)->isUsed()){
    status->setCurrentIndex(1);
  }else{
    status->setCurrentIndex(0);
  }
  if(cards->at(0)->isPowered() && cards->at(1)->isPowered()){
    both->setVisible(TRUE);
  }else{
    both->setVisible(FALSE);
  }
}
void Switchy::statusChange(int index)
{
  if(index==-1)
    return;
  if(!cards->at(index)->isUsed()){
    if(index==0)
      dbus->Integrated();
    else
      dbus->Discrete();
    int res=KMessageBox::questionYesNo(0,i18n("Would you like to logout to switch GPU?"),"Switchy",KStandardGuiItem::yes(),KStandardGuiItem::no(),QString("switchy"));
    if(res==KMessageBox::Yes){
      kde->logout();
    }
  }
  
}
void Switchy::unusedOff()
{
  if(cards->at(0)->isPowered() && cards->at(1)->isPowered()){
    dbus->CardsOff();
    updateApplet();
  }
}

#include "switchy.moc"
