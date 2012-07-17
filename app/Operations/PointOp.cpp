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

#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QRadioButton>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QGroupBox>

#include "PointOp.h"
#include "ImgWidget.h"
#include "ImageListBox.h"
#include "Widgets/ImageWidgets/StandardImageWindow.h"
#include "Widgets/ImageWidgets/DoubleImageWindow.h"

#include "../Tools.h"

using namespace std;
using namespace imagein;
using namespace genericinterface;

PointOp::PointOp() : GenericOperation(Tools::tr("Pixel operations").toStdString()) {
    
}

PointOp::PixelOp* PointOp::PixelOp::fromString(QString op, QString expr) {
    if(op=="+") return new PixAdd(expr.toInt(0,0));
    if(op=="-") return new PixAdd(-expr.toInt(0,0));
    if(op=="*") return new PixMul(expr.toDouble());
    if(op=="/") return new PixMul(1/expr.toDouble());
    if(op=="&") return new PixAnd(expr.toUInt(0,0));
    if(op=="|") return new PixOr(expr.toUInt(0,0));
    if(op=="^") return new PixXor(expr.toUInt(0,0));
    if(op=="<<") return new PixLshift(expr.toUInt(0,0));
    if(op==">>") return new PixRshift(expr.toUInt(0,0));
    if(op=="") return new PixIdent();
    std::cout << "Unknown operator '" << op.toStdString() << "' !" << std::endl;
    return new PixIdent();
}
PointOp::DoublePixelOp* PointOp::DoublePixelOp::fromString(QString op, QString expr) {
    if(op=="+") return new DoublePixAdd(expr.toDouble());
    if(op=="-") return new DoublePixAdd(-expr.toDouble());
    if(op=="*") return new DoublePixMul(expr.toDouble());
    if(op=="/") return new DoublePixMul(1/expr.toDouble());
    if(op=="") return new DoublePixIdent();
    std::cout << "Unknown operator '" << op.toStdString() << "' !" << std::endl;
    return new DoublePixIdent();
}

PointOp::ImageOp* PointOp::ImageOp::fromString(QString op) {
    if(op=="+") return new ImgAdd();
    if(op=="-") return new ImgSub();
    if(op=="&") return new ImgAnd();
    if(op=="|") return new ImgOr();
    if(op=="^") return new ImgXor();
    if(op=="") return new ImgIdent();
    std::cout << "Unknown operator '" << op.toStdString() << "' !" << std::endl;
    return new ImgIdent();
}
PointOp::DoubleImageOp* PointOp::DoubleImageOp::fromString(QString op) {
    if(op=="+") return new DoubleImgAdd();
    if(op=="-") return new DoubleImgSub();
    if(op=="*") return new DoubleImgMul();
    if(op=="/") return new DoubleImgDiv();
    if(op=="") return new DoubleImgIdent();
    std::cout << "Unknown operator '" << op.toStdString() << "' !" << std::endl;
    return new DoubleImgIdent();
}

void PointOp::operator()(const ImageWindow* currentWnd, vector<ImageWindow*>& wndList) {

    QStringList pixOperators, imgOperators;

    pixOperators << "" << "+" << "-" << "*" << "/" << "&" << "|" << "^" << "<<" << ">>";
    imgOperators << "" << "+" << "-" << "*" << "/" << "&" << "|" << "^";
    QString currentImgName = currentWnd->windowTitle();
    map<const Image*,string> stdImgList;
    map<const Image_t<double>*,string> dblImgList;
    for(vector<ImageWindow*>::iterator it = wndList.begin(); it != wndList.end(); ++it) {
        if((*it)->isStandard()) {
            const StandardImageWindow* stdImgWnd = dynamic_cast<const StandardImageWindow*>(*it);
            stdImgList.insert(pair<const Image*, string>(stdImgWnd->getImage(), stdImgWnd->windowTitle().toStdString()));
        }
        else if((*it)->isDouble()) {
            const DoubleImageWindow* dblImgWnd = dynamic_cast<const DoubleImageWindow*>(*it);
            dblImgList.insert(pair<const Image_t<double>*, string>(dblImgWnd->getImage(), dblImgWnd->windowTitle().toStdString()));
        }
    }

    
    QDialog* dialog = new QDialog();
    dialog->setWindowTitle(dialog->tr("Parameter"));
    dialog->setMinimumWidth(180);
    QVBoxLayout* layout = new QVBoxLayout();
    dialog->setLayout(layout);

    QGroupBox* radioGroup = new QGroupBox("Second operand", dialog);
    QRadioButton* valueButton = new QRadioButton(dialog->tr("Value"));
    QRadioButton* imageButton = new QRadioButton(dialog->tr("Image"));
    QHBoxLayout* radioLayout = new QHBoxLayout(radioGroup);
    radioLayout->addWidget(valueButton);
    radioLayout->addWidget(imageButton);
    layout->addWidget(radioGroup);
    valueButton->setChecked(true);

    QCheckBox* colorBox = new QCheckBox("Explode colors", dialog);
    layout->addWidget(colorBox);

    int nChannel = currentWnd->getDisplayImage()->getNbChannels();

    QHBoxLayout** valueLayouts = new QHBoxLayout*[nChannel+1];
    QComboBox** pixOperatorBoxes = new QComboBox*[nChannel+1];
    QComboBox** imgOperatorBoxes = new QComboBox*[nChannel+1];
    QLineEdit** exprEdits = new QLineEdit*[nChannel+1];
    MixImageListBox** imageBoxes = new MixImageListBox*[nChannel+1];

    QWidget* pixelWidget = new QWidget(dialog);
    valueLayouts[0] = new QHBoxLayout();
    pixOperatorBoxes[0] = new QComboBox(pixelWidget);
    imgOperatorBoxes[0] = new QComboBox(pixelWidget);
    pixOperatorBoxes[0]->addItems(pixOperators);
    imgOperatorBoxes[0]->addItems(imgOperators);
    exprEdits[0] = new QLineEdit(pixelWidget);
    exprEdits[0]->setFixedWidth(64);
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

    QWidget* colorWidget = new QWidget(dialog);
    colorWidget->setLayout(new QVBoxLayout());
    for(int i=1; i <= nChannel; ++i) {
        valueLayouts[i] = new QHBoxLayout();
        pixOperatorBoxes[i] = new QComboBox(colorWidget);
        imgOperatorBoxes[i] = new QComboBox(colorWidget);
        pixOperatorBoxes[i]->addItems(pixOperators);
        imgOperatorBoxes[i]->addItems(imgOperators);
        exprEdits[i] = new QLineEdit(colorWidget);
        exprEdits[i]->setFixedWidth(64);
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
    
    layout->setSizeConstraint(QLayout::SetFixedSize);
    
    QPushButton *okButton = new QPushButton(dialog->tr("Validate"), dialog);
    okButton->setDefault(true);
    layout->addWidget(okButton);
    QObject::connect(okButton, SIGNAL(clicked()), dialog, SLOT(accept()));
    
    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog->exec());
    
    if(code!=QDialog::Accepted) {
        return;
    }

    bool dblResult = currentWnd->isDouble();
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

    if(dblResult) {
        const DoubleImageWindow* currentDblWnd = dynamic_cast<const DoubleImageWindow*>(currentWnd);
        const Image_t<double>* image = currentDblWnd ? currentDblWnd->getImage() : NULL;
        unsigned int maxWidth = image->getWidth();
        unsigned int maxHeight = image->getHeight();


        Image_t<double>* resImg;

        if(valueButton->isChecked()) {
            DoublePixelOp** pixelOps = new DoublePixelOp*[nChannel];
            if(!colorBox->isChecked()) {
                QString expr = exprEdits[0]->text();
                DoublePixelOp* pixelOp = DoublePixelOp::fromString(pixOperatorBoxes[0]->currentText(), expr);
                for(int i=0; i<nChannel; ++i) pixelOps[i] = pixelOp;
            }
            else {
                for(int i=0; i<nChannel; ++i) {
                    QString expr = exprEdits[i+1]->text();
                    pixelOps[i] = DoublePixelOp::fromString(pixOperatorBoxes[i+1]->currentText(), expr);
                }
            }

            resImg = new Image_t<double>(image->getWidth(), image->getHeight(), nChannel);
            for(int c = 0; c < nChannel; ++c) {
                for(unsigned int j = 0; j < image->getHeight(); ++j) {
                    for(unsigned int i = 0; i < image->getWidth(); ++i) {
                        double value = image->getPixel(i, j, c);
                        value = pixelOps[c]->operator()(value);
                        resImg->setPixel(i, j, c, value);
                    }
                }
            }
        }
        else {
            DoubleImageOp** imageOps = new DoubleImageOp*[nChannel];
            bool isDblImg[nChannel];
            const Image_t<double>* dblImageImgs[nChannel];
            const Image* stdImageImgs[nChannel];
            if(!colorBox->isChecked()) {
                DoubleImageOp* imageOp = DoubleImageOp::fromString(imgOperatorBoxes[0]->currentText());
                for(int i=0; i<nChannel; ++i) imageOps[i] = imageOp;
                if(imageBoxes[0]->currentType() == MixImageListBox::DBLIMG) {
                    const Image_t<double>* imageImg = imageBoxes[0]->getDblImage(imageBoxes[0]->currentText().toStdString());
                    for(int i=0; i<nChannel; ++i) {
                        dblImageImgs[i] = imageImg;
                        isDblImg[i] = true;
                    }
                    maxWidth = min(maxWidth, imageImg->getWidth());
                    maxHeight = min(maxHeight, imageImg->getHeight());
                }
                else {
                    const Image* imageImg = imageBoxes[0]->getStdImage(imageBoxes[0]->currentText().toStdString());
                    for(int i=0; i<nChannel; ++i) {
                        stdImageImgs[i] = imageImg;
                        isDblImg[i] = false;
                    }
                    maxWidth = min(maxWidth, imageImg->getWidth());
                    maxHeight = min(maxHeight, imageImg->getHeight());
                }

            }
            else {
                for(int i=0; i<nChannel; ++i) {
                    imageOps[i] = DoubleImageOp::fromString(imgOperatorBoxes[i+1]->currentText());
                    if(imageBoxes[i+1]->currentType() == MixImageListBox::DBLIMG) {
                        const Image_t<double>* imageImg = imageBoxes[i+1]->getDblImage(imageBoxes[i+1]->currentText().toStdString());
                        for(int i=0; i<nChannel; ++i) {
                            dblImageImgs[i] = imageImg;
                            isDblImg[i] = true;
                        }
                        maxWidth = min(maxWidth, imageImg->getWidth());
                        maxHeight = min(maxHeight, imageImg->getHeight());
                    }
                    else {
                        const Image* imageImg = imageBoxes[i+1]->getStdImage(imageBoxes[i+1]->currentText().toStdString());
                        for(int i=0; i<nChannel; ++i) {
                            stdImageImgs[i] = imageImg;
                            isDblImg[i] = false;
                        }
                        maxWidth = min(maxWidth, imageImg->getWidth());
                        maxHeight = min(maxHeight, imageImg->getHeight());
                    }
                }
            }
            resImg = new Image_t<double>(maxWidth, maxHeight, nChannel);
            for(int c = 0; c < resImg->getNbChannels(); ++c) {
                for(unsigned int j = 0; j < resImg->getHeight(); ++j) {
                    for(unsigned int i = 0; i < resImg->getWidth(); ++i) {
                        double value1 = image->getPixel(i, j, c);
                        double value2;
                        if(isDblImg[c]) {
                            const unsigned int channel = (c < dblImageImgs[c]->getNbChannels() ? c : 0);
                            value2 = dblImageImgs[c]->getPixel(i, j, channel);
                        }
                        else {
                            const unsigned int channel = (c < stdImageImgs[c]->getNbChannels() ? c : 0);
                            value2 = stdImageImgs[c]->getPixel(i, j, channel);
                        }
                        value1 = imageOps[c]->operator()(value1, value2);
                        resImg->setPixel(i, j, c, value1);
                    }
                }
            }
        }

        this->outDoubleImage(resImg, "", currentDblWnd->isNormalized(), currentDblWnd->isLogScaled());
    }
    else {
        const Image* image = dynamic_cast<const StandardImageWindow*>(currentWnd)->getImage();
        unsigned int maxWidth = image->getWidth();
        unsigned int maxHeight = image->getHeight();


        Image* resImg;

        if(valueButton->isChecked()) {
            PixelOp** pixelOps = new PixelOp*[nChannel];
            if(!colorBox->isChecked()) {
                QString expr = exprEdits[0]->text();
                PixelOp* pixelOp = PixelOp::fromString(pixOperatorBoxes[0]->currentText(), expr);
                for(int i=0; i<nChannel; ++i) pixelOps[i] = pixelOp;
            }
            else {
                for(int i=0; i<nChannel; ++i) {
                    QString expr = exprEdits[i+1]->text();
                    pixelOps[i] = PixelOp::fromString(pixOperatorBoxes[i+1]->currentText(), expr);
                }
            }

            resImg = new Image(image->getWidth(), image->getHeight(), nChannel);
            for(int c = 0; c < nChannel; ++c) {
                for(unsigned int j = 0; j < image->getHeight(); ++j) {
                    for(unsigned int i = 0; i < image->getWidth(); ++i) {
                        Image::depth_t value = image->getPixel(i, j, c);
                        value = pixelOps[c]->operator()(value);
                        resImg->setPixel(i, j, c, value);
                    }
                }
            }
        }
        else {
            ImageOp** imageOps = new ImageOp*[nChannel];
            const Image**  imageImgs = new const Image*[nChannel];
            if(!colorBox->isChecked()) {
                ImageOp* imageOp = ImageOp::fromString(imgOperatorBoxes[0]->currentText());
                const Image* imageImg = imageBoxes[0]->getStdImage(imageBoxes[0]->currentText().toStdString());
                maxWidth = min(maxWidth, imageImg->getWidth());
                maxHeight = min(maxHeight, imageImg->getHeight());
                for(int i=0; i<nChannel; ++i) {
                    imageOps[i] = imageOp;
                    imageImgs[i] = imageImg;
                }
            }
            else {
                for(int i=0; i<nChannel; ++i) {
                    imageOps[i] = ImageOp::fromString(imgOperatorBoxes[i+1]->currentText());
                    imageImgs[i] = imageBoxes[i+1]->getStdImage(imageBoxes[i+1]->currentText().toStdString());
                    maxWidth = min(maxWidth, imageImgs[i]->getWidth());
                    maxHeight = min(maxHeight, imageImgs[i]->getHeight());
                }
            }
            resImg = new Image(maxWidth, maxHeight, nChannel);
            for(int c = 0; c < resImg->getNbChannels(); ++c) {
                for(unsigned int j = 0; j < resImg->getHeight(); ++j) {
                    for(unsigned int i = 0; i < resImg->getWidth(); ++i) {
                        Image::depth_t value1 = image->getPixel(i, j, c);
                        const unsigned int channel = (c < imageImgs[c]->getNbChannels() ? c : 0);
                        Image::depth_t value2 = imageImgs[c]->getPixel(i, j, channel);
                        value1 = imageOps[c]->operator()(value1, value2);
                        resImg->setPixel(i, j, c, value1);
                    }
                }
            }
        }

        this->outImage(resImg);
    }
}

bool PointOp::needCurrentImg() const {
    return true;
}

bool PointOp::isValidImgWnd(const genericinterface::ImageWindow*) const {
    return true;
}
