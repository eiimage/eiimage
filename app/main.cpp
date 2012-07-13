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
#include <QLibraryInfo>

#include "BuiltinOpSet.h"

#include "Services/PluginManager.h"
#include "Services/EIImageService.h"

#include "Operations/PointOp.h"
#include "Operations/ThresholdOp.h"
#include "Operations/TranslateOp.h"
#include "Operations/RotateOp.h"
#include "Operations/FlipOp.h"
#include "Operations/CenterOp.h"
#include "Operations/SplitColorOp.h"
#include "Operations/CombineColorOp.h"
#include "Operations/SignalToNoiseOp.h"
#include "Operations/MeanSquaredErrorOp.h"
#include "Operations/FFTOp.h"

using namespace genericinterface;
using namespace std;

int main(int argc, char** argv)
{
  QApplication app(argc, argv);
  app.setOrganizationName("insa");
  app.setApplicationName("eiimage");


  Log::configure(true, false, 0);

  QTranslator qtTranslator;
  QString tr = "qt_fr_FR";
  qtTranslator.load(tr, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
  cout << tr.toStdString();
  app.installTranslator(&qtTranslator);

  QTranslator giTranslator;
  giTranslator.load("genericinterface_fr");
  app.installTranslator(&giTranslator);

  GenericInterface gi("ImageEII", Qt::LeftDockWidgetArea);

  PluginManager* pluginManager = new PluginManager(&gi);
  EIImageService* eiimageService = new EIImageService(&gi);

  gi.addService(pluginManager);
//  gi.addService(eiimageService);
  gi.changeService(GenericInterface::WINDOW_SERVICE, eiimageService);
//  gi.addService(GenericInterface::WINDOW_SERVICE, eiimageService);

  QObject::connect(pluginManager, SIGNAL(addPlugin(OpSet*)), eiimageService, SLOT(addOpSet(OpSet*)));
  QObject::connect(pluginManager, SIGNAL(removePlugin(OpSet*)), eiimageService, SLOT(removeOpSet(OpSet*)));

  gi.run();

  return app.exec();
}
