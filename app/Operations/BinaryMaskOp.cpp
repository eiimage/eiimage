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

#include "BinaryMaskOp.h"
#include "Widgets/ImageListBox.h"
#include <Widgets/ImageWidgets/StandardImageWindow.h>
#include <QFormLayout>
#include <QDialogButtonBox>

using namespace std;
using namespace imagein;
using namespace genericinterface;

BinaryMaskOp::BinaryMaskOp() : GenericOperation(qApp->translate("BinaryMaskOp", "Apply mask").toStdString())
{
}

bool BinaryMaskOp::needCurrentImg() const {
    return true;
}

void BinaryMaskOp::operator()(const ImageWindow *currentWnd,
                              const vector<const ImageWindow *> &imgWndList) {

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


/*    QDialog* dialog = new QDialog(currentWnd);*/
    auto* dialog = new QDialog(QApplication::activeWindow());
    /*dialog = new QuantificationDialog(QApplication::activeWindow(), imgName);*/

    dialog->setWindowTitle(currentImgName);
    auto* layout = new QFormLayout(dialog);
    auto* imgBox = new MixImageListBox(dialog, currentImgName.toStdString(), stdImgList, dblImgList);
    layout->insertRow(0, qApp->translate("BinaryMaskOp","Mask to apply : "), imgBox);
    auto* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, Qt::Horizontal, dialog);
    layout->insertRow(1, buttonBox);
    QObject::connect(buttonBox, SIGNAL(accepted()), dialog, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), dialog, SLOT(reject()));

    auto code = static_cast<QDialog::DialogCode>(dialog->exec());

    if(code!=QDialog::Accepted) return;

    Image_t<double>* mask;
    if(imgBox->currentType() == MixImageListBox::STDIMG) {
        const Image* stdImg = imgBox->getStdImage(imgBox->currentText().toStdString());
        if(stdImg == nullptr) return;
        mask = Converter<Image_t<double> >::convert(*stdImg);
    }
    else if(imgBox->currentType() == MixImageListBox::DBLIMG) {
        const Image_t<double>* dblImg = imgBox->getDblImage(imgBox->currentText().toStdString());
        if(dblImg == nullptr) return;
        mask = new Image_t<double>(*dblImg);
    }
    else return;

    Image_t<double>* img;
    if(currentWnd->isStandard()) {
        const auto* siw = dynamic_cast<const StandardImageWindow*>(currentWnd);
        img = Converter<Image_t<double> >::convert(*siw->getImage());
    }
    else if(currentWnd->isDouble()) {
        const auto* diw = dynamic_cast<const DoubleImageWindow*>(currentWnd);
        img = new Image_t<double>(*diw->getImage());
    }

//    const double mean = (mask->max() - mask->min()) / 2.;
    const double max = mask->max();
    for(Image_t<double>::iterator it = mask->begin(); it < mask->end(); ++it) {
//        *it = (*it < mean) ? 0. : 1.;
        *it = *it / max;
    }

    const unsigned int width = min(mask->getWidth(), img->getWidth());
    const unsigned int height = min(mask->getHeight(), img->getHeight());
    auto* resImg = new Image_t<double>(width, height, img->getNbChannels());
    for(unsigned int c = 0; c < resImg->getNbChannels(); ++c) {
        const unsigned int maskChannel = mask->getNbChannels() >= img->getNbChannels() ? c : 0;
        for(unsigned int j = 0; j < resImg->getHeight(); ++j) {
            for(unsigned int i = 0; i < resImg->getWidth(); ++i) {
                resImg->pixelAt(i, j, c) = img->pixelAt(i, j, c) * mask->pixelAt(i, j, maskChannel);
            }
        }
    }
    delete img;
    delete mask;

    if(currentWnd->isStandard()) {
        Image* stdImg = Converter<Image>::convertAndRound(*resImg);
        delete resImg;
        this->outImage(stdImg, currentImgName.toStdString());
    }
    else if(currentWnd->isDouble()) {
        this->outDoubleImage(resImg, currentImgName.toStdString());
    }
}

bool BinaryMaskOp::isValidImgWnd(const genericinterface::ImageWindow *imgWnd) const {
    return (imgWnd && (imgWnd->isStandard() || imgWnd->isDouble()));
}
