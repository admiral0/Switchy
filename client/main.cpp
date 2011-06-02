/*
    GPU switching client
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
#include <QCoreApplication>
#include <QString>
#include <QStringList>
#include <QSettings>
#include <iostream>
#include "VgaCDbus.h"
#include "quirks.h"
using namespace std;
void printHelp();
int main(int argc, char **argv){
  QCoreApplication app(argc,argv);
  if(app.arguments().size()==1){
    std::cout << "error: no params\n";
    printHelp();
    return 0;
    
  }else{
    QSettings settings("org.admiral0","vgad");
    QString action=app.arguments().at(1);
    OrgAdmiral0VgaSwitcherooInterface vgad("org.admiral0.VgaSwitcheroo","/org/admiral0/VgaSwitcheroo",QDBusConnection::systemBus());
    if(action=="--auto"){
      if(app.arguments().size()<3){
	cout << "error: --auto expects option\n";
	printHelp();
      }
      quirk_ati_intel_acer(app.arguments().at(2),VideoInfo::getInfo("/proc/sys/debug/vgaswitcheroo/switch"),&vgad);
      
    }else if(action=="--igd"){
      vgad.Integrated();
    }else if(action=="--dis"){
      vgad.Discrete();
    }else if(action=="--all"){
      vgad.CardsOn();
    }else if(action=="--off"){
      vgad.CardsOff();
    }else if(action=="--help"){
      printHelp();
    }else{
      cout << "error: option " << action.toAscii().data() << " not recognized\n";
      printHelp();
    }
  }
  return 0;
}

void printHelp()
{
  cout << qApp->argv()[0]<< "  --auto option | --igd | --dis | --all \n";
  cout << "\t--auto [option]\t" << "option can be one of startup,shutdown,logout,login.\n" << "\t\t\tUses settings and quirks to turn on/off GPU.\n";
  cout << "\t--igd\t" << "Enable integrated card.\n";
  cout << "\t--dis\t" << "Enable discrete card.\n";
  cout << "\t--all\t" << "Enable both cards. Depends on your system which one will be used (usually integrated).\n";
  cout << "\t--off\t" << "Disable unused card if possible\n";
  cout << "\t--help\t" << "Get this information.\n";
}
