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
#include "Operations/HadamardOp.h"
#include "Operations/HaarOp.h"
#include "Operations/DCTOp.h"
#include "Operations/HoughOp.h"
#include "Operations/InverseHoughOp.h"
#include "Operations/PyramidOp.h"
#include "Operations/InversePyramidOp.h"
#include "Operations/ClassAnalysisOp.h"


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
  QString lang = QString("en_US");
  if(argc > 1) {
    lang = QString(argv[1]);
  }

  QTranslator qtTranslator;
  QString tr = "qt_";
  tr += lang;
  if(!qtTranslator.load(tr, QLibraryInfo::location(QLibraryInfo::TranslationsPath))) {
      cout << "Error while loading " << tr.toStdString() << endl;
  }
  app.installTranslator(&qtTranslator);

  QTranslator giTranslator;
  if(!giTranslator.load(QString("genericinterface_") + lang.mid(0, 2))) {
      cout << "Error while loading genericinterface_en.qm" << endl;
  }
  app.installTranslator(&giTranslator);

  QTranslator eiiTranslator;
  if(!eiiTranslator.load(QString("eiimage_") + lang.mid(0, 2))) {
      cout << "Error while loading eiimage_en.qm" << endl;
  }
  app.installTranslator(&eiiTranslator);

  GenericInterface gi("eiimage", Qt::LeftDockWidgetArea);

  PluginManager* pluginManager = new PluginManager(&gi);
  EIImageService* eiimageService = new EIImageService(&gi);

//  gi.addService(eiimageService);
  gi.changeService(GenericInterface::WINDOW_SERVICE, eiimageService);
//  gi.addService(GenericInterface::WINDOW_SERVICE, eiimageService);

  QObject::connect(pluginManager, SIGNAL(addPlugin(OpSet*)), eiimageService, SLOT(addOpSet(OpSet*)));
  QObject::connect(pluginManager, SIGNAL(removePlugin(OpSet*)), eiimageService, SLOT(removeOpSet(OpSet*)));

  BuiltinOpSet* transforms = new BuiltinOpSet(qApp->translate("", "&Image").toStdString());
  transforms->addOperation(new TranslateOp());
  transforms->addOperation(new RotateOp());
  transforms->addOperation(new CenterOp());
  transforms->addOperation(new FlipOp(FlipOp::Horizontal));
  transforms->addOperation(new FlipOp(FlipOp::Vertical));
  transforms->addOperation(new SplitColorOp());
  transforms->addOperation(new CombineColorOp());
  transforms->addOperation(new ScalingOp());

  BuiltinOpSet* info = new BuiltinOpSet(qApp->translate("", "&Tools").toStdString());

  info->addOperation(new SignalToNoiseOp());
  info->addOperation(new MeanSquaredErrorOp());
  info->addOperation(new EntropyOp());
  info->addOperation(new NoiseOp());
  info->addOperation(new RandomImgOp());
  info->addOperation(new ColorimetryOp());
  info->addOperation(new PseudoColorOp());
  info->addOperation(new RejectionRingOp());
  info->addOperation(new SinusSynthesisOp());

  BuiltinOpSet* encode = new BuiltinOpSet(qApp->translate("", "&Encoding").toStdString());
  encode->addOperation(new HuffmanOp());
  encode->addOperation(new MICDEncodingOp());

  BuiltinOpSet* morpho = new BuiltinOpSet("&Morpho. math.");
  morpho->addOperation(new DMMOp());

  BuiltinOpSet* opSet = new BuiltinOpSet("&Operations");
  opSet->addOperation(new PointOp());
  opSet->addOperation(new ThresholdOp());
  opSet->addOperation(new QuantificationOp());
  opSet->addOperation(new HistogramOp());
  opSet->addOperation(new BFlitOp());
  opSet->addOperation(new CroissanceOp());
  opSet->addOperation(new FFTOp());
  opSet->addOperation(new IFFTOp());
  opSet->addOperation(new ZeroCrossingOp());
  opSet->addOperation(new HadamardOp());
  opSet->addOperation(new HaarOp());
  opSet->addOperation(new DCTOp());
  opSet->addOperation(new HoughOp());
  opSet->addOperation(new InverseHoughOp());
  opSet->addOperation(new PyramidOp());
  opSet->addOperation(new InversePyramidOp());
  opSet->addOperation(new ClassAnalysisOp());


  eiimageService->addOpSet(transforms);
  eiimageService->addOpSet(opSet);
  eiimageService->addOpSet(encode);
  eiimageService->addOpSet(morpho);
  gi.addService(new MorphoMatService);
  gi.addService(new filtrme::FilteringService);
  eiimageService->addOpSet(info);

  gi.addService(pluginManager);

  gi.run();

  return app.exec();
}
