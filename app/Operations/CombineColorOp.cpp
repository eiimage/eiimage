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

#include "CombineColorOp.h"
#include "../Tools.h"

using namespace std;
using namespace imagein;

CombineColorOp::CombineColorOp() : Operation(qApp->translate("Operations", "Combine color planes").toStdString())
{
}

bool CombineColorOp::needCurrentImg() const {
    return false;
}

void CombineColorOp::operator()(const imagein::Image*, const std::map<const imagein::Image*, std::string>& imgList) {

    QDialog* dialog = new QDialog();
    dialog->setWindowTitle(qApp->translate("Operations", "Parameters"));
    dialog->setMinimumWidth(180);
    QFormLayout* layout = new QFormLayout();
    dialog->setLayout(layout);


    unsigned int nChannel = 3;
    ImageListBox** imageBoxes = new ImageListBox*[nChannel];

    for(unsigned int i=0; i < nChannel; ++i) {
        imageBoxes[i] = new ImageListBox(dialog, NULL, imgList);
        QLabel* label = new QLabel(Tools::colorName(i, nChannel), dialog);
        layout->insertRow(i, label, imageBoxes[i]);
    }

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, Qt::Horizontal, dialog);
    layout->insertRow(nChannel, buttonBox);
    QObject::connect(buttonBox, SIGNAL(accepted()), dialog, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), dialog, SLOT(reject()));

    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog->exec());

    if(code!=QDialog::Accepted) {
        return;
    }

    GrayscaleImage* channels[nChannel];
    unsigned int maxWidth = numeric_limits<unsigned int>::max();
    unsigned int maxHeight = numeric_limits<unsigned int>::max();
    for(unsigned int c = 0; c < nChannel; ++c) {
        const Image* img = imageBoxes[c]->currentImage();
        if(img == NULL) return;
        channels[c] = Converter<GrayscaleImage>::convert(*img);
        maxWidth = min(maxWidth, channels[c]->getWidth());
        maxHeight = min(maxHeight, channels[c]->getHeight());
    }

    Image* resImg = new Image(maxWidth, maxHeight, nChannel);

    for(unsigned int c = 0; c < nChannel; ++c) {
        for(unsigned int j = 0; j < resImg->getHeight(); ++j) {
            for(unsigned int i = 0; i < resImg->getWidth(); ++i) {
                resImg->setPixel(i, j, c, channels[c]->getPixel(i, j));
            }
        }
    }
    this->outImage(resImg, qApp->translate("CombineColorOp", "Reconstructed Color image").toStdString());
}
