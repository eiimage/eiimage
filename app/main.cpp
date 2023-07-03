/*
 * Copyright 2011-2012 INSA Rennes
 * 
 * This file is part of ImageINSA.
 * 
 * ImageINSA is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ImageINSA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with ImageINSA.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QApplication>
#include <QTranslator>

#include <GenericInterface.h>

#include <iostream>
#include <QLibraryInfo>

#include "BuiltinOpSet.h"

#include "Services/PluginManager.h"
#include "Services/ImageINSAService.h"

#include "Operations/AbsoluteConvertOp.h"
#include "Operations/DoubleConvertOp.h"
#include "Operations/PointOp.h"
#include "Operations/ThresholdOp.h"
#include "Operations/TranslateOp.h"
#include "Operations/RotateOp.h"
#include "Operations/FlipOp.h"
#include "Operations/CenterOp.h"
#include "Operations/SplitColorOp.h"
#include "Operations/SplitHsvOp.h"
#include "Operations/CombineColorOp.h"
#include "Operations/CombineHsvOp.h"
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
#include "Operations/DoubleEntropyOp.h"
#include "Operations/EntropyOp.h"
#include "Operations/HuffmanOp.h"
#include "Operations/RejectionRingOp.h"
#include "Operations/DPCMEncodingOp.h"
#include "Operations/HadamardOp.h"
#include "Operations/DCTOp.h"
#include "Operations/HoughOp.h"
#include "Operations/InverseHoughOp.h"
#include "Operations/PyramidOp.h"
#include "Operations/InversePyramidOp.h"
#include "Operations/ClassAnalysisOp.h"
#include "Operations/ClassResultOp.h"
#include "Operations/SeparatorOp.h"
#include "Operations/MedianOp.h"
#include "Operations/UCharConvertOp.h"


#include "Services/MorphoMatService.h"
#include "Services/FilteringService.h"

using namespace genericinterface;
using namespace std;

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    app.setOrganizationName("INSA");
    app.setApplicationName("ImageINSA");
    //QMainWindow mainWindow;
    //mainWindow.setWindowIcon(QIcon("/home/trauffle/Documents/Work/GitLabINSA/imageinsa_tanguy/app/img/Logo_INSA.ico"));

    Log::configure(true, false, 0);

    QSettings settings;
    QString lang = settings.value(QSETTINGS_LANGUAGE_PREFERENCE,
                                  QLocale::system().name().split('_').first()).toString();
    if(argc > 1) {
        lang = QString(argv[1]);
    }

    QTranslator qtTranslator;
    QString tr = "qt_" + lang;
    if(!qtTranslator.load(tr) &&
       !qtTranslator.load(tr, QLibraryInfo::location(QLibraryInfo::TranslationsPath))) {
        cout << "Error while loading " << tr.toStdString() << endl;
    }
    app.installTranslator(&qtTranslator);


    QTranslator giTranslator;
    if(!giTranslator.load(QString("genericinterface_") + lang, "lang")) {
        cout << "Error while loading genericinterface_en.qm" << endl;
    }
    app.installTranslator(&giTranslator);

    QTranslator mainTranslator;
    if(!mainTranslator.load(QString("imageinsa_") + lang, "lang")) {
        cout << "Error while loading imageinsa_en.qm" << endl;
    }
    app.installTranslator(&mainTranslator);

    GenericInterface gi("ImageINSA", Qt::LeftDockWidgetArea);

    PluginManager* pluginManager = new PluginManager(&gi);
    ImageINSAService* mainService = new ImageINSAService(&gi);

    gi.changeService(GenericInterface::WINDOW_SERVICE, mainService);

    QObject::connect(pluginManager, SIGNAL(addPlugin(OpSet*)), mainService, SLOT(addOpSet(OpSet*)));
    QObject::connect(pluginManager, SIGNAL(removePlugin(OpSet*)), mainService, SLOT(removeOpSet(OpSet*)));

    BuiltinOpSet* image = new BuiltinOpSet(qApp->translate("", "&Image").toStdString());

    image->addOperation(new TranslateOp());
    image->addOperation(new RotateOp());
    image->addOperation(new CenterOp());
    image->addOperation(new FlipOp(FlipOp::Horizontal));
    image->addOperation(new FlipOp(FlipOp::Vertical));
    image->addOperation(new SeparatorOp());
    //Mask and crops here
    image->addOperation(new SeparatorOp());
    image->addOperation(new AbsoluteConvertOp());
    image->addOperation(new UCharConvertOp());
    image->addOperation(new DoubleConvertOp());
    image->addOperation(new SeparatorOp());
    image->addOperation(new ScalingOp());
    image->addOperation(new QuantificationOp());
    image->addOperation(new ThresholdOp());
    image->addOperation(new SeparatorOp());
    image->addOperation(new HistogramOp());
    image->addOperation(new PointOp());
    image->addOperation(new SeparatorOp());
    image->addOperation(new NoiseOp());

    //à mettre dans oclors
  /*
    */
    BuiltinOpSet* tools = new BuiltinOpSet(qApp->translate("", "&Tools").toStdString());

    tools->addOperation(new RandomImgOp());
    tools->addOperation(new ColorimetryOp());
    tools->addOperation(new RejectionRingOp());
    tools->addOperation(new SinusSynthesisOp());
    tools->addOperation(new SeparatorOp());
    tools->addOperation(new SignalToNoiseOp());
    tools->addOperation(new MeanSquaredErrorOp());
    tools->addOperation(new SeparatorOp());
    tools->addOperation(new EntropyOp());
    tools->addOperation(new DoubleEntropyOp());



    BuiltinOpSet* encode = new BuiltinOpSet(qApp->translate("", "&Coding").toStdString());

    encode->addOperation(new HuffmanOp());
    encode->addOperation(new DPCMEncodingOp());
    encode->addOperation(new SeparatorOp());
    encode->addOperation(new HadamardOp());
    encode->addOperation(new DCTOp());

    BuiltinOpSet* morpho = new BuiltinOpSet("&Morpho. math.");
    morpho->addOperation(new DMMOp());
    morpho->addOperation(new SeparatorOp());


    BuiltinOpSet* analyse = new BuiltinOpSet(qApp->translate("", "Analysis").toStdString());

    analyse->addOperation(new FFTOp());
    analyse->addOperation(new IFFTOp());
    analyse->addOperation(new SeparatorOp());
    analyse->addOperation(new HoughOp());
    analyse->addOperation(new InverseHoughOp());
    analyse->addOperation(new SeparatorOp());
    analyse->addOperation(new PyramidOp());
    analyse->addOperation(new InversePyramidOp());
    analyse->addOperation(new SeparatorOp());
    analyse->addOperation(new CroissanceOp());
    analyse->addOperation(new SeparatorOp());
    analyse->addOperation(new ZeroCrossingOp());
    analyse->addOperation(new SeparatorOp());
    analyse->addOperation(new ClassAnalysisOp());
    analyse->addOperation(new ClassResultOp());

    BuiltinOpSet* colors = new BuiltinOpSet(qApp->translate("", "Colors").toStdString());
    colors->addOperation(new SeparatorOp());
    colors->addOperation(new SplitColorOp());
    colors->addOperation(new SplitHsvOp());
    colors->addOperation(new CombineColorOp());
    colors->addOperation(new CombineHSVOp());
    colors->addOperation(new SeparatorOp());
    colors->addOperation(new PseudoColorOp());

    BuiltinOpSet* filter = new BuiltinOpSet(qApp->translate("", "Filtering").toStdString());
    filter->addOperation(new BFlitOp());
    filter->addOperation(new MedianOp());



    mainService->addOpSet(image);
    mainService->addOpSet(colors);
    mainService->addOpSet(morpho);
    gi.addService(new filtrme::FilteringService);
    mainService->addOpSet(filter);
    mainService->addOpSet(analyse);
    mainService->addOpSet(encode);
    gi.addService(new MorphoMatService);

    mainService->addOpSet(tools);

    gi.setWindowIcon(QIcon("../../app/img/Logo_INSA.ico"));
    gi.addService(pluginManager);
    gi.run();

    return app.exec();
}
