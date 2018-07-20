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

using namespace std;
using namespace imagein;
using namespace genericinterface;

class TestGui: public QObject
{
    Q_OBJECT

private:
    GenericInterface* gi;
    StandardImageWindow* _img;
    BuiltinOpSet* _image;

private slots:
    void testGenericInterfaceSetup();
    void testWindowServiceAddFile();
    void testMenuActions();
    void testFileServiceSave();
    void testRemoveImage();

    void testThresholdOpOtsu();
    void testThresholdOp127();
    void testThresholdOpDouble();

    void testQuantifOpLinearCentered2();
    void testQuantifOpLinearCentered3();
    void testQuantifOpLinearCentered5();
    void testQuantifOpLinearCentered16();

    void testQuantifOpLloydMax2();
    void testQuantifOpLloydMax3();
    void testQuantifOpLloydMax5();
    void testQuantifOpLloydMax16();

    void testQuantifOpNonLinearCentered2();
    void testQuantifOpNonLinearCentered3();
    void testQuantifOpNonLinearCentered5();
    void testQuantifOpNonLinearCentered16();

    void testQuantifOpNonLinearMean2();
    void testQuantifOpNonLinearMean3();
    void testQuantifOpNonLinearMean5();
    void testQuantifOpNonLinearMean16();

    void testTranslateOpPos();
    void testTranslateOpNeg();

    void testRotateOpCwise();
    void testRotateOpACwise();

    void testFlipOpHor();
    void testFlipOpVer();

    void testScalingOpBilinear();
    void testScalingOpNearest();


    void testCompQuant();
    void testCompThreshold();
};


void TestGui::testGenericInterfaceSetup(){
      gi = new GenericInterface ("ImageInsa", Qt::LeftDockWidgetArea);
      PluginManager* pluginManager = new PluginManager(gi);
      ImageINSAService* mainService = new ImageINSAService(gi);

      _image = new BuiltinOpSet(qApp->translate("", "&Image").toStdString());
      _image->addOperation(new PointOp());
      _image->addOperation(new TranslateOp());
      _image->addOperation(new RotateOp());
      _image->addOperation(new CenterOp());
      _image->addOperation(new FlipOp(FlipOp::Horizontal));
      _image->addOperation(new FlipOp(FlipOp::Vertical));
      _image->addOperation(new SeparatorOp());
      _image->addOperation(new SplitColorOp());
      _image->addOperation(new SplitHsvOp());
      _image->addOperation(new CombineColorOp());
      _image->addOperation(new CombineHSVOp());
      _image->addOperation(new SeparatorOp());
      _image->addOperation(new ScalingOp());
      _image->addOperation(new QuantificationOp());
      _image->addOperation(new ThresholdOp());
      _image->addOperation(new HistogramOp());
      _image->addOperation(new SeparatorOp());
      _image->addOperation(new UCharConvertOp());

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

      mainService->addOpSet(_image);
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
  ws->addFile(QString("src/test.png"));
  _img = dynamic_cast<StandardImageWindow*>(ws->getCurrentImageWindow());
  /* I have a doubt on the usefulness of the following test.
   *Is void casting really relevant when comparing values ?
   */
  QVERIFY((void*)_img != (void*)0);
  QCOMPARE(ws->getCurrentImageWindow()->getPath(),QString("src/test.png"));
}

void TestGui::testFileServiceSave() {
  FileService* fs = dynamic_cast<FileService*>(gi->getService(GenericInterface::FILE_SERVICE));
  fs->save("tata.jpg");

  Image test("src/test.png"), tata("tata.jpg");

  QCOMPARE(test.getWidth(),      tata.getWidth());
  QCOMPARE(test.getHeight(),     tata.getHeight());
  QCOMPARE(test.getNbChannels(), tata.getNbChannels());


}

void TestGui::testRemoveImage() {
  QFile("tata.jpg").remove();
  QString sbempty = QFINDTESTDATA("tata.jpg");
  QCOMPARE(sbempty, QString(""));

}

void TestGui::testThresholdOpOtsu(){
  WindowService* ws = dynamic_cast<WindowService*>(gi->getService(GenericInterface::WINDOW_SERVICE));
  ws->addFile(QString("src/test_thresholding_imao5.bmp"));

  _img = dynamic_cast<StandardImageWindow*>(ws->getCurrentImageWindow());
  const imagein::Image* image = _img->getImage();
  map<const Image*, string> imgList;
  imgList.insert(pair<const Image*, string>(image, _img->windowTitle().toStdString()));

  std::vector<GenericOperation*> opvect = _image->getOperations();
  std::vector<GenericOperation*>::iterator it;

  for (it = opvect.begin() ; it != opvect.end(); ++it){
    if((*it)->getName()=="Thresholding"){
      (dynamic_cast<ThresholdOp*>(*it))->setTest(true);
      (dynamic_cast<ThresholdOp*>(*it))->setTestOtsu(true);
      (*it)->operator()(ws);
    }
  }

  FileService* fs = dynamic_cast<FileService*>(gi->getService(GenericInterface::FILE_SERVICE));
  fs->save("res/test_thresholding_result_imao5_otsu.vff");
}

void TestGui::testThresholdOp127(){

  WindowService* ws = dynamic_cast<WindowService*>(gi->getService(GenericInterface::WINDOW_SERVICE));
  ws->addFile(QString("src/test_thresholding_imao5.bmp"));


  _img = dynamic_cast<StandardImageWindow*>(ws->getCurrentImageWindow());
  const imagein::Image* image = _img->getImage();
  map<const Image*, string> imgList;
  imgList.insert(pair<const Image*, string>(image, _img->windowTitle().toStdString()));

  std::vector<GenericOperation*> opvect = _image->getOperations();
  std::vector<GenericOperation*>::iterator it;

  for (it = opvect.begin() ; it != opvect.end(); ++it){
    if((*it)->getName()=="Thresholding"){
      (dynamic_cast<ThresholdOp*>(*it))->setTestOtsu(false);
      (dynamic_cast<ThresholdOp*>(*it))->setTest(true);
      (dynamic_cast<ThresholdOp*>(*it))->setTestThreshold1(127);
      (*it)->operator()(ws);
    }
  }

  FileService* fs = dynamic_cast<FileService*>(gi->getService(GenericInterface::FILE_SERVICE));
  fs->save("res/test_thresholding_imao5_127.vff");
}

void TestGui::testThresholdOpDouble(){

  WindowService* ws = dynamic_cast<WindowService*>(gi->getService(GenericInterface::WINDOW_SERVICE));
  ws->addFile(QString("src/test_thresholding_imao5.bmp"));


  _img = dynamic_cast<StandardImageWindow*>(ws->getCurrentImageWindow());
  const imagein::Image* image = _img->getImage();
  map<const Image*, string> imgList;
  imgList.insert(pair<const Image*, string>(image, _img->windowTitle().toStdString()));

  std::vector<GenericOperation*> opvect = _image->getOperations();
  std::vector<GenericOperation*>::iterator it;

  for (it = opvect.begin() ; it != opvect.end(); ++it){
    if((*it)->getName()=="Thresholding"){
      (dynamic_cast<ThresholdOp*>(*it))->setTest(true);
      (dynamic_cast<ThresholdOp*>(*it))->setTestThreshold1(30);
      (dynamic_cast<ThresholdOp*>(*it))->setTestThreshold2(200);
      (*it)->operator()(ws);
    }
  }

  FileService* fs = dynamic_cast<FileService*>(gi->getService(GenericInterface::FILE_SERVICE));
  fs->save("res/test_thresholding_imao5_30_200.vff");
}

void TestGui::testQuantifOpLinearCentered2(){
  WindowService* ws = dynamic_cast<WindowService*>(gi->getService(GenericInterface::WINDOW_SERVICE));
  ws->addFile(QString("src/test_quantization_barba.bmp"));

  std::vector<GenericOperation*> opvect = _image->getOperations();
  std::vector<GenericOperation*>::iterator it;

  for (it = opvect.begin() ; it != opvect.end(); ++it){
    if((*it)->getName()=="Quantification"){
      (dynamic_cast<QuantificationOp*>(*it))->setTest(true);
      (dynamic_cast<QuantificationOp*>(*it))->setValues(2);
      (*it)->operator()(ws);
    }
  }

  std::vector<const ImageWindow*> imgvect = ws->getImageWindows();
  std::vector<const ImageWindow*>::iterator it2;

  for(it2 = imgvect.begin() ; it2 != imgvect.end() ; ++it2){
    cout << (*it2)->windowTitle().toStdString() << "\n";
  }

  _img = dynamic_cast<StandardImageWindow*>(ws->getCurrentImageWindow());

  FileService* fs = dynamic_cast<FileService*>(gi->getService(GenericInterface::FILE_SERVICE));
  fs->save("res/test_quantization_LinearCentered_barba_2.bmp");
}

void TestGui::testQuantifOpLinearCentered3(){
  WindowService* ws = dynamic_cast<WindowService*>(gi->getService(GenericInterface::WINDOW_SERVICE));
  ws->addFile(QString("src/test_quantization_berber.bmp"));

  std::vector<GenericOperation*> opvect = _image->getOperations();
  std::vector<GenericOperation*>::iterator it;

  for (it = opvect.begin() ; it != opvect.end(); ++it){
    if((*it)->getName()=="Quantification"){
      (dynamic_cast<QuantificationOp*>(*it))->setTest(true);
      (dynamic_cast<QuantificationOp*>(*it))->setValues(3);
      (*it)->operator()(ws);
    }
  }

  _img = dynamic_cast<StandardImageWindow*>(ws->getCurrentImageWindow());

  FileService* fs = dynamic_cast<FileService*>(gi->getService(GenericInterface::FILE_SERVICE));
  fs->save("res/test_quantization_LinearCentered_berber_3.bmp");
}

void TestGui::testQuantifOpLinearCentered5(){
  WindowService* ws = dynamic_cast<WindowService*>(gi->getService(GenericInterface::WINDOW_SERVICE));
  ws->addFile(QString("src/test_quantization_imao5.bmp"));

  std::vector<GenericOperation*> opvect = _image->getOperations();
  std::vector<GenericOperation*>::iterator it;

  for (it = opvect.begin() ; it != opvect.end(); ++it){
    if((*it)->getName()=="Quantification"){
      (dynamic_cast<QuantificationOp*>(*it))->setTest(true);
      (dynamic_cast<QuantificationOp*>(*it))->setValues(5);
      (*it)->operator()(ws);
    }
  }

  _img = dynamic_cast<StandardImageWindow*>(ws->getCurrentImageWindow());

  FileService* fs = dynamic_cast<FileService*>(gi->getService(GenericInterface::FILE_SERVICE));
  fs->save("res/test_quantization_LinearCentered_imao5_5.bmp");

}

void TestGui::testQuantifOpLinearCentered16(){
  WindowService* ws = dynamic_cast<WindowService*>(gi->getService(GenericInterface::WINDOW_SERVICE));
  ws->addFile(QString("src/test_quantization_monarch.bmp"));

  std::vector<GenericOperation*> opvect = _image->getOperations();
  std::vector<GenericOperation*>::iterator it;

  for (it = opvect.begin() ; it != opvect.end(); ++it){
    if((*it)->getName()=="Quantification"){
      (dynamic_cast<QuantificationOp*>(*it))->setTest(true);
      (dynamic_cast<QuantificationOp*>(*it))->setValues(16);
      (*it)->operator()(ws);
    }
  }

  _img = dynamic_cast<StandardImageWindow*>(ws->getCurrentImageWindow());

  FileService* fs = dynamic_cast<FileService*>(gi->getService(GenericInterface::FILE_SERVICE));
  fs->save("res/test_quantization_LinearCentered_monarch_16.bmp");

}

void TestGui::testQuantifOpLloydMax2(){
  WindowService* ws = dynamic_cast<WindowService*>(gi->getService(GenericInterface::WINDOW_SERVICE));
  ws->addFile(QString("src/test_quantization_barba.bmp"));

  std::vector<GenericOperation*> opvect = _image->getOperations();
  std::vector<GenericOperation*>::iterator it;

  for (it = opvect.begin() ; it != opvect.end(); ++it){
    if((*it)->getName()=="Quantification"){
      (dynamic_cast<QuantificationOp*>(*it))->setTest(true);
      (dynamic_cast<QuantificationOp*>(*it))->setQuantif(4);
      (dynamic_cast<QuantificationOp*>(*it))->setValues(2);
      (*it)->operator()(ws);
    }
  }
  _img = dynamic_cast<StandardImageWindow*>(ws->getCurrentImageWindow());

  FileService* fs = dynamic_cast<FileService*>(gi->getService(GenericInterface::FILE_SERVICE));
  fs->save("res/test_quantization_lloydMax_barba_2.bmp");
}

void TestGui::testQuantifOpLloydMax3(){
  WindowService* ws = dynamic_cast<WindowService*>(gi->getService(GenericInterface::WINDOW_SERVICE));
  ws->addFile(QString("src/test_quantization_berber.bmp"));

  std::vector<GenericOperation*> opvect = _image->getOperations();
  std::vector<GenericOperation*>::iterator it;

  for (it = opvect.begin() ; it != opvect.end(); ++it){
    if((*it)->getName()=="Quantification"){
      (dynamic_cast<QuantificationOp*>(*it))->setTest(true);
      (dynamic_cast<QuantificationOp*>(*it))->setQuantif(4);
      (dynamic_cast<QuantificationOp*>(*it))->setValues(3);
      (*it)->operator()(ws);
    }
  }

  _img = dynamic_cast<StandardImageWindow*>(ws->getCurrentImageWindow());

  FileService* fs = dynamic_cast<FileService*>(gi->getService(GenericInterface::FILE_SERVICE));
  fs->save("res/test_quantization_lloydMax_berber_3.bmp");
}

void TestGui::testQuantifOpLloydMax5(){
  WindowService* ws = dynamic_cast<WindowService*>(gi->getService(GenericInterface::WINDOW_SERVICE));
  ws->addFile(QString("src/test_quantization_imao5.bmp"));

  std::vector<GenericOperation*> opvect = _image->getOperations();
  std::vector<GenericOperation*>::iterator it;

  for (it = opvect.begin() ; it != opvect.end(); ++it){
    if((*it)->getName()=="Quantification"){
      (dynamic_cast<QuantificationOp*>(*it))->setTest(true);
      (dynamic_cast<QuantificationOp*>(*it))->setQuantif(4);
      (dynamic_cast<QuantificationOp*>(*it))->setValues(5);
      (*it)->operator()(ws);
    }
  }

  _img = dynamic_cast<StandardImageWindow*>(ws->getCurrentImageWindow());

  FileService* fs = dynamic_cast<FileService*>(gi->getService(GenericInterface::FILE_SERVICE));
  fs->save("res/test_quantization_lloydMax_imao5_5.bmp");

}

void TestGui::testQuantifOpLloydMax16(){
  WindowService* ws = dynamic_cast<WindowService*>(gi->getService(GenericInterface::WINDOW_SERVICE));
  ws->addFile(QString("src/test_quantization_monarch.bmp"));

  std::vector<GenericOperation*> opvect = _image->getOperations();
  std::vector<GenericOperation*>::iterator it;

  for (it = opvect.begin() ; it != opvect.end(); ++it){
    if((*it)->getName()=="Quantification"){
      (dynamic_cast<QuantificationOp*>(*it))->setTest(true);
      (dynamic_cast<QuantificationOp*>(*it))->setQuantif(4);
      (dynamic_cast<QuantificationOp*>(*it))->setValues(16);
      (*it)->operator()(ws);
    }
  }

  _img = dynamic_cast<StandardImageWindow*>(ws->getCurrentImageWindow());

  FileService* fs = dynamic_cast<FileService*>(gi->getService(GenericInterface::FILE_SERVICE));
  fs->save("res/test_quantization_lloydMax_monarch_16.bmp");

}

void TestGui::testQuantifOpNonLinearCentered2(){
  WindowService* ws = dynamic_cast<WindowService*>(gi->getService(GenericInterface::WINDOW_SERVICE));
  ws->addFile(QString("src/test_quantization_barba.bmp"));

  std::vector<GenericOperation*> opvect = _image->getOperations();
  std::vector<GenericOperation*>::iterator it;

  for (it = opvect.begin() ; it != opvect.end(); ++it){
    if((*it)->getName()=="Quantification"){
      (dynamic_cast<QuantificationOp*>(*it))->setTest(true);
      (dynamic_cast<QuantificationOp*>(*it))->setQuantif(1);
      (dynamic_cast<QuantificationOp*>(*it))->setValues(2);
      (*it)->operator()(ws);
    }
  }
  _img = dynamic_cast<StandardImageWindow*>(ws->getCurrentImageWindow());

  FileService* fs = dynamic_cast<FileService*>(gi->getService(GenericInterface::FILE_SERVICE));
  fs->save("res/test_quantization_nonlinearCentered_barba_2.bmp");
}

void TestGui::testQuantifOpNonLinearCentered3(){
  WindowService* ws = dynamic_cast<WindowService*>(gi->getService(GenericInterface::WINDOW_SERVICE));
  ws->addFile(QString("src/test_quantization_berber.bmp"));

  std::vector<GenericOperation*> opvect = _image->getOperations();
  std::vector<GenericOperation*>::iterator it;

  for (it = opvect.begin() ; it != opvect.end(); ++it){
    if((*it)->getName()=="Quantification"){
      (dynamic_cast<QuantificationOp*>(*it))->setTest(true);
      (dynamic_cast<QuantificationOp*>(*it))->setQuantif(1);
      (dynamic_cast<QuantificationOp*>(*it))->setValues(3);
      (*it)->operator()(ws);
    }
  }

  _img = dynamic_cast<StandardImageWindow*>(ws->getCurrentImageWindow());

  FileService* fs = dynamic_cast<FileService*>(gi->getService(GenericInterface::FILE_SERVICE));
  fs->save("res/test_quantization_nonlinearCentered_berber_3.bmp");
}

void TestGui::testQuantifOpNonLinearCentered5(){
  WindowService* ws = dynamic_cast<WindowService*>(gi->getService(GenericInterface::WINDOW_SERVICE));
  ws->addFile(QString("src/test_quantization_imao5.bmp"));

  std::vector<GenericOperation*> opvect = _image->getOperations();
  std::vector<GenericOperation*>::iterator it;

  for (it = opvect.begin() ; it != opvect.end(); ++it){
    if((*it)->getName()=="Quantification"){
      (dynamic_cast<QuantificationOp*>(*it))->setTest(true);
      (dynamic_cast<QuantificationOp*>(*it))->setQuantif(1);
      (dynamic_cast<QuantificationOp*>(*it))->setValues(5);
      (*it)->operator()(ws);
    }
  }

  _img = dynamic_cast<StandardImageWindow*>(ws->getCurrentImageWindow());

  FileService* fs = dynamic_cast<FileService*>(gi->getService(GenericInterface::FILE_SERVICE));
  fs->save("res/test_quantization_nonlinearCentered_imao5_5.bmp");

}

void TestGui::testQuantifOpNonLinearCentered16(){
  WindowService* ws = dynamic_cast<WindowService*>(gi->getService(GenericInterface::WINDOW_SERVICE));
  ws->addFile(QString("src/test_quantization_monarch.bmp"));

  std::vector<GenericOperation*> opvect = _image->getOperations();
  std::vector<GenericOperation*>::iterator it;

  for (it = opvect.begin() ; it != opvect.end(); ++it){
    if((*it)->getName()=="Quantification"){
      (dynamic_cast<QuantificationOp*>(*it))->setTest(true);
      (dynamic_cast<QuantificationOp*>(*it))->setQuantif(1);
      (dynamic_cast<QuantificationOp*>(*it))->setValues(16);
      (*it)->operator()(ws);
    }
  }

  _img = dynamic_cast<StandardImageWindow*>(ws->getCurrentImageWindow());

  FileService* fs = dynamic_cast<FileService*>(gi->getService(GenericInterface::FILE_SERVICE));
  fs->save("res/test_quantization_nonlinearCentered_monarch_16.bmp");

}

void TestGui::testQuantifOpNonLinearMean2(){
  WindowService* ws = dynamic_cast<WindowService*>(gi->getService(GenericInterface::WINDOW_SERVICE));
  ws->addFile(QString("src/test_quantization_barba.bmp"));

  std::vector<GenericOperation*> opvect = _image->getOperations();
  std::vector<GenericOperation*>::iterator it;

  for (it = opvect.begin() ; it != opvect.end(); ++it){
    if((*it)->getName()=="Quantification"){
      (dynamic_cast<QuantificationOp*>(*it))->setTest(true);
      (dynamic_cast<QuantificationOp*>(*it))->setQuantif(2);
      (dynamic_cast<QuantificationOp*>(*it))->setValues(2);
      (*it)->operator()(ws);
    }
  }
  _img = dynamic_cast<StandardImageWindow*>(ws->getCurrentImageWindow());

  FileService* fs = dynamic_cast<FileService*>(gi->getService(GenericInterface::FILE_SERVICE));
  fs->save("res/test_quantization_nonLinearMean_Value_barba_2.bmp");
}

void TestGui::testQuantifOpNonLinearMean3(){
  WindowService* ws = dynamic_cast<WindowService*>(gi->getService(GenericInterface::WINDOW_SERVICE));
  ws->addFile(QString("src/test_quantization_berber.bmp"));

  std::vector<GenericOperation*> opvect = _image->getOperations();
  std::vector<GenericOperation*>::iterator it;

  for (it = opvect.begin() ; it != opvect.end(); ++it){
    if((*it)->getName()=="Quantification"){
      (dynamic_cast<QuantificationOp*>(*it))->setTest(true);
      (dynamic_cast<QuantificationOp*>(*it))->setQuantif(2);
      (dynamic_cast<QuantificationOp*>(*it))->setValues(3);
      (*it)->operator()(ws);
    }
  }

  _img = dynamic_cast<StandardImageWindow*>(ws->getCurrentImageWindow());

  FileService* fs = dynamic_cast<FileService*>(gi->getService(GenericInterface::FILE_SERVICE));
  fs->save("res/test_quantization_nonLinearMean_Value_berber_3.bmp");
}

void TestGui::testQuantifOpNonLinearMean5(){
  WindowService* ws = dynamic_cast<WindowService*>(gi->getService(GenericInterface::WINDOW_SERVICE));
  ws->addFile(QString("src/test_quantization_imao5.bmp"));

  std::vector<GenericOperation*> opvect = _image->getOperations();
  std::vector<GenericOperation*>::iterator it;

  for (it = opvect.begin() ; it != opvect.end(); ++it){
    if((*it)->getName()=="Quantification"){
      (dynamic_cast<QuantificationOp*>(*it))->setTest(true);
      (dynamic_cast<QuantificationOp*>(*it))->setQuantif(2);
      (dynamic_cast<QuantificationOp*>(*it))->setValues(5);
      (*it)->operator()(ws);
    }
  }

  _img = dynamic_cast<StandardImageWindow*>(ws->getCurrentImageWindow());

  FileService* fs = dynamic_cast<FileService*>(gi->getService(GenericInterface::FILE_SERVICE));
  fs->save("res/test_quantization_nonLinearMean_Value_imao5_5.bmp");

}

void TestGui::testQuantifOpNonLinearMean16(){
  WindowService* ws = dynamic_cast<WindowService*>(gi->getService(GenericInterface::WINDOW_SERVICE));
  ws->addFile(QString("src/test_quantization_monarch.bmp"));

  std::vector<GenericOperation*> opvect = _image->getOperations();
  std::vector<GenericOperation*>::iterator it;

  for (it = opvect.begin() ; it != opvect.end(); ++it){
    if((*it)->getName()=="Quantification"){
      (dynamic_cast<QuantificationOp*>(*it))->setTest(true);
      (dynamic_cast<QuantificationOp*>(*it))->setQuantif(2);
      (dynamic_cast<QuantificationOp*>(*it))->setValues(16);
      (*it)->operator()(ws);
    }
  }

  _img = dynamic_cast<StandardImageWindow*>(ws->getCurrentImageWindow());

  FileService* fs = dynamic_cast<FileService*>(gi->getService(GenericInterface::FILE_SERVICE));
  fs->save("res/test_quantization_nonLinearMean_Value_monarch_16.bmp");
}

void TestGui::testTranslateOpPos(){
  WindowService* ws = dynamic_cast<WindowService*>(gi->getService(GenericInterface::WINDOW_SERVICE));
  ws->addFile(QString("src/test_imageop_cdm.bmp"));

  std::vector<GenericOperation*> opvect = _image->getOperations();
  std::vector<GenericOperation*>::iterator it;

  for (it = opvect.begin() ; it != opvect.end(); ++it){
    if((*it)->getName()=="Translation"){
      (dynamic_cast<TranslateOp*>(*it))->setTest(true);
      (dynamic_cast<TranslateOp*>(*it))->setXVal(100);
      (dynamic_cast<TranslateOp*>(*it))->setYVal(50);
      (*it)->operator()(ws);
    }
  }
  _img = dynamic_cast<StandardImageWindow*>(ws->getCurrentImageWindow());

  FileService* fs = dynamic_cast<FileService*>(gi->getService(GenericInterface::FILE_SERVICE));
  fs->save("res/test_translation_result_cdm_100_50.bmp");
}

void TestGui::testTranslateOpNeg(){
  WindowService* ws = dynamic_cast<WindowService*>(gi->getService(GenericInterface::WINDOW_SERVICE));
  ws->addFile(QString("src/test_imageop_cdm.bmp"));

  std::vector<GenericOperation*> opvect = _image->getOperations();
  std::vector<GenericOperation*>::iterator it;

  for (it = opvect.begin() ; it != opvect.end(); ++it){
    if((*it)->getName()=="Translation"){
      (dynamic_cast<TranslateOp*>(*it))->setTest(true);
      (dynamic_cast<TranslateOp*>(*it))->setXVal(-100);
      (dynamic_cast<TranslateOp*>(*it))->setYVal(-50);
      (*it)->operator()(ws);
    }
  }

  _img = dynamic_cast<StandardImageWindow*>(ws->getCurrentImageWindow());

  FileService* fs = dynamic_cast<FileService*>(gi->getService(GenericInterface::FILE_SERVICE));
  fs->save("res/test_translation_result_cdm_-100_-50.bmp");
}

void TestGui::testRotateOpCwise(){
  WindowService* ws = dynamic_cast<WindowService*>(gi->getService(GenericInterface::WINDOW_SERVICE));
  ws->addFile(QString("src/test_imageop_cdm.bmp"));

  std::vector<GenericOperation*> opvect = _image->getOperations();
  std::vector<GenericOperation*>::iterator it;

  for (it = opvect.begin() ; it != opvect.end(); ++it){
    if((*it)->getName()=="Rotation"){
      (dynamic_cast<RotateOp*>(*it))->setTest(true);
      (dynamic_cast<RotateOp*>(*it))->setAngle(-30.0);
      (*it)->operator()(ws);
    }
  }
  _img = dynamic_cast<StandardImageWindow*>(ws->getCurrentImageWindow());

  FileService* fs = dynamic_cast<FileService*>(gi->getService(GenericInterface::FILE_SERVICE));
  fs->save("res/test_rotation_result_cdm_clkwise_-30.bmp");
}

void TestGui::testRotateOpACwise(){
  WindowService* ws = dynamic_cast<WindowService*>(gi->getService(GenericInterface::WINDOW_SERVICE));
  ws->addFile(QString("src/test_imageop_cdm.bmp"));

  std::vector<GenericOperation*> opvect = _image->getOperations();
  std::vector<GenericOperation*>::iterator it;

  for (it = opvect.begin() ; it != opvect.end(); ++it){
    if((*it)->getName()=="Rotation"){
      (dynamic_cast<RotateOp*>(*it))->setTest(true);
      (dynamic_cast<RotateOp*>(*it))->setAngle(30.0);
      (*it)->operator()(ws);
    }
  }

  _img = dynamic_cast<StandardImageWindow*>(ws->getCurrentImageWindow());

  FileService* fs = dynamic_cast<FileService*>(gi->getService(GenericInterface::FILE_SERVICE));
  fs->save("res/test_rotation_result_cdm_anticlkwise_30.bmp");
}

void TestGui::testFlipOpHor(){
  WindowService* ws = dynamic_cast<WindowService*>(gi->getService(GenericInterface::WINDOW_SERVICE));
  ws->addFile(QString("src/test_imageop_cdm.bmp"));

  std::vector<GenericOperation*> opvect = _image->getOperations();
  std::vector<GenericOperation*>::iterator it;

  for (it = opvect.begin() ; it != opvect.end(); ++it){
    if((*it)->getName()=="Flip horizontal"){
      (*it)->operator()(ws);
    }
  }

  _img = dynamic_cast<StandardImageWindow*>(ws->getCurrentImageWindow());

  FileService* fs = dynamic_cast<FileService*>(gi->getService(GenericInterface::FILE_SERVICE));
  fs->save("res/test_flip_result_cdm_horizontal.bmp");
}

void TestGui::testFlipOpVer(){
  WindowService* ws = dynamic_cast<WindowService*>(gi->getService(GenericInterface::WINDOW_SERVICE));
  ws->addFile(QString("src/test_imageop_cdm.bmp"));

  std::vector<GenericOperation*> opvect = _image->getOperations();
  std::vector<GenericOperation*>::iterator it;

  for (it = opvect.begin() ; it != opvect.end(); ++it){
    if((*it)->getName()=="Flip vertical"){
      (*it)->operator()(ws);
    }
  }

  _img = dynamic_cast<StandardImageWindow*>(ws->getCurrentImageWindow());

  FileService* fs = dynamic_cast<FileService*>(gi->getService(GenericInterface::FILE_SERVICE));
  fs->save("res/test_flip_result_cdm_vertical.bmp");
}

void TestGui::testScalingOpBilinear(){
  WindowService* ws = dynamic_cast<WindowService*>(gi->getService(GenericInterface::WINDOW_SERVICE));
  ws->addFile(QString("src/test_scaling_toinou.bmp"));

  std::vector<GenericOperation*> opvect = _image->getOperations();
  std::vector<GenericOperation*>::iterator it;

  for (it = opvect.begin() ; it != opvect.end(); ++it){
    if((*it)->getName()=="Scaling"){
      (dynamic_cast<ScalingOp*>(*it))->setTest(true);
      (dynamic_cast<ScalingOp*>(*it))->setInterpolation(1);
      (dynamic_cast<ScalingOp*>(*it))->setXFactor(2.0);
      (dynamic_cast<ScalingOp*>(*it))->setYFactor(1.0);
      (*it)->operator()(ws);
    }
  }

  _img = dynamic_cast<StandardImageWindow*>(ws->getCurrentImageWindow());

  FileService* fs = dynamic_cast<FileService*>(gi->getService(GenericInterface::FILE_SERVICE));
  fs->save("res/test_scaling_result_toinou_x2_y1_bilinear.bmp");
}

void TestGui::testScalingOpNearest(){
  WindowService* ws = dynamic_cast<WindowService*>(gi->getService(GenericInterface::WINDOW_SERVICE));
  ws->addFile(QString("src/test_scaling_toinou.bmp"));

  std::vector<GenericOperation*> opvect = _image->getOperations();
  std::vector<GenericOperation*>::iterator it;

  for (it = opvect.begin() ; it != opvect.end(); ++it){
    if((*it)->getName()=="Scaling"){
      (dynamic_cast<ScalingOp*>(*it))->setTest(true);
      (dynamic_cast<ScalingOp*>(*it))->setInterpolation(0);
      (dynamic_cast<ScalingOp*>(*it))->setXFactor(0.2);
      (dynamic_cast<ScalingOp*>(*it))->setYFactor(0.2);
      (*it)->operator()(ws);
    }
  }
  //cout << "ouagzeruvhe _________________________________________##################\n";
  _img = dynamic_cast<StandardImageWindow*>(ws->getCurrentImageWindow());
//  cout << "ouagzeruvhe _________________________________________333333333333333333\n";
  FileService* fs = dynamic_cast<FileService*>(gi->getService(GenericInterface::FILE_SERVICE));
  fs->save("res/test_scaling_result_toinou_x0.2_y0.2_nearest.bmp");
}



void TestGui::testCompThreshold(){
  WindowService* ws = dynamic_cast<WindowService*>(gi->getService(GenericInterface::WINDOW_SERVICE));

  const Image* res = new Image("ref/test_thresholding_result_otsuMansion_otsu.jpg");
  const Image* ref = new Image("res/test_thresholding_otsuMansion.vff");

  for(unsigned int i = 0; i < res->getHeight(); i++)
    for(unsigned int j = 0; j < res->getWidth(); j++)
      for(unsigned int nb = 0; nb < res->getNbChannels(); nb++) {
        QCOMPARE((int)res->getPixel(j, i, nb) , (int) ref->getPixel(j, i, nb));
      }

  res = new Image("ref/test_thresholding_result_imao5_127.jpg");
  ref = new Image("res/test_thresholding_imao5.vff");

  for(unsigned int i = 0; i < res->getHeight(); i++)
    for(unsigned int j = 0; j < res->getWidth(); j++)
      for(unsigned int nb = 0; nb < res->getNbChannels(); nb++) {
        QCOMPARE((int)res->getPixel(j, i, nb) , (int) ref->getPixel(j, i, nb));
      }
}



/*  L'idée serait de split les tests en deux fichiers deux tests; le premier (celui ci)
    servirait à créer les images issues des opérations, qui seraient stockées dans le directory
    res; issues du directory src.

    La seconde classe comparerait les images créées à l'aide de matlab considérées comme références,
    dans le dossier ref à celles du fichier res. Le tout se ferait à la suite mais à l'issu du même
    appel à la classe de test.

    On aurait donc un testAlgo et un testComp, qui pour la même opé threshold feraient:
      testAlgo -> testThresholdOp
      testComp -> testResThresholdOp

*/
void TestGui::testCompQuant(){

}

void TestGui::testMenuActions(){
  // dans ce test on peut vérifier que les menus contiennent bien ce qu'ils sont sensés contenir
  // En accédant aux actions du menu et en les listant simplement.
    WindowService* ws = dynamic_cast<WindowService*>(gi->getService(GenericInterface::WINDOW_SERVICE));
    _img = dynamic_cast<StandardImageWindow*>(ws->getCurrentImageWindow());

    QMenu* imagemenu = gi->menu("&Image");

    std::cout << _img->getPath().toStdString() << "\n";
    imagemenu->menuAction()->trigger();
    for( int i=0; i<imagemenu->actions().count(); ++i )
    {
        std::cout << (imagemenu->actions())[i]->text().toStdString() << "\n";
    }

}

QTEST_MAIN(TestGui)
#include "testgui.moc"
