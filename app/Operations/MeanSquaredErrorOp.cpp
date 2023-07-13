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

#include <QLabel>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <Image.h>

#include <Widgets/ImageListBox.h>
#include "../Tools.h"

#include "MeanSquaredErrorOp.h"
#include "Widgets/ImageWidgets/StandardImageWindow.h"

using namespace std;
using namespace imagein;
using namespace genericinterface;

MeanSquaredErrorOp::MeanSquaredErrorOp() : GenericOperation(qApp->translate("Operations", "Mean squared error").toStdString())
{
}

bool MeanSquaredErrorOp::needCurrentImg() const {
    return true;
}

void MeanSquaredErrorOp::operator()(const genericinterface::ImageWindow* currentWnd, const std::vector<const genericinterface::ImageWindow*>& imgWndList) {

    auto* dialog = new QDialog();
    dialog->setWindowTitle(qApp->translate("Operations", "Compare to..."));
    dialog->setMinimumWidth(180);
    auto* layout = new QFormLayout();
    dialog->setLayout(layout);

    QString currentImgName = currentWnd->windowTitle();

    map<const Image*,string> stdImgList;
    map<const Image_t<double>*,string> dblImgList;
    for(auto it : imgWndList) {
        if(it->isStandard()) {
            const auto* stdImgWnd = dynamic_cast<const StandardImageWindow*>(it);
            stdImgList.insert(pair<const Image*, string>(stdImgWnd->getImage(), stdImgWnd->windowTitle().toStdString()));
        }
        else if(it->isDouble()) {
            const auto* dblImgWnd = dynamic_cast<const DoubleImageWindow*>(it);
            dblImgList.insert(pair<const Image_t<double>*, string>(dblImgWnd->getImage(), dblImgWnd->windowTitle().toStdString()));
        }
    }

    auto* imageBox = new MixImageListBox(dialog, currentImgName.toStdString(), stdImgList,dblImgList);

    layout->insertRow(0, qApp->translate("Operations", "Compare %1 to : ").arg(currentImgName), imageBox);

    auto* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, Qt::Horizontal, dialog);
    layout->insertRow(1, buttonBox);
    QObject::connect(buttonBox, SIGNAL(accepted()), dialog, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), dialog, SLOT(reject()));

    auto code = static_cast<QDialog::DialogCode>(dialog->exec());

    if(code!=QDialog::Accepted) {
        return;
    }

    const auto* currentDblWnd = dynamic_cast<const DoubleImageWindow*>(currentWnd);
    const Image_t<double>* image = currentDblWnd ? Converter<Image_t<double> >::convert(*currentDblWnd->getImage()) : Converter<Image_t<double> >::convert(*currentWnd->getDisplayImage());

    unsigned int maxWidth = image->getWidth();
    unsigned int maxHeight = image->getHeight();
    unsigned int maxChannel = image->getNbChannels();

    bool isDblImg;
    const Image_t<double>* dblImageImgs;
    const Image* stdImageImgs;


    if(imageBox->currentType() == MixImageListBox::DBLIMG) {
        const Image_t<double>* dblImg = imageBox->getDblImage(imageBox->currentText().toStdString());
        dblImageImgs = dblImg;
        isDblImg = true;

        maxWidth = min(maxWidth, dblImg->getWidth());
        maxHeight = min(maxHeight, dblImg->getHeight());
        maxChannel = min(maxChannel, dblImg->getNbChannels());
    }
    else {
        const Image* stdImg = imageBox->getStdImage(imageBox->currentText().toStdString());
        stdImageImgs = stdImg;
        isDblImg = false;

        maxWidth = min(maxWidth, stdImg->getWidth());
        maxHeight = min(maxHeight, stdImg->getHeight());
        maxChannel = min(maxChannel, stdImg->getNbChannels());
    }


    /*On est obligé de vérifier entre image1/image2 ET image2/image1 pour les deux types (double et Uchar)*/
    bool isChannelMismatch = image->getNbChannels()!=maxChannel;
    bool isChannelMismatch2;

    bool isHeightMismatch = image->getHeight() != maxHeight;
    bool isHeightMismatch2;

    bool isWidthMismatch = image->getWidth() != maxWidth;
    bool isWidthMismatch2;

    if (isDblImg) {
        isChannelMismatch2 = maxChannel != dblImageImgs->getNbChannels();
        isHeightMismatch2 = maxHeight != dblImageImgs->getHeight();
        isWidthMismatch2 =  maxWidth != dblImageImgs->getWidth();
    }
    else {
        isChannelMismatch2 = maxChannel != stdImageImgs->getNbChannels();
        isHeightMismatch2 = maxHeight != stdImageImgs->getHeight();
        isWidthMismatch2 = maxWidth != stdImageImgs->getWidth();
    }

    if (isWidthMismatch || isWidthMismatch2 || isHeightMismatch || isHeightMismatch2 ){
        string channelMismatchMessage = qApp->translate("MeanSquaredErrorOp", "MSE with larger image cropped from top left corner \n").toStdString();
        this->outText(channelMismatchMessage);
    }

    auto* imageToLevelOfGrey = new Image_t<double>(maxWidth, maxHeight, maxChannel);
    auto* imageToLevelOfGrey2 = new Image_t<double>(maxWidth, maxHeight, maxChannel);
    /*L'image opérande de gauche est en couleur et celle de droite est en niveau de gris*/
    /*On fait une conversion de l'opérande de gauche en niveau de gris*/
    if(isChannelMismatch){
        for(unsigned int j = 0; j < maxHeight; ++j) {
            for (unsigned int i = 0; i < maxWidth; ++i) {
                double tmpValue = 0;
                for(int c = 0; c < image->getNbChannels(); ++c) {
                    tmpValue += image->getPixel(i,j,c);
                }
                imageToLevelOfGrey->setPixel(i,j,0,tmpValue/3);
            }
        }
    }
        /*L'image opérande de droite est en couleur et celle de gauche est en niveau de gris et l'image est de type uchar*/
        /*On fait une conversion de l'opérande de droite en niveau de gris*/
    else if (isChannelMismatch2 && !isDblImg){
        for(unsigned int j = 0; j < maxHeight; ++j) {
            for (unsigned int i = 0; i < maxWidth ; ++i) {
                double tmpValue = 0;
                for(int c = 0; c < stdImageImgs->getNbChannels(); ++c) {
                    tmpValue += stdImageImgs->getPixel(i,j,c);
                }
                imageToLevelOfGrey2->setPixel(i,j,0,tmpValue/3);
            }
        }
    }
        /*L'image opérande de droite est en couleur et celle de gauche est en niveau de gris et l'image est de type double*/
        /*On fait une conversion de l'opérande de droite en niveau de gris*/
    else if (isChannelMismatch2 && isDblImg){
        for(unsigned int j = 0; j < maxHeight; ++j) {
            for (unsigned int i = 0; i < maxWidth; ++i) {
                double tmpValue = 0;
                for(int c = 0; c < dblImageImgs->getNbChannels(); ++c) {
                    tmpValue += dblImageImgs->getPixel(i,j,c);
                }
                imageToLevelOfGrey2->setPixel(i,j,0,tmpValue/3);
            }
        }
    }


    double mse = 0, me = 0;
    double deviation = 0;
    if (isChannelMismatch || isChannelMismatch2) {
        string channelMismatchMessage = qApp->translate("MeanSquaredErrorOp",
                                                        "MSE with color image transformed to grayscale \n").toStdString();
        this->outText(channelMismatchMessage);
    }

    if (isDblImg) {
        for (unsigned int c = 0; c < maxChannel; ++c) {
            for (unsigned int j = 0; j < maxHeight; ++j) {
                for (unsigned int i = 0; i < maxWidth; ++i) {
                    if (isChannelMismatch2){
                        deviation = image->getPixel(i, j, c) - imageToLevelOfGrey2->getPixel(i, j, c);
                    }
                    else if (isChannelMismatch){
                        deviation = imageToLevelOfGrey->getPixel(i, j, c) - dblImageImgs->getPixel(i, j, c);
                    }
                    else
                        deviation = image->getPixel(i, j, c) - dblImageImgs->getPixel(i, j, c);

                    mse += deviation*deviation;
                    me += abs(deviation);
                }
            }
        }
    }
    else{
        for (unsigned int c = 0; c < maxChannel; ++c) {
            for (unsigned int j = 0; j < maxHeight; ++j) {
                for (unsigned int i = 0; i < maxWidth; ++i) {
                    if (isChannelMismatch2){
                        deviation = image->getPixel(i, j, c) - imageToLevelOfGrey2->getPixel(i, j, c);
                    }
                    else if (isChannelMismatch){
                        deviation = imageToLevelOfGrey->getPixel(i, j, c) - stdImageImgs->getPixel(i, j, c);
                    }
                    else
                        deviation = image->getPixel(i, j, c) - stdImageImgs->getPixel(i, j, c);

                    mse += deviation*deviation;
                    me += abs(deviation);
                }
            }
        }
    }


    mse = mse / static_cast<double>(maxChannel * maxWidth * maxHeight);
    me = me / static_cast<double>(maxChannel * maxWidth * maxHeight);
//    QString text = qApp->translate("MeanSquareErrorOp", "Mean squarred error : %1 (mean error : %2)");
//    Change the name to match the calculation performed
    QString text = qApp->translate("MeanSquareErrorOp", "Mean squarred error : %1 (mean absolute error : %2)");
    text = text.arg(mse, 0, 'f', 2);
    text = text.arg(me, 0, 'f', 2);

    this->outText(text.toStdString());
    outText("-------------------------------------------");

}

bool MeanSquaredErrorOp::isValidImgWnd(const genericinterface::ImageWindow *imgWnd) const {
        return imgWnd != nullptr;
}
