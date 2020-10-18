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

#include "ColorimetryOp.h"
#include "../Tools.h"
#include <QDialog>
#include <QFormLayout>
#include <QComboBox>
#include <QMessageBox>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>
#include <QApplication>
#include <QSlider>
#include <QLabel>
#include <QColorDialog>
#include "ColorDialog.h"
using namespace imagein;

ColorimetryOp::ColorimetryOp() : Operation(qApp->translate("Operations", "Generate RGB/HSV image").toStdString())
{
}

bool ColorimetryOp::needCurrentImg() const {
    return false;
}

void ColorimetryOp::operator()(const imagein::Image*, const std::map<const imagein::Image*, std::string>&) {

    ColorDialog* dialog = new ColorDialog(QApplication::activeWindow());
    dialog->setWindowTitle(QString(qApp->translate("Operations", "RGB/HSV image generator")));
    QObject::connect(dialog, SIGNAL(needHint()), this, SLOT(giveHint()));

    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog->exec());
    QColor color = dialog->getColor();

    if(code!=QDialog::Accepted) return;

    /*Pop-up a warning window if the input value of width or height equals to 0*/
    if( dialog->getWidth()==0 || dialog->getHeight()==0 ){
            QMessageBox *msgBox;
            msgBox = new QMessageBox(QString(qApp->translate("ColorimetryOp","Warning")),
                QString(qApp->translate("ColorimetryOp", "Empty image generation is not allowed")),
                QMessageBox::Warning,
                QMessageBox::Ok | QMessageBox::Default,
                QMessageBox::NoRole | QMessageBox::Escape,
                0);
            msgBox->show();
            return;
    }

    Image* resImg = new Image(dialog->getWidth(), dialog->getHeight(), 3);
    for(unsigned int j = 0; j < resImg->getHeight(); ++j) {
        for(unsigned int i = 0; i < resImg->getWidth(); ++i) {
            resImg->setPixelAt(i, j, 0, color.red());
        }
    }
    for(unsigned int j = 0; j < resImg->getHeight(); ++j) {
        for(unsigned int i = 0; i < resImg->getWidth(); ++i) {
            resImg->setPixelAt(i, j, 1, color.green());
        }
    }
    for(unsigned int j = 0; j < resImg->getHeight(); ++j) {
        for(unsigned int i = 0; i < resImg->getWidth(); ++i) {
            resImg->setPixelAt(i, j, 2, color.blue());
        }
    }
    outImage(resImg, "RGB Image");
}

void ColorimetryOp::giveHint(){

    QDialog* hint = new QDialog(QApplication::activeWindow());
    hint->setWindowTitle(QString(qApp->translate("ColorDialog", "A small tip")));
    hint->setLayout(new QVBoxLayout);
    QLabel* pic = new QLabel(hint);
    pic->setPixmap(QPixmap(":/img/qcolor_hsv.png"));
    hint->resize(320,280);
    pic->resize(hint->size());
    hint->show();
    std::string outputMessage1 = qApp->translate("ColorimetryOp","H, for hue, is in the range 0 to 359. Red is 0 (degrees), green is 120, and blue is 240 as shown by the pop-up image.\n\n").toStdString();
    std::string outputMessage2 = qApp->translate("ColorimetryOp","S, for saturation, is in the range 0 to 255, and the bigger it is, the stronger the color is.\n\n").toStdString();
    std::string outputMessage3 = qApp->translate("ColorimetryOp","V, for value, is in the range 0 to 255 and represents lightness or brightness of the color, 0 represents for black.\n\n-------------------------------------------").toStdString();
    outText(outputMessage1+outputMessage2+outputMessage3);
}
