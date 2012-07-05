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

#include "PixelOperation.h"
#include "ImgWidget.h"

using namespace std;
using namespace imagein;

PixelOperation::PixelOperation() : Operation("Pixel operations") {
    
}

PixelOperation::PixelOp* PixelOperation::PixelOp::fromString(QString op, QString expr) {
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

PixelOperation::ImageOp* PixelOperation::ImageOp::fromString(QString op) {
    if(op=="+") return new ImgAdd();
    if(op=="-") return new ImgSub();
    if(op=="&") return new ImgAnd();
    if(op=="|") return new ImgOr();
    if(op=="^") return new ImgXor();
    if(op=="") return new ImgIdent();
    std::cout << "Unknown operator '" << op.toStdString() << "' !" << std::endl;
    return new ImgIdent();
}

vector<QWidget*> PixelOperation::operator()(const imagein::Image* image, const std::map<std::string, const imagein::Image*>& imgList) {
    vector<QWidget*> result;
    QStringList pixOperators, imgOperators;
    pixOperators << "" << "+" << "-" << "*" << "/" << "&" << "|" << "^" << "<<" << ">>";
    imgOperators << "" << "+" << "-" << "&" << "|" << "^";
    QStringList imageNames;
    for(map<string, const Image*>::const_iterator it = imgList.begin(); it != imgList.end(); ++it) {
        imageNames << it->first.c_str();
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

    int nChannel = image->getNbChannels();
    QHBoxLayout** valueLayouts = new QHBoxLayout*[nChannel+1];
    QComboBox** pixOperatorBoxes = new QComboBox*[nChannel+1];
    QComboBox** imgOperatorBoxes = new QComboBox*[nChannel+1];
    QLineEdit** exprEdits = new QLineEdit*[nChannel+1];
    QComboBox** imageBoxes = new QComboBox*[nChannel+1];

    QWidget* pixelWidget = new QWidget(dialog);
    valueLayouts[0] = new QHBoxLayout();
    pixOperatorBoxes[0] = new QComboBox(pixelWidget);
    imgOperatorBoxes[0] = new QComboBox(pixelWidget);
    pixOperatorBoxes[0]->addItems(pixOperators); 
    imgOperatorBoxes[0]->addItems(imgOperators); 
    exprEdits[0] = new QLineEdit(pixelWidget);
    exprEdits[0]->setFixedWidth(64);
    imageBoxes[0] = new QComboBox(pixelWidget);
    imageBoxes[0]->addItems(imageNames);
    valueLayouts[0]->addWidget(new QLabel("Image", pixelWidget));
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
        imageBoxes[i] = new QComboBox(colorWidget);
        imageBoxes[i]->addItems(imageNames);
        valueLayouts[i]->addWidget(new QLabel(colorName(i-1, nChannel), colorWidget));
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
        return result;
    }

    PixelOp** pixelOps = new PixelOp*[nChannel];
    ImageOp** imageOps = new ImageOp*[nChannel];
    const Image**  imageImgs = new const Image*[nChannel];
    unsigned int maxWidth = image->getWidth();
    unsigned int maxHeight = image->getHeight();
    
    if(!colorBox->isChecked()) {
        QString expr = exprEdits[0]->text();
        PixelOp* pixelOp = PixelOp::fromString(pixOperatorBoxes[0]->currentText(), expr);
        ImageOp* imageOp = ImageOp::fromString(imgOperatorBoxes[0]->currentText());
        const Image* imageImg = imgList.find(imageBoxes[0]->currentText().toStdString())->second;
        maxWidth = min(maxWidth, imageImg->getWidth());
        maxHeight = min(maxHeight, imageImg->getHeight());
        
        for(int i=0; i<nChannel; ++i) {
            pixelOps[i] = pixelOp;
            imageOps[i] = imageOp;
            imageImgs[i] = imageImg;
        }
    }
    else {
        for(int i=0; i<nChannel; ++i) {
            QString expr = exprEdits[i+1]->text();
            pixelOps[i] = PixelOp::fromString(pixOperatorBoxes[i+1]->currentText(), expr);
            imageOps[i] = ImageOp::fromString(imgOperatorBoxes[i+1]->currentText());
            imageImgs[i] = imgList.find(imageBoxes[i+1]->currentText().toStdString())->second;
            maxWidth = min(maxWidth, imageImgs[i]->getWidth());
            maxHeight = min(maxHeight, imageImgs[i]->getHeight());
        }
    }

    Image* resImg;
    
    if(valueButton->isChecked()) {
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
    result.push_back(new ImgWidget(resImg, ""));
        
    return result;
}

bool PixelOperation::needCurrentImg() {
    return true;
}
