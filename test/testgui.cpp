#include <QtWidgets>
#include <QtTest/QtTest>
#include <QString>
#include <GenericInterface.h>

#include "../lib/detiq-t/GenericInterface/Widgets/ImageWidgets/StandardImageWindow.h"

#include "../core/BuiltinOpSet.h"
#include "../core/Operation.h"

#include "../app/Services/MorphoMatService.h"
#include "../app/Services/FilteringService.h"
#include "../app/Services/PluginManager.h"
#include "../app/Services/ImageINSAService.h"

#include "../app/Operations/PointOp.h"
#include "../app/Operations/ThresholdOp.h"
#include "../app/Operations/TranslateOp.h"
#include "../app/Operations/RotateOp.h"
#include "../app/Operations/FlipOp.h"
#include "../app/Operations/CenterOp.h"
#include "../app/Operations/SplitColorOp.h"
#include "../app/Operations/SplitHsvOp.h"
#include "../app/Operations/CombineColorOp.h"
#include "../app/Operations/CombineHsvOp.h"
#include "../app/Operations/SignalToNoiseOp.h"
#include "../app/Operations/MeanSquaredErrorOp.h"
#include "../app/Operations/FFTOp.h"
#include "../app/Operations/IFFTOp.h"
#include "../app/Operations/RandomImgOp.h"
#include "../app/Operations/NoiseOp.h"
#include "../app/Operations/BFlitOp.h"
#include "../app/Operations/DMMOp.h"
#include "../app/Operations/PseudoColorOp.h"
#include "../app/Operations/CroissanceOp.h"
#include "../app/Operations/ZeroCrossingOp.h"
#include "../app/Operations/HistogramOp.h"
#include "../app/Operations/ColorimetryOp.h"
#include "../app/Operations/SinusSynthesisOp.h"
#include "../app/Operations/ScalingOp.h"
#include "../app/Operations/QuantificationOp.h"
#include "../app/Operations/DoubleEntropyOp.h"
#include "../app/Operations/EntropyOp.h"
#include "../app/Operations/HuffmanOp.h"
#include "../app/Operations/RejectionRingOp.h"
#include "../app/Operations/DPCMEncodingOp.h"
#include "../app/Operations/HadamardOp.h"
#include "../app/Operations/DCTOp.h"
#include "../app/Operations/HoughOp.h"
#include "../app/Operations/InverseHoughOp.h"
#include "../app/Operations/PyramidOp.h"
#include "../app/Operations/InversePyramidOp.h"
#include "../app/Operations/ClassAnalysisOp.h"
#include "../app/Operations/ClassResultOp.h"
#include "../app/Operations/SeparatorOp.h"
#include "../app/Operations/MedianOp.h"
#include "../app/Operations/UCharConvertOp.h"

using namespace imagein;
using namespace genericinterface;

class TestGui: public QObject
{
    Q_OBJECT

private:
    GenericInterface* gi;
    StandardImageWindow* _img;

private slots:
    void testGenericInterfaceSetup();
    void testWindowServiceAddFile();
    void testFileServiceSave();
    void testRemoveImage();
    void testThresholdOp();
};


void TestGui::testGenericInterfaceSetup(){
      gi = new GenericInterface ("ImageInsa", Qt::LeftDockWidgetArea);
      PluginManager* pluginManager = new PluginManager(gi);
      ImageINSAService* mainService = new ImageINSAService(gi);

      BuiltinOpSet* image = new BuiltinOpSet(qApp->translate("", "&Image").toStdString());
      image->addOperation(new PointOp());
      image->addOperation(new TranslateOp());
      image->addOperation(new RotateOp());
      image->addOperation(new CenterOp());
      image->addOperation(new FlipOp(FlipOp::Horizontal));
      image->addOperation(new FlipOp(FlipOp::Vertical));
      image->addOperation(new SeparatorOp());
      image->addOperation(new SplitColorOp());
      image->addOperation(new SplitHsvOp());
      image->addOperation(new CombineColorOp());
      image->addOperation(new CombineHSVOp());
      image->addOperation(new SeparatorOp());
      image->addOperation(new ScalingOp());
      image->addOperation(new QuantificationOp());
      image->addOperation(new ThresholdOp());
      image->addOperation(new HistogramOp());
      image->addOperation(new SeparatorOp());
      image->addOperation(new UCharConvertOp());

      BuiltinOpSet* tools = new BuiltinOpSet(qApp->translate("", "&Tools").toStdString());

      tools->addOperation(new SignalToNoiseOp());
      tools->addOperation(new MeanSquaredErrorOp());
      tools->addOperation(new DoubleEntropyOp());
      tools->addOperation(new EntropyOp());
      tools->addOperation(new NoiseOp());
      tools->addOperation(new SeparatorOp());
      tools->addOperation(new RandomImgOp());
      tools->addOperation(new ColorimetryOp());
      tools->addOperation(new RejectionRingOp());
      tools->addOperation(new SinusSynthesisOp());

      BuiltinOpSet* encode = new BuiltinOpSet(qApp->translate("", "&Encoding").toStdString());
      encode->addOperation(new HuffmanOp());
      encode->addOperation(new DPCMEncodingOp());

      BuiltinOpSet* morpho = new BuiltinOpSet("&Morpho. math.");
      morpho->addOperation(new DMMOp());
      morpho->addOperation(new SeparatorOp());

      BuiltinOpSet* transfo = new BuiltinOpSet(qApp->translate("", "Transforms").toStdString());
      transfo->addOperation(new FFTOp());
      transfo->addOperation(new IFFTOp());
      transfo->addOperation(new HadamardOp());
      transfo->addOperation(new DCTOp());
      transfo->addOperation(new HoughOp());
      transfo->addOperation(new InverseHoughOp());

      BuiltinOpSet* analyse = new BuiltinOpSet(qApp->translate("", "Analysis").toStdString());
      analyse->addOperation(new CroissanceOp());
      analyse->addOperation(new ZeroCrossingOp());
      analyse->addOperation(new PyramidOp());
      analyse->addOperation(new InversePyramidOp());
      analyse->addOperation(new ClassAnalysisOp());
      analyse->addOperation(new ClassResultOp());
      analyse->addOperation(new PseudoColorOp());

      BuiltinOpSet* filter = new BuiltinOpSet(qApp->translate("", "Filtering").toStdString());
      filter->addOperation(new BFlitOp());
      filter->addOperation(new MedianOp());

      mainService->addOpSet(image);
      mainService->addOpSet(encode);
      mainService->addOpSet(morpho);
      mainService->addOpSet(analyse);
      mainService->addOpSet(transfo);
      mainService->addOpSet(filter);
      mainService->addOpSet(tools);

      gi->addService(new MorphoMatService);
      gi->addService(new filtrme::FilteringService);
      gi->addService(pluginManager);
      gi->run();
}

void TestGui::testWindowServiceAddFile() {
  WindowService* ws = dynamic_cast<WindowService*>(gi->getService(GenericInterface::WINDOW_SERVICE));
  ws->addFile(QString("test.jpg"));
  _img = dynamic_cast<StandardImageWindow*>(ws->getCurrentImageWindow());
  /* I have a doubt on the usefulness of the following test.
   *Is void casting really relevant when comparing values ?
   */
  QVERIFY((void*)_img != (void*)0);
  QCOMPARE(ws->getCurrentImageWindow()->getPath(),QString("test.jpg"));
}

void TestGui::testFileServiceSave() {
  FileService* fs = dynamic_cast<FileService*>(gi->getService(GenericInterface::FILE_SERVICE));
  fs->save("tata.jpg");

  Image test("test.jpg"), tata("tata.jpg");

  QCOMPARE(test.getWidth(),      tata.getWidth());
  QCOMPARE(test.getHeight(),     tata.getHeight());
  QCOMPARE(test.getNbChannels(), tata.getNbChannels());
}

void TestGui::testRemoveImage() {
  QFile("tata.jpg").remove();
  QString sbempty = QFINDTESTDATA("tata.jpg");
  QCOMPARE(sbempty, QString(""));
}

void TestGui::testThresholdOp(){
    WindowService* ws = dynamic_cast<WindowService*>(gi->getService(GenericInterface::WINDOW_SERVICE));
    ws->addFile(QString("test.jpg"));

    QMenu* imagemenu = gi->menu("&Image");

    QList<QAction*> list = imagemenu->actions();
    for( int i=0; i<list.count(); ++i )
    {
        std::cout << list[i]->text().toStdString() << "\n";
        list[i]->trigger();
        std::cout << i << " passed\n";
    }
}

QTEST_MAIN(TestGui)
#include "testgui.moc"
