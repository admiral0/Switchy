/***************************************************************************
 *   Copyright (C) 2010 by admiral0 <admiral0@tuxfamily.org>               *
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

#include <plasma/svg.h>
#include <plasma/theme.h>
#include <QFormLayout>
#include <QLabel>
#include <kfiledialog.h>
#include <QLineEdit>
#include <kconfigdialog.h>
#include "ui_vgaswitcheroo.h"
#include <plasma/widgets/pushbutton.h>
#include <QGraphicsLinearLayout>
#include <QTimer>
Switchy::Switchy(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args)
{
    setBackgroundHints(DefaultBackground);
    setHasConfigurationInterface(true);
    cards=NULL;
    c1=new Plasma::PushButton(this);
    c2=new Plasma::PushButton(this);
    ui=new Ui::vgaswitcheroo;
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    tmr=new QTimer();
}


Switchy::~Switchy()
{
    if(cards)
      delete cards;
    delete c1;
    delete c2;delete ui;delete tmr;
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
  c1->setText(card1name);
  c1->setCheckable(TRUE);
  c2->setText(card2name);
  c2->setCheckable(TRUE);
  box->addItem(c1);
  box->addItem(c2);
  this->setLayout(box);
  updateApplet();
  tmr->setInterval(31000);
  connect(tmr,SIGNAL(timeout()),this,SLOT(updateApplet()));
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
  c1->setText(card1name);
  c2->setText(card2name);
  cards=getInfo();
  if(cards->at(0)->isUsed()){
    c1->setText(card1name);   
  }else{
    c1->setText(QString("(")+card1name+")");
  }
  if(cards->at(0)->isPowered())
      c1->setChecked(TRUE);
  else
      c1->setChecked(FALSE);
  if(cards->at(1)->isUsed()){
    c2->setText(card2name);   
  }else{
    c2->setText(QString("(")+card2name+")");
  }
  if(cards->at(1)->isPowered())
      c2->setChecked(TRUE);
  else
      c2->setChecked(FALSE);
}

#include "switchy.moc"
