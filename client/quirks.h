#ifndef VGAC_QUIRKS
#define VGAC_QUIRKS

#include <QString>
#include "../videoinfo.h"
#include "VgaCDbus.h"
#include <QHash>



void quirk_ati_intel_acer(QString event,QList<VideoInfo*> *data,OrgAdmiral0VgaSwitcherooInterface *iface){
  if(data->at(0)->isUsed() && !data->at(1)->isPowered()){
    iface->CardsOn();
  }
}
#endif