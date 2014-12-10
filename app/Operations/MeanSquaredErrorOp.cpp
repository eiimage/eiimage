/*
 * Copyright 2011-2012 INSA Rennes
 *
 * This file is part of INSAimage.
 *
 * INSAimage is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * INSAimage is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with INSAimage.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QLabel>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <Image.h>

#include <Widgets/ImageListBox.h>
#include "../Tools.h"

#include "MeanSquaredErrorOp.h"

using namespace std;
using namespace imagein;

MeanSquaredErrorOp::MeanSquaredErrorOp() : Operation(qApp->translate("Operations", "Mean squared error").toStdString())
{
}

bool MeanSquaredErrorOp::needCurrentImg() const {
    return true;
}

void MeanSquaredErrorOp::operator()(const imagein::Image* image, const std::map<const imagein::Image*, std::string>& imgList) {

    QDialog* dialog = new QDialog();
    dialog->setWindowTitle(qApp->translate("Operations", "Compare to..."));
    dialog->setMinimumWidth(180);
    QFormLayout* layout = new QFormLayout();
    dialog->setLayout(layout);

    QString currentImgName = QString(imgList.find(image)->second.c_str());

    ImageListBox* imageBox = new ImageListBox(dialog, image, imgList);
    layout->insertRow(0, qApp->translate("Operations", "Compare %1 to : ").arg(currentImgName), imageBox);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, Qt::Horizontal, dialog);
    layout->insertRow(1, buttonBox);
    QObject::connect(buttonBox, SIGNAL(accepted()), dialog, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), dialog, SLOT(reject()));

    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog->exec());

    if(code!=QDialog::Accepted) {
        return;
    }

    const Image* sndImg = imageBox->currentImage();
    unsigned int maxWidth = min(image->getWidth(), sndImg->getWidth());
    unsigned int maxHeight = min(image->getHeight(), sndImg->getHeight());
    unsigned int maxChannel = min(image->getNbChannels(), sndImg->getNbChannels());

    double mse = 0, me = 0;
    for(unsigned int c = 0; c < maxChannel; ++c) {
        for(unsigned int j = 0; j < maxHeight; ++j) {
            for(unsigned int i = 0; i < maxWidth; ++i) {
                int deviation = image->getPixel(i, j, c) - sndImg->getPixel(i, j, c);
                mse += deviation*deviation;
                me += abs(deviation);
            }
        }
    }

    mse = mse / static_cast<double>(maxChannel * maxWidth * maxHeight);
    me = me / static_cast<double>(maxChannel * maxWidth * maxHeight);
    QString text = qApp->translate("MeanSquareErrorOp", "Mean squarred error : %1 (mean error : %2)");
    text = text.arg(mse, 0, 'f', 2);
    text = text.arg(me, 0, 'f', 2);

    this->outText(text.toStdString());

}
