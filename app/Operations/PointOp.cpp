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

#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QRadioButton>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QDebug>

#include "PointOp.h"
#include <Widgets/ImageListBox.h>
#include <Widgets/ImageWidgets/StandardImageWindow.h>
#include <Widgets/ImageWidgets/DoubleImageWindow.h>
#include <Converter.h>
#include "MyQLineEdit.h"
#include "../Tools.h"

using namespace std;
using namespace imagein;
using namespace genericinterface;

PointOp::PointOp() : GenericOperation(qApp->translate("Operations", "Pixel operations").toStdString()) {

}

PointOp::PixelOp* PointOp::PixelOp::fromString(const QString& op, const QString& expr) {
    if(op=="+") return new PixAdd(expr.toInt(nullptr,0));
    if(op=="-") return new PixAdd(-expr.toInt(nullptr,0));
    if(op=="*") return new PixMul(expr.toDouble());
    if(op=="/") return new PixMul(1/expr.toDouble());
    if(op=="& (bit-wise AND)") return new PixBitwiseAnd(expr.toUInt(nullptr,0));
    if(op=="&& (logical AND)") return new PixLogicalAnd(expr.toInt(nullptr,0));
    if(op=="! (bit-wise NOT)") return new PixLogicalNot(expr.toUInt(nullptr,0));
    if(op=="| (bit-wise OR)") return new PixBitwiseOr(expr.toUInt(nullptr,0));
    if(op=="|| (logical OR)") return new PixLogicalOr(expr.toUInt(nullptr,0));
    if(op=="^ (bit-wise XOR)") return new PixBitwiseXor(expr.toUInt(nullptr,0));
    if(op=="^^ (logical XOR)") return new PixLogicalXor(expr.toUInt(nullptr,0));
    if(op=="<<") return new PixLshift(expr.toUInt(nullptr,0));
    if(op==">>") return new PixRshift(expr.toUInt(nullptr,0));
    if(op=="") return new PixIdent();
    std::cout << "Unknown operator '" << op.toStdString() << "' ! PixelOp" << std::endl;
    return new PixIdent();
}
PointOp::DoublePixelOp* PointOp::DoublePixelOp::fromString(const QString& op, const QString& expr) {
    if(op=="+") return new DoublePixAdd(expr.toDouble());
    if(op=="-") return new DoublePixAdd(-expr.toDouble());
    if(op=="*") return new DoublePixMul(expr.toDouble());
    if(op=="/") return new DoublePixMul(1/expr.toDouble());
    if(op=="&& (logical AND)") return new DoublePixLogicalAnd(expr.toInt());
    if(op=="! (bit-wise NOT)") return new DoublePixLogicalNot(expr.toInt());
    if(op=="|| (logical OR)") return new DoublePixLogicalOr(expr.toInt());
    if(op=="^^ (logical XOR)") return new DoublePixLogicalXor(expr.toInt());
    if(op=="<<") return new DoublePixLshift(expr.toInt());
    if(op==">>") return new DoublePixRshift(expr.toInt());
    if(op=="") return new DoublePixIdent();
    std::cout << "Unknown operator '" << op.toStdString() << "' ! DoublePixelOp" << std::endl;
    return new DoublePixIdent();
}

PointOp::ImageOp* PointOp::ImageOp::fromString(const QString& op) {
    if(op=="+") return new ImgAdd();
    if(op=="-") return new ImgSub();
    if(op=="*") return new ImgMul();
    if(op=="/") return new ImgDiv();
    if(op=="& (bit-wise AND)") return new ImgBitwiseAnd();
    if(op=="&& (logical AND)") return new ImgLogicalAnd();
    if(op=="| (bit-wise OR)") return new ImgBitwiseOr();
    if(op=="|| (logical OR)") return new ImgLogicalOr();
    if(op=="^ (bit-wise XOR)") return new ImgBitwiseXor();
    if(op=="^^ (logical XOR)") return new ImgLogicalXor();
    if(op=="") return new ImgIdent();
    std::cout << "Unknown operator '" << op.toStdString() << "' ! ImageOp" << std::endl;
    return new ImgIdent();
}

PointOp::DoubleImageOp* PointOp::DoubleImageOp::fromString(const QString& op) {
    if(op=="+") return new DoubleImgAdd();
    if(op=="-") return new DoubleImgSub();
    if(op=="*") return new DoubleImgMul();
    if(op=="/") return new DoubleImgDiv();
    if(op=="&& (logical AND)") return new DoubleImgLogicalAnd();
    if(op=="|| (logical OR)") return new DoubleImgLogicalOr();
    if(op=="^^ (logical XOR)") return new DoubleImgLogicalXor();
    if(op=="") return new DoubleImgIdent();
    std::cout << "Unknown operator '" << op.toStdString() << "' ! DoubleImageOp" << std::endl;
    return new DoubleImgIdent();
}

void PointOp::operator()(const ImageWindow* currentWnd, const vector<const ImageWindow*>& wndList) {

    QStringList pixOperators, pixDoubleOperators, imgOperators, imgDoubleOperators;

    pixOperators << "" << "+" << "-" << "*" << "/" << qApp->translate("PointOp","! (bit-wise NOT)") << qApp->translate("PointOp","& (bit-wise AND)") << qApp->translate("PointOp","| (bit-wise OR)") << qApp->translate("PointOp","^ (bit-wise XOR)") << ">>" << "<<" << qApp->translate("PointOp","&& (logical AND)") << qApp->translate("PointOp","|| (logical OR)") << qApp->translate("PointOp","^^ (logical XOR)");
    pixDoubleOperators << "" << "+" << "-" << "*" << "/" << qApp->translate("PointOp","!! (logical NOT)") << qApp->translate("PointOp","&& (logical AND)") << qApp->translate("PointOp","|| (logical OR)") << qApp->translate("PointOp","^^ (logical XOR)");
    imgOperators << "" << "+" << "-" << "*" << "/" << qApp->translate("PointOp","& (bit-wise AND)") << qApp->translate("PointOp","| (bit-wise OR)") << qApp->translate("PointOp","^ (bit-wise XOR)") << qApp->translate("PointOp","&& (logical AND)") << qApp->translate("PointOp","|| (logical OR)") << qApp->translate("PointOp","^^ (logical XOR)");
    imgDoubleOperators << "" << "+" << "-" << "*" << "/" << qApp->translate("PointOp","&& (logical AND)") << qApp->translate("PointOp","|| (logical OR)") << qApp->translate("PointOp","^^ (logical XOR)");

    QString currentImgName = currentWnd->windowTitle();
    map<const Image*,string> stdImgList;
    map<const Image_t<double>*,string> dblImgList;
    for(auto it : wndList) {
        if(it->isStandard()) {
            const auto* stdImgWnd = dynamic_cast<const StandardImageWindow*>(it);
            stdImgList.insert(pair<const Image*, string>(stdImgWnd->getImage(), stdImgWnd->windowTitle().toStdString()));
        }
        else if(it->isDouble()) {
            const auto* dblImgWnd = dynamic_cast<const DoubleImageWindow*>(it);
            dblImgList.insert(pair<const Image_t<double>*, string>(dblImgWnd->getImage(), dblImgWnd->windowTitle().toStdString()));
        }
    }

    auto* dialog = new QDialog();
    dialog->setWindowTitle(qApp->translate("Operations", "Parameters"));
    dialog->setMinimumWidth(180);
    auto* layout = new QVBoxLayout();
    dialog->setLayout(layout);

    auto* radioGroup = new QGroupBox(qApp->translate("PointOp", "Second operand"), dialog);
    auto* valueButton = new QRadioButton(qApp->translate("PointOp", "Value"));
    valueButton->setWhatsThis(qApp->translate("PointOp", "Enter a real number in the blank space below as the second operand"));
    auto* imageButton = new QRadioButton(qApp->translate("PointOp", "Image"));
    imageButton->setWhatsThis(qApp->translate("PointOp", "Select an image as the second operand"));

    auto* radioGroup2 = new QGroupBox(qApp->translate("PointOp", "Output image"), dialog);
    auto* uCharButton = new QRadioButton(qApp->translate("PointOp", "UChar"));
    uCharButton->setWhatsThis(qApp->translate("PointOp", "Output the result image in uchar format"));
    auto* doubleButton = new QRadioButton(qApp->translate("PointOp", "Double"));
    doubleButton->setWhatsThis(qApp->translate("PointOp", "Output the result image in double format"));

    auto* radioLayout = new QHBoxLayout(radioGroup);
    radioLayout->addWidget(valueButton);
    radioLayout->addWidget(imageButton);

    auto* radioLayout2 = new QHBoxLayout(radioGroup2);
    radioLayout2->addWidget(uCharButton);
    radioLayout2->addWidget(doubleButton);

    layout->addWidget(radioGroup);
    layout->addWidget(radioGroup2);
    valueButton->setChecked(true);
    uCharButton->setChecked(true);

    //Si l'image est une image double le bouton est obligatoirement coché
    if(currentWnd->isDouble()){
        doubleButton->setChecked(true);
        doubleButton->setEnabled(false);
        uCharButton->setEnabled(false);
    }

    auto* checkbox = new QGroupBox(qApp->translate("PointOp","Options"));
    auto* optLayout = new QHBoxLayout();
    auto* gridLayout = new QGridLayout();
    auto* offsetBox = new QCheckBox(qApp->translate("PointOp","Offset"));
    offsetBox->setWhatsThis(qApp->translate("PointOp", "Add an offset of 127"));
    auto* scalingBox = new QCheckBox(qApp->translate("PointOp","Scaling"));
    scalingBox->setWhatsThis(qApp->translate("PointOp", "Map the value of each pixel to the range of 0-255 proportionally"));
    auto* colorBox = new QCheckBox(qApp->translate("PointOp", "Explode colors"));
    colorBox->setWhatsThis(qApp->translate("PointOp", "Check this option to execute the manipulation by channel"));
    offsetBox->setAutoExclusive(false);
    scalingBox->setAutoExclusive(false);
    offsetBox->setEnabled(!doubleButton->isChecked());
    scalingBox->setEnabled(!doubleButton->isChecked());
    offsetBox->setChecked(doubleButton->isChecked());
    scalingBox->setChecked(doubleButton->isChecked());
    colorBox->setAutoExclusive(false);
    optLayout->addWidget(offsetBox);
    optLayout->addWidget(scalingBox);

    gridLayout->addLayout(optLayout,1,1);
    gridLayout->addWidget(colorBox,2,1);

    checkbox->setLayout(gridLayout);
    layout->addWidget(checkbox);

    int nChannel = (int)currentWnd->getDisplayImage()->getNbChannels();

    auto** valueLayouts = new QHBoxLayout*[nChannel+1];
    auto** pixOperatorBoxes = new QComboBox*[nChannel+1];
    auto** imgOperatorBoxes = new QComboBox*[nChannel+1];
    auto** exprEdits = new MyQLineEdit*[nChannel+1];
    auto** imageBoxes = new MixImageListBox*[nChannel+1];

    auto* pixelWidget = new QWidget(dialog);
    auto* imgWidget = new QWidget(dialog);
    valueLayouts[0] = new QHBoxLayout();
    pixOperatorBoxes[0] = new QComboBox(pixelWidget);
    imgOperatorBoxes[0] = new QComboBox(imgWidget);

    if(currentWnd->isStandard()){
        pixOperatorBoxes[0]->addItems(pixOperators);
        imgOperatorBoxes[0]->addItems(imgOperators);
    }else{
        pixOperatorBoxes[0]->addItems(pixDoubleOperators);
        imgOperatorBoxes[0]->addItems(imgDoubleOperators);
    }

    exprEdits[0] = new MyQLineEdit(pixelWidget);
    exprEdits[0]->setFixedWidth(64);
/*************************************************************/
/*Overwrite QLineEdit class, send signals according to the change of combobox content to determine the corresponding supported input data type*/
    QObject::connect(pixOperatorBoxes[0], SIGNAL(currentTextChanged(QString)), exprEdits[0], SLOT(updateValidator(QString)));
    QObject::connect(imgOperatorBoxes[0], SIGNAL(currentTextChanged(QString)), exprEdits[0], SLOT(updateValidator(QString)));
/*************************************************************/
    imageBoxes[0] = new MixImageListBox(pixelWidget, currentImgName.toStdString(), stdImgList, dblImgList);
    valueLayouts[0]->addWidget(new QLabel(currentImgName, pixelWidget));
    valueLayouts[0]->addWidget(pixOperatorBoxes[0]);
    valueLayouts[0]->addWidget(imgOperatorBoxes[0]);
    valueLayouts[0]->addWidget(exprEdits[0]);
    valueLayouts[0]->addWidget(imageBoxes[0]);
    imageBoxes[0]->setVisible(false);
    imgOperatorBoxes[0]->setVisible(false);
    pixelWidget->setLayout(valueLayouts[0]);
    layout->addWidget(pixelWidget);

    auto* colorWidget = new QWidget(dialog);
    colorWidget->setLayout(new QVBoxLayout());
    for(int i=1; i <= nChannel; ++i) {
        valueLayouts[i] = new QHBoxLayout();
        pixOperatorBoxes[i] = new QComboBox(colorWidget);
        imgOperatorBoxes[i] = new QComboBox(colorWidget);
        if(currentWnd->isStandard()){
            pixOperatorBoxes[i]->addItems(pixOperators);
            imgOperatorBoxes[i]->addItems(imgOperators);
        }else{
            pixOperatorBoxes[i]->addItems(pixDoubleOperators);
            imgOperatorBoxes[i]->addItems(imgDoubleOperators);
        }
        exprEdits[i] = new MyQLineEdit(colorWidget);
        exprEdits[i]->setFixedWidth(64);
        QObject::connect(pixOperatorBoxes[i], SIGNAL(currentTextChanged(QString)), exprEdits[i], SLOT(updateValidator(QString)));
        QObject::connect(imgOperatorBoxes[i], SIGNAL(currentTextChanged(QString)), exprEdits[i], SLOT(updateValidator(QString)));
        imageBoxes[i] = new MixImageListBox(colorWidget, currentImgName.toStdString(), stdImgList, dblImgList);
        valueLayouts[i]->addWidget(new QLabel(currentImgName+"::"+Tools::colorName(i-1, nChannel), colorWidget));
        valueLayouts[i]->addWidget(pixOperatorBoxes[i]);
        valueLayouts[i]->addWidget(imgOperatorBoxes[i]);
        valueLayouts[i]->addWidget(exprEdits[i]);
        valueLayouts[i]->addWidget(imageBoxes[i]);
        imageBoxes[i]->setVisible(false);
        imgOperatorBoxes[i]->setVisible(false);
        colorWidget->layout()->addItem(valueLayouts[i]);
    }
    colorWidget->setVisible(false);
    layout->addWidget(colorWidget);

    for(int i=0; i<=nChannel; ++i) {
        QObject::connect(valueButton, SIGNAL(toggled(bool)), exprEdits[i], SLOT(setVisible(bool)));
        QObject::connect(valueButton, SIGNAL(toggled(bool)), pixOperatorBoxes[i], SLOT(setVisible(bool)));
        QObject::connect(imageButton, SIGNAL(toggled(bool)), imageBoxes[i], SLOT(setVisible(bool)));
        QObject::connect(imageButton, SIGNAL(toggled(bool)), imgOperatorBoxes[i], SLOT(setVisible(bool)));
    }
    QObject::connect(colorBox, SIGNAL(toggled(bool)), pixelWidget, SLOT(setHidden(bool)));
    QObject::connect(colorBox, SIGNAL(toggled(bool)), colorWidget, SLOT(setVisible(bool)));
    QObject::connect(uCharButton, SIGNAL(toggled(bool)), offsetBox, SLOT(setEnabled(bool)));
    QObject::connect(uCharButton, SIGNAL(toggled(bool)), scalingBox, SLOT(setEnabled(bool)));
    QObject::connect(doubleButton, SIGNAL(toggled(bool)), offsetBox, SLOT(setChecked(bool)));
    QObject::connect(doubleButton, SIGNAL(toggled(bool)), scalingBox, SLOT(setChecked(bool)));

    layout->setSizeConstraint(QLayout::SetFixedSize);

    auto *okButton = new QPushButton(qApp->translate("Operations", "Validate"), dialog);
    okButton->setDefault(true);
    layout->addWidget(okButton);
    QObject::connect(okButton, SIGNAL(clicked()), dialog, SLOT(accept()));
    
    auto code = static_cast<QDialog::DialogCode>(dialog->exec());
    
    if(code!=QDialog::Accepted) {
        return;
    }

    bool dblResult = currentWnd->isDouble() || doubleButton->isChecked();
    if(!valueButton->isChecked()) {
        if(!colorBox->isChecked()) {
            dblResult = dblResult || (imageBoxes[0]->currentType() == MixImageListBox::DBLIMG);
        }
        else {
            for(int i=0; i<nChannel; ++i) {
                dblResult = dblResult || (imageBoxes[i+1]->currentType() == MixImageListBox::DBLIMG);
            }
        }
    }

    const auto* currentDblWnd = dynamic_cast<const DoubleImageWindow*>(currentWnd);
    const Image_t<double>* image = currentDblWnd ? Converter<Image_t<double> >::convert(*currentDblWnd->getImage()) : Converter<Image_t<double> >::convert(*currentWnd->getDisplayImage());
    unsigned int maxWidth = image->getWidth();
    unsigned int maxHeight = image->getHeight();
    unsigned int maxChannel = image->getNbChannels();

    Image_t<double>* resDoubleImg;

    if(valueButton->isChecked()) {
        auto** pixelOps = new PixelOp*[nChannel];
        auto** doublePixelOps = new DoublePixelOp*[nChannel];
        if(!colorBox->isChecked()) {
            QString expr = exprEdits[0]->text();
            PixelOp* pixelOp = PixelOp::fromString(pixOperatorBoxes[0]->currentText(), expr);
            DoublePixelOp* doublePixelOp = DoublePixelOp::fromString(pixOperatorBoxes[0]->currentText(), expr);
            for(int i=0; i<nChannel; ++i){
                pixelOps[i] = pixelOp;
                doublePixelOps[i] = doublePixelOp;
            }
        }
        else {
            for(int i=0; i<nChannel; ++i) {
                QString expr = exprEdits[i+1]->text();
                pixelOps[i] = PixelOp::fromString(imgOperatorBoxes[i+1]->currentText(), expr);
                doublePixelOps[i] = DoublePixelOp::fromString(imgOperatorBoxes[i+1]->currentText(), expr);
            }
        }

        resDoubleImg = new Image_t<double>(image->getWidth(), image->getHeight(), nChannel);
        for(int c = 0; c < nChannel; ++c) {
            for(unsigned int j = 0; j < image->getHeight(); ++j) {
                for(unsigned int i = 0; i < image->getWidth(); ++i) {
                    if(dblResult){
                        double value1 = image->getPixel(i, j, c);
                        double value2 = doublePixelOps[c]->operator()(value1);
                        resDoubleImg->setPixel(i, j, c, value2);
                    }else{
                        int value1 = (int)image->getPixel(i, j, c);
                        int value2 = pixelOps[c]->operator()(value1);
                        resDoubleImg->setPixel(i, j, c, value2);
                    }
                }
            }
        }
    }
    else {
        /*The double image process exist already, but cases of standard images is ignored*/
        auto** stdImageOps = new ImageOp*[nChannel];
        auto** dblImageOps = new DoubleImageOp*[nChannel];

        bool isDblImg[nChannel];
        const Image_t<double>* dblImageImgs[nChannel];
        const Image* stdImageImgs[nChannel];
        if(!colorBox->isChecked()) {
            DoubleImageOp* dblImageOp = DoubleImageOp::fromString(imgOperatorBoxes[0]->currentText());
            ImageOp* stdImageOp = ImageOp::fromString(imgOperatorBoxes[0]->currentText());
            for(int i=0; i<nChannel; ++i){
                stdImageOps[i] = stdImageOp;
                dblImageOps[i] = dblImageOp;
            }
            if(imageBoxes[0]->currentType() == MixImageListBox::DBLIMG) {
                const Image_t<double>* dblImg = imageBoxes[0]->getDblImage(imageBoxes[0]->currentText().toStdString());
                for(int i=0; i<nChannel; ++i) {
                    dblImageImgs[i] = dblImg;
                    isDblImg[i] = true;
                }
                maxWidth = min(maxWidth, dblImg->getWidth());
                maxHeight = min(maxHeight, dblImg->getHeight());
                maxChannel = min(maxChannel, dblImg->getNbChannels());
            }
            else {
                const Image* stdImg = imageBoxes[0]->getStdImage(imageBoxes[0]->currentText().toStdString());
                for(int i=0; i<nChannel; ++i) {
                    stdImageImgs[i] = stdImg;
                    isDblImg[i] = false;
                }
                maxWidth = min(maxWidth, stdImg->getWidth());
                maxHeight = min(maxHeight, stdImg->getHeight());
                maxChannel = min(maxChannel, stdImg->getNbChannels());
            }
        }
        else {
            for(int i=0; i<nChannel; ++i) {
                stdImageOps[i] = ImageOp::fromString(imgOperatorBoxes[i+1]->currentText());
                dblImageOps[i] = DoubleImageOp::fromString(imgOperatorBoxes[i+1]->currentText());
                if(imageBoxes[i+1]->currentType() == MixImageListBox::DBLIMG) {
                    const Image_t<double>* dblImg = imageBoxes[i+1]->getDblImage(imageBoxes[i+1]->currentText().toStdString());
                    for(int j=0; i<nChannel; ++i) {
                        dblImageImgs[i] = dblImg;
                        isDblImg[i] = true;
                    }
                    maxWidth = min(maxWidth, dblImg->getWidth());
                    maxHeight = min(maxHeight, dblImg->getHeight());
                    maxChannel = min(maxChannel, dblImg->getNbChannels());
                }
                else {
                    const Image* stdImg = imageBoxes[i+1]->getStdImage(imageBoxes[i+1]->currentText().toStdString());
                    for(int j=0; i<nChannel; ++i) {
                        stdImageImgs[i] = stdImg;
                        isDblImg[i] = false;
                    }
                    maxWidth = min(maxWidth, stdImg->getWidth());
                    maxHeight = min(maxHeight, stdImg->getHeight());
                    maxChannel = min(maxChannel, stdImg->getNbChannels());
                }
            }
        }


        bool isChannelMismatch = image->getNbChannels()!=maxChannel;
        bool isChannelMismatch2;

        bool isHeightMismatch = image->getHeight() != maxHeight;
        bool isHeightMismatch2;

        bool isWidthMismatch = image->getWidth() != maxWidth;
        bool isWidthMismatch2;

        if (isDblImg[0]) {
            isChannelMismatch2 = maxChannel != dblImageImgs[0]->getNbChannels();
            isHeightMismatch2 = maxHeight != dblImageImgs[0]->getHeight();
            isWidthMismatch2 =  maxWidth != dblImageImgs[0]->getWidth();
        }
        else {
            isChannelMismatch2 = maxChannel != stdImageImgs[0]->getNbChannels();
            isHeightMismatch2 = maxHeight != stdImageImgs[0]->getHeight();
            isWidthMismatch2 = maxWidth != stdImageImgs[0]->getWidth();
        }

        if(isChannelMismatch || isChannelMismatch2) {
            QMessageBox::information(nullptr, qApp->translate("PointOp", "warning pixel operation on images"),
                                     qApp->translate("PointOp",
                                                     "You have performed an operation between a grayscale image (dimension 1) and a color image (dimension 3)"));

            string channelMismatchMessage = qApp->translate("PointOp", "The color image has been transformed to grayscale \n").toStdString();
            this->outText(channelMismatchMessage);
        }


        if (isHeightMismatch || isWidthMismatch || isWidthMismatch2 || isHeightMismatch2) {
            QMessageBox::information(nullptr, qApp->translate("PointOp", "warning pixel operation on images"),
                                     qApp->translate("PointOp", "You have performed an operation on 2 images of different dimensions"));

            string sizeMismatchMessage = qApp->translate("PointOp", "Larger image cropped from top left corner \n").toStdString();
            this->outText(sizeMismatchMessage);
        }

        auto* imageToLevelOfGrey = new Image_t<double>(maxWidth, maxHeight, maxChannel);
        auto* imageToLevelOfGrey2 = new Image_t<double>(maxWidth, maxHeight, maxChannel);

        /*L'image opérande de gauche est en couleur et celle de droite est en niveau de gris*/
        /*On fait une conversion de l'opérande de gauche en niveau de gris*/
        if(isChannelMismatch)
            imageToLevelOfGrey = Converter<GrayscaleImage_t<double>>::convert(*image);

            /*L'image opérande de droite est en couleur et celle de gauche est en niveau de gris et l'image est de type uchar*/
            /*On fait une conversion de l'opérande de droite en niveau de gris*/
        else if (isChannelMismatch2 && !isDblImg[0]) {
            auto* temp = new Image_t<double>(maxWidth, maxHeight,maxChannel);
            temp = Converter<Image_t<double>>::convert(*stdImageImgs[0]);   //Conversion de Uchar en double
            imageToLevelOfGrey2 = Converter<GrayscaleImage_t<double>>::convert(*temp);
        }

            /*L'image opérande de droite est en couleur et celle de gauche est en niveau de gris et l'image est de type double*/
            /*On fait une conversion de l'opérande de droite en niveau de gris*/
        else if (isChannelMismatch2 && isDblImg[0])
            imageToLevelOfGrey2 = Converter<GrayscaleImage_t<double>>::convert(*dblImageImgs[0]);


        resDoubleImg = new Image_t<double>(maxWidth, maxHeight, maxChannel);
        for(int c = 0; c < resDoubleImg->getNbChannels(); ++c) {
            for(unsigned int j = 0; j < resDoubleImg->getHeight(); ++j) {
                for(unsigned int i = 0; i < resDoubleImg->getWidth(); ++i) {

                    /*Si image est en couleur et dblImageImgs est en NDG, on utilise la conversion de cette première en NDG*/
                    double value1 = (nChannel==maxChannel) ? image->getPixel(i, j, c):imageToLevelOfGrey->getPixel(i, j, 0);
                    double value2,resValue;

                    if(isDblImg[c]) {
                        /*Si image est en NDG et dblImageImgs est en couleur, on utilise la conversion de cette dernière en NDG*/
                        if (isChannelMismatch2)
                            value2 = imageToLevelOfGrey2->getPixel(i, j, 0);
                        else
                            value2 = dblImageImgs[c]->getPixel(i, j, c);
                    }
                    else {  //Ici, les deux images sont de type uchar
                        if (isChannelMismatch2)
                            value2 = imageToLevelOfGrey2->getPixel(i, j, 0);
                        else
                            value2 = stdImageImgs[c]->getPixel(i, j, c);
                    }
//                L'utilisateur choisit si la sortie est de type Uchar ou double via un bouton
                    if(doubleButton->isChecked())
                        resValue = dblImageOps[c]->operator()(value1, value2);
                    else
                        resValue = stdImageOps[c]->operator()((int)value1, (int)value2);

                    resDoubleImg->setPixel(i, j, c, resValue);
                }
            }
        }
    }

    if(dblResult) {
        if(currentDblWnd) {
            this->outDoubleImage(resDoubleImg, "", AUTO, AUTO, currentDblWnd->isLogScaled());
        }
        else {
            this->outDoubleImage(resDoubleImg, "", AUTO, AUTO, false);
        }
        delete image;
    }
    else {
        Image* charResImg;
        bool _scaling = scalingBox->isChecked();
        bool _offset = offsetBox->isChecked();
        std::string outputMessage;

        //Toutes les opérations sont réalisées sur des doubles.
        //Si le résultat demandé n'est pas de type double, on fait une conversion en int à la fin
        Image_t<int>* intResImg = Converter<Image_t<int> >::convert(*resDoubleImg);
        if(_scaling && _offset ){
            charResImg  =  Converter<Image>::convertScaleAndOffset(*intResImg, &outputMessage);
        }
        else if(_scaling){
            charResImg =  Converter<Image>::convertAndScale(*intResImg, &outputMessage);
        }
        else if(_offset){
            charResImg = Converter<Image>::convertAndOffset(*intResImg, &outputMessage);
        }
        else{
            charResImg = Converter<Image>::convert(*resDoubleImg);
            outputMessage = qApp->translate("Operations","Pas de conversion [min : 0, max : 255]").toStdString();
        }
        this->outText(outputMessage);
        this->outText("-------------------------------------------");

        this->outImage(charResImg);
    }
}

bool PointOp::needCurrentImg() const {
    return true;
}

bool PointOp::isValidImgWnd(const genericinterface::ImageWindow* imgWnd) const {
    return imgWnd != nullptr;
}
