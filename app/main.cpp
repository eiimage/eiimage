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

#include "Operations/TestOp.h"
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
#include "Operations/IFFTOp.h"
#include "Operations/RandomImgOp.h"
#include "Operations/NoiseOp.h"
#include "Operations/BFlitOp.h"
#include "Operations/DMMOp.h"
#include "Operations/PseudoColorOp.h"
#include "Operations/CroissanceOp.h"
#include "Operations/ZeroCrossingOp.h"
#include "Operations/HistogramOp.h"
#include "Operations/ColorimetryOp.h"
#include "Operations/SinusSynthesisOp.h"
#include "Operations/ScalingOp.h"
#include "Operations/QuantificationOp.h"
#include "Operations/EntropyOp.h"
#include "Operations/HuffmanOp.h"
#include "Operations/RejectionRingOp.h"
#include "Operations/MICDEncodingOp.h"


#include "Services/MorphoMatService.h"
#include "Services/FilteringService.h"

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

  QTranslator eiiTranslator;
  eiiTranslator.load("eiimage_fr");
  app.installTranslator(&eiiTranslator);

  GenericInterface gi("eiimage", Qt::LeftDockWidgetArea);

  PluginManager* pluginManager = new PluginManager(&gi);
  EIImageService* eiimageService = new EIImageService(&gi);

  gi.addService(pluginManager);
//  gi.addService(eiimageService);
  gi.changeService(GenericInterface::WINDOW_SERVICE, eiimageService);
//  gi.addService(GenericInterface::WINDOW_SERVICE, eiimageService);

  QObject::connect(pluginManager, SIGNAL(addPlugin(OpSet*)), eiimageService, SLOT(addOpSet(OpSet*)));
  QObject::connect(pluginManager, SIGNAL(removePlugin(OpSet*)), eiimageService, SLOT(removeOpSet(OpSet*)));


  BuiltinOpSet* opSet = new BuiltinOpSet("Operations");
  opSet->addOperation(new QuantificationOp());
  opSet->addOperation(new PointOp());
  opSet->addOperation(new ThresholdOp());
  opSet->addOperation(new TranslateOp());
  opSet->addOperation(new RotateOp());
  opSet->addOperation(new FlipOp(FlipOp::Horizontal));
  opSet->addOperation(new FlipOp(FlipOp::Vertical));
  opSet->addOperation(new CenterOp());
  opSet->addOperation(new SplitColorOp());
  opSet->addOperation(new CombineColorOp());
  opSet->addOperation(new SignalToNoiseOp());
  opSet->addOperation(new MeanSquaredErrorOp());
  opSet->addOperation(new FFTOp());
  opSet->addOperation(new IFFTOp());
  opSet->addOperation(new RandomImgOp());
  opSet->addOperation(new NoiseOp());
  opSet->addOperation(new BFlitOp());
  opSet->addOperation(new DMMOp());
  opSet->addOperation(new TestOp());
  opSet->addOperation(new PseudoColorOp());
  opSet->addOperation(new CroissanceOp());
  opSet->addOperation(new ZeroCrossingOp());
  opSet->addOperation(new HistogramOp());
  opSet->addOperation(new ColorimetryOp());
  opSet->addOperation(new SinusSynthesisOp());
  opSet->addOperation(new ScalingOp());
  opSet->addOperation(new EntropyOp());
  opSet->addOperation(new HuffmanOp());
  opSet->addOperation(new RejectionRingOp());
  opSet->addOperation(new MICDEncodingOp());

  eiimageService->addOpSet(opSet);

  gi.addService(new MorphoMatService);
  gi.addService(new filtrme::FilteringService);

  gi.run();

  return app.exec();
}
