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

#include <limits>

#include <QLabel>
#include <QDialog>
#include <QFormLayout>
#include <QDialogButtonBox>

#include <GrayscaleImage.h>
#include <Converter.h>

#include <Widgets/ImageListBox.h>

#include "CombineHsvOp.h"
#include "../Tools.h"

using namespace std;
using namespace imagein;

CombineHSVOp::CombineHSVOp() : DoubleOperation(qApp->translate("Operations", "Combine HSV planes").toStdString())
{
}

bool CombineHSVOp::needCurrentImg() const {
    return false;
}

void CombineHSVOp::operator()(const imagein::Image_t<double>*, const map<const imagein::Image_t<double>*, string>& imgList) {

    QDialog* dialog = new QDialog();
    dialog->setWindowTitle(qApp->translate("Operations", "Parameters"));
    dialog->setMinimumWidth(180);
    QFormLayout* layout = new QFormLayout();
    dialog->setLayout(layout);

    ImageListBox_t<double>** imageBoxes = new ImageListBox_t<double>*[3*sizeof(ImageListBox_t<double>*)];

        imageBoxes[0] = new ImageListBox_t<double>(dialog, NULL, imgList);
        QLabel* label = new QLabel("Hue", dialog);
        layout->insertRow(0, label, imageBoxes[0]);

        imageBoxes[1] = new ImageListBox_t<double>(dialog, NULL, imgList);
        QLabel* label1 = new QLabel("Saturation", dialog);
        layout->insertRow(1, label1, imageBoxes[1]);

        imageBoxes[2] = new ImageListBox_t<double>(dialog, NULL, imgList);
        QLabel* label2 = new QLabel("Value", dialog);
        layout->insertRow(2, label2, imageBoxes[2]);


    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, Qt::Horizontal, dialog);
    layout->insertRow(3, buttonBox);
    QObject::connect(buttonBox, SIGNAL(accepted()), dialog, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), dialog, SLOT(reject()));

    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog->exec());

    if(code!=QDialog::Accepted) {
        return;
    }

    QColor color;
    int width = imageBoxes[0]->currentImage()->getWidth();
    int height = imageBoxes[0]->currentImage()->getHeight();

    Image* resImg = new Image(width, height, 3);
    const Image_t<double>* H = imageBoxes[0]->currentImage();
    const Image_t<double>* S = imageBoxes[1]->currentImage();
    const Image_t<double>* V = imageBoxes[2]->currentImage();


    for(int i=0;i<width;i++){
        for(int j=0;j<height;j++){
            color.setHsv(H->getPixel(i,j,0),S->getPixel(i,j,0) ,V->getPixel(i,j,0) );
            resImg->setPixel(i,j,0,color.red());
            resImg->setPixel(i,j,1,color.green());
            resImg->setPixel(i,j,2,color.blue());

        }
    }


    this->outImage(resImg, qApp->translate("CombineHsvOp", "Reconstructed  image").toStdString());
}
