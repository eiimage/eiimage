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

#include "ScalingOp.h"
#include "../Tools.h"
#include <Widgets/ImageWidgets/StandardImageWindow.h>
#include <Widgets/ImageWidgets/DoubleImageWindow.h>
#include <QDialog>
#include <QFormLayout>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>
#include <QApplication>
#include <GrayscaleImage.h>
#include <QGroupBox>
#include <QRadioButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QDoubleSpinBox>

using namespace std;
using namespace imagein;
using namespace genericinterface;

ScalingOp::ScalingOp() : GenericOperation(qApp->translate("Operations", "Scaling (Enlarge/Reduce)").toStdString())
{
  _test=false, _interp=0, _xfactor=1., _yfactor=1.;
}

bool ScalingOp::needCurrentImg() const {
    return true;
}

bool ScalingOp::isValidImgWnd(const genericinterface::ImageWindow* imgWnd) const {
    return imgWnd != NULL;
}

void ScalingOp::operator()(const genericinterface::ImageWindow* currentWnd, const vector<const ImageWindow*>&) {

    QDialog* dialog = new QDialog(QApplication::activeWindow());
    dialog->setWindowTitle(QString(qApp->translate("Operations", "Scaling (Enlarge/Reduce)")));
    dialog->setMinimumWidth(180);
    QGridLayout* layout = new QGridLayout();
    dialog->setLayout(layout);

//    QDoubleSpinBox* xScaleBox = new QDoubleSpinBox();
//    QDoubleSpinBox* yScaleBox = new QDoubleSpinBox();

//    QDoubleSpinBox* xScaleBoxN = new QDoubleSpinBox();
//    QDoubleSpinBox* xScaleBoxD = new QDoubleSpinBox();

    QLineEdit* xScaleBoxN = new QLineEdit();
    xScaleBoxN->setText("1");
    xScaleBoxN->setValidator(new QIntValidator(0, 100));
    QLineEdit* xScaleBoxD = new QLineEdit();
    xScaleBoxD->setText("1");
    xScaleBoxD->setValidator(new QIntValidator(0, 100));

//    QDoubleSpinBox* yScaleBoxN = new QDoubleSpinBox();
//    QDoubleSpinBox* yScaleBoxD = new QDoubleSpinBox();
    QLineEdit* yScaleBoxN = new QLineEdit();
    yScaleBoxN->setText("1");
    yScaleBoxN->setValidator(new QIntValidator(0, 100));
    QLineEdit* yScaleBoxD = new QLineEdit();
    yScaleBoxD->setText("1");
    yScaleBoxD->setValidator(new QIntValidator(0, 100));

//    xScaleBox->setValue(1.0);
//    yScaleBox->setValue(1.0);

    QComboBox* algoBox = new QComboBox();
    algoBox->addItem(qApp->translate("ScalingOp", "Nearest neighboor (standard)"));
    algoBox->addItem(qApp->translate("ScalingOp", "Bi-linear"));
    algoBox->addItem(qApp->translate("ScalingOp", "Parabolic"));
    algoBox->addItem(qApp->translate("ScalingOp", "Spline"));

//    layout->insertRow(0, qApp->translate("ScalingOp", "Interpolation : "), algoBox);
//    layout->insertRow(1, qApp->translate("ScalingOp", "X scale factor : "), xScaleBoxN);
//    layout->insertRow(1, qApp->translate("ScalingOp", " / "), xScaleBoxD);
//    layout->insertRow(2, qApp->translate("ScalingOp", "Y scale factor : "), yScaleBoxN);
//    layout->insertRow(2, qApp->translate("ScalingOp", " / "), yScaleBoxD);

    QLabel* interpolation = new QLabel();
    interpolation->setText(qApp->translate("ScalingOp", "Interpolation : "));
    QLabel* xscaleRow = new QLabel();
    xscaleRow->setText(qApp->translate("ScalingOp", "X scale factor : "));
    QLabel* yscaleRow = new QLabel();
    yscaleRow->setText(qApp->translate("ScalingOp", "Y scale factor : "));
    QLabel* xDivision = new QLabel();
    xDivision->setText("   /   ");
    QLabel* yDivision = new QLabel();
    yDivision->setText("   /   ");

    layout->addWidget(interpolation, 0, 0, 1, 1);
    layout->addWidget(algoBox, 0, 1, 1, 3);
    layout->addWidget(xscaleRow, 1, 0, 1, 1);
    layout->addWidget(xScaleBoxN, 1, 1, 1, 1);
    layout->addWidget(xDivision, 1, 2, 1, 1);
    layout->addWidget(xScaleBoxD, 1, 3, 1, 1);
    layout->addWidget(yscaleRow, 2, 0, 1, 1);
    layout->addWidget(yScaleBoxN, 2, 1, 1, 1);
    layout->addWidget(yDivision, 2, 2, 1, 1);
    layout->addWidget(yScaleBoxD, 2, 3, 1, 1);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, Qt::Horizontal, dialog);
    layout->addWidget(buttonBox, 3, 0, 1, 4);
    QObject::connect(buttonBox, SIGNAL(accepted()), dialog, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), dialog, SLOT(reject()));

    if(_test){
//      xScaleBox->setValue(_xfactor);
//      yScaleBox->setValue(_yfactor);

      xScaleBoxN->setText(QString::number(_xfactor));
      xScaleBoxD->setText(QString::number(_yfactor));
      yScaleBoxN->setText(QString::number(_xfactor));
      yScaleBoxD->setText(QString::number(_yfactor));
      algoBox->setCurrentIndex(_interp);
    }
    else{
      QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog->exec());
      if(code!=QDialog::Accepted) return;
    }

    Interpolation inter;
    switch(algoBox->currentIndex()) {
        case 1: inter = BilinearInterpolation; break;
        case 2: inter = ParabolicInterpolation; break;
        case 3: inter = SplineInterpolation; break;
        default: inter = NearestInterpolation; break;
    }

//    /*Process input text to get usable data*/
//    QString xScale = xScaleBox->text();
//    QString yScale = yScaleBox->text();
//    if(xScale.contains('/')){
//        QString numerator, denominator;
//        for (QChar *it = xScale.begin(); it!=xScale.end(); ++it) {
//           if(it!=Qchar('/')) numerator+=it;
//        }
//    }else if(xScale.toDouble()){
//        double xValue = xScale.toDouble();
//    }else{

//    }



    if(currentWnd->isStandard()) {
        const Image* image = static_cast<const StandardImageWindow*>(currentWnd)->getImage();
        Image* resImg = NULL;
        switch(inter) {
            case NearestInterpolation:
//                resImg = scale<Image::depth_t, Nearest>(image, xScaleBox->value(), yScaleBox->value());
                resImg = scale<Image::depth_t, Nearest>(image, xScaleBoxN->text().toDouble()/xScaleBoxD->text().toDouble(), yScaleBoxN->text().toDouble()/yScaleBoxD->text().toDouble());
                break;
            case BilinearInterpolation:
//                resImg = scale<Image::depth_t, Bilinear>(image, xScaleBox->value(), yScaleBox->value());
                resImg = scale<Image::depth_t, Bilinear>(image, xScaleBoxN->text().toDouble()/xScaleBoxD->text().toDouble(), yScaleBoxN->text().toDouble()/yScaleBoxD->text().toDouble());
                break;
            case ParabolicInterpolation:
//                resImg = scale<Image::depth_t, Parabolic>(image, xScaleBox->value(), yScaleBox->value());
                resImg = scale<Image::depth_t, Parabolic>(image, xScaleBoxN->text().toDouble()/xScaleBoxD->text().toDouble(), yScaleBoxN->text().toDouble()/yScaleBoxD->text().toDouble());
                break;
            case SplineInterpolation:
//                resImg = scale<Image::depth_t, Spline>(image, xScaleBox->value(), yScaleBox->value());
                resImg = scale<Image::depth_t, Spline>(image, xScaleBoxN->text().toDouble()/xScaleBoxD->text().toDouble(), yScaleBoxN->text().toDouble()/yScaleBoxD->text().toDouble());
                break;
        }
        if(resImg != NULL) {
            QString interpolationType;
            if(inter==1) interpolationType = QString(qApp->translate("ScalingOp","Bilinear Interpolation"));
            else if(inter==2) interpolationType = QString(qApp->translate("ScalingOp","Parabolic Interpolation"));
            else if(inter==3) interpolationType = QString(qApp->translate("ScalingOp","Spline Interpolation"));
            else interpolationType = QString(qApp->translate("ScalingOp","Nearest Interpolation"));
            outImage(resImg,qApp->translate("ScalingOp","Scaled").toStdString() + " - " + interpolationType.toStdString());
        }
    }
    else if(currentWnd->isDouble()) {
        const Image_t<double>* image = static_cast<const DoubleImageWindow*>(currentWnd)->getImage();
//        Image_t<double>* resImg = scale<double, Nearest>(image, xScaleBox->value(), yScaleBox->value());
        Image_t<double>* resImg = scale<double, Nearest>(image, xScaleBoxN->text().toDouble()/xScaleBoxD->text().toDouble(), yScaleBoxN->text().toDouble()/yScaleBoxD->text().toDouble());
        outDoubleImage(resImg, qApp->translate("ScalingOp", "scaled").toStdString());
    }
}

template<typename D, template<typename> class I>
Image_t<D> *ScalingOp::scale(const Image_t<D> *image, double xScale, double yScale) {

    unsigned int newWidth = image->getWidth() * xScale;
    unsigned int newHeight = image->getHeight() * yScale;
    Image_t<double>* tmpImg = new Image_t<double>(newWidth, image->getHeight(), image->getNbChannels(), 0.);

    if(newWidth > image->getWidth()) {
        for(unsigned int c = 0; c < image->getNbChannels(); ++c) {
            for(unsigned int j = 0; j < image->getHeight(); ++j) {
                I<D>::interpolate(image->getConstRow(j, c), tmpImg->getRow(j, c));
            }
        }
    }
    else {
        for(unsigned int c = 0; c < tmpImg->getNbChannels(); ++c) {
            for(unsigned int j = 0; j < image->getHeight(); ++j) {
                for(unsigned int i = 0; i < tmpImg->getWidth(); ++i) {
                    double value = 0;
                    int nValue = 0;
                    for(unsigned int k = i / xScale; k < (i + 1) / xScale; ++k) {
                        value += image->getPixelAt(k, j, c);
                        ++nValue;
                    }
                    tmpImg->setPixelAt(i, j, c, value / nValue);
                }
            }
        }
    }

    Image_t<double>* tmpImg2 = new Image_t<double>(tmpImg->getWidth(), newHeight, image->getNbChannels(), 0.);

    if(newHeight >= image->getHeight()) {
        for(unsigned int c = 0; c < tmpImg->getNbChannels(); ++c) {
            for(unsigned int i = 0; i < tmpImg->getWidth(); ++i) {
                I<double>::interpolate(tmpImg->getConstColumn(i, c), tmpImg2->getColumn(i, c));
            }
        }

    }
    else {
        for(unsigned int c = 0; c < tmpImg2->getNbChannels(); ++c) {
            for(unsigned int i = 0; i < tmpImg2->getWidth(); ++i) {
                for(unsigned int j = 0; j < tmpImg2->getHeight(); ++j) {
                    double value = 0;
                    int nValue = 0;
                    for(unsigned int k = j / yScale; k < (j + 1) / yScale; ++k) {
                        value += tmpImg->getPixelAt(i, k, c);
                        ++nValue;
                    }
                    tmpImg2->setPixelAt(i, j, c, value / nValue);
                }
            }
        }
    }

    for(typename Image_t<double>::iterator it = tmpImg2->begin(); it < tmpImg2->end(); ++it) {
        if(*it > 255.) *it = 255.;
        if(*it < 0.) *it = 0.;
    }

   Image_t<D>* resImg = Converter<Image_t<D> >::convert(*tmpImg2);
   delete tmpImg;
   delete tmpImg2;
   return resImg;
}

template<typename D>
void ScalingOp::Nearest<D>::interpolate(typename Image_t<D>::ConstLine src, typename Image_t<double>::Line dst) {
    double scale = (double)dst.size() / src.size();
    for(int i = 0; i < src.size(); ++i) {
        for(int j = i*scale; j < (i+1)*scale; ++j) {
            dst[j] = src[i];
        }
    }
}

template<typename D>
void ScalingOp::Bilinear<D>::interpolate(typename Image_t<D>::ConstLine src, typename Image_t<double>::Line dst) {
    double scale = (double)dst.size() / src.size();
    int offset = floor( scale / 2 );
    for(int i = 0; i < offset; ++i) {
        dst[i] = src[0];
    }
    for(int i = 0; i < (src.size() - 1); ++i) {
        const D vl = src[i];
        const D vr = src[i + 1];
        int n = floor((i+1)*scale) - floor(i*scale);
        double dist = (double)(vr - vl) / n;
        double value = vl;
        for(int j = 0; j < n; ++j) {
            dst[i*scale + j + offset] = value;
            value += dist;
        }
    }
    for(int i = (src.size()-1)*scale + offset; i < dst.size(); ++i) {
        dst[i] = src[src.size()-1];
    }
}

template<typename D>
void ScalingOp::Parabolic<D>::interpolate(typename Image_t<D>::ConstLine src, typename Image_t<double>::Line dst) {

    double scale = (double)dst.size() / src.size();
    for(int i = 0; i < src.size(); ++i) {
        const D f1 = src[(i > 0) ? i - 1 : i];
        const D f2 = src[i];
        const D f3 = src[(i < (src.size() - 1)) ? i + 1 : i];
        int n = floor((i+1)*scale) - floor(i*scale);
        for(int j = 0; j < n; ++j) {
            double a = f1 + f3 - 2 * f2;
            double b = f3 - f1;
            const double p = (double)j / n -0.5;
            dst[i*scale + j] = f2 + p * ( b / 2. + p * a / 2.);
        }
    }
}

template<typename D>
void ScalingOp::Spline<D>::interpolate(typename Image_t<D>::ConstLine src, typename Image_t<double>::Line dst) {

    double scale = (double)dst.size() / src.size();
    int offset = floor( scale / 2 );
    {
        const D f0 = src[0];
        const D fm1 = src[0];
        const D fm2 = src[0];
        const D fp1 = src[1];
        const D fp2 = (src.size() > 1) ? src[2] : fp1;
        for(int j = 0; j < offset; ++j) {
            double c = 10. * f0 + 4. * (fp1 + fm1) - (fp2 +fm2);
            dst[j] = c / 16.;
        }
    }
    for(int i = 0; i < (src.size() - 1); ++i) {
        const D f0  = src[i];
        const D fm1 = (i > 0) ? src[i - 1] : f0;
        const D fm2 = (i > 1) ? src[i - 2] : fm1;
        const D fp1 = (i < (src.size() - 1)) ? src[i + 1] : f0;
        const D fp2 = (i < (src.size() - 2)) ? src[i + 2] : fp1;
        const D fp3 = (i < (src.size() - 3)) ? src[i + 3] : fp2;
        int n = floor((i+1)*scale) - floor(i*scale);
        for(int j = 0; j < n; ++j) {
            double a = 7. * (fp2 + fm1) - 6. * (fp1 + f0) - (fp3 + fm2);
            double b = 12. * (fp1 - fm1) - 2. * (fp2 - fm2);
            double c = 10. * f0 + 4. * (fp1 + fm1) - (fp2 +fm2);
            const double p = (double)j / n;
            dst[i*scale + j + offset] = (a * p * p + b * p + c ) / 16.;
        }
    }
    const D f0  = src[src.size() - 1];
    const D fm1 = src.size() > 1 ? src[src.size() - 2] : f0;
    const D fm2 = src.size() > 1 ? src[src.size() - 3] : fm1;
    const D fp1 = f0;
    const D fp2 = fp1;
    for(int j = (src.size()-1)*scale + offset; j < dst.size(); ++j) {
        double c = 10. * f0 + 4. * (fp1 + fm1) - (fp2 +fm2);
        dst[j] = c / 16.;
    }
}

void ScalingOp::setTest(bool a){
  _test = a;
}

void ScalingOp::setInterpolation(int a){
  _interp = a;
}

void ScalingOp::setXFactor(double a){
  _xfactor = a;
}

void ScalingOp::setYFactor(double a){
  _yfactor = a;
}
