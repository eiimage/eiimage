/*
 * Copyright 2011-2012 INSA Rennes
 * 
 * This file is part of EIImage.
 * 
 * EIImage is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * EIImage is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with EIImage.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QApplication>
#include <QTranslator>

#include <GenericInterface.h>

#include <iostream>

#include "Services/PluginManager.h"
#include "Services/EIImageService.h"

using namespace genericinterface;
using namespace std;

int main(int argc, char** argv)
{
  qDebug("HW!");
  QApplication app(argc, argv);
  app.setOrganizationName("insa");
  app.setApplicationName("eiimage");


  Log::configure(true, false, 0);

  QTranslator giTranslator;
  giTranslator.load("genericinterface_fr");
  app.installTranslator(&giTranslator);

  GenericInterface gi("ImageEII", Qt::LeftDockWidgetArea);

  PluginManager* pluginManager = new PluginManager(&gi);
  EIImageService* eiimageService = new EIImageService(&gi);

  gi.addService(pluginManager);
  gi.addService(eiimageService);

  QObject::connect(pluginManager, SIGNAL(addPlugin(OpSet*)), eiimageService, SLOT(addOpSet(OpSet*)));
  QObject::connect(pluginManager, SIGNAL(removePlugin(OpSet*)), eiimageService, SLOT(removeOpSet(OpSet*)));

  gi.run();

  return app.exec();
}
