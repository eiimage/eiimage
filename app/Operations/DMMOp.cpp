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

#include "DMMOp.h"
#include "DMMDialog.h"
#include "Operation.h"
#include "../Tools.h"

using namespace std;
using namespace imagein;
using namespace imagein::MorphoMat;

bool DMMOp::needCurrentImg() const {
    return true;
}

DMMOp::DMMOp() : Operation(qApp->translate("Operations", "DMM").toStdString()) {

}

void DMMOp::operator()(const imagein::Image* image, const std::map<const imagein::Image*, std::string>&) {


    DMMDialog* dialog = new DMMDialog();

    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog->exec());

    if(code!=QDialog::Accepted) {
        return;
    }
    vector<StructElem::Dir> basicElems = dialog->getBasicElements();
    if(basicElems.size() == 0) return;

    DMMDialog::Op op = dialog->getSelectedOp();

    switch(op) {
        case DMMDialog::Dilatation:
        {
            Image* resImg = dilatation(image, basicElems);
            outImage(resImg, qApp->translate("DMMOp", "DMM (dilatation)").toStdString());
            break;
        }
        case DMMDialog::Erosion:
        {
            Image* resImg = erosion(image, basicElems);
            outImage(resImg, qApp->translate("DMMOp", "DMM (erosion)").toStdString());
            break;
        }
        case DMMDialog::Opening:
        {
            Image* resImg = opening(image, basicElems);
            outImage(resImg, qApp->translate("DMMOp", "DMM (opening)").toStdString());
            break;
        }
        case DMMDialog::Closing:
        {
            Image* resImg = closing(image, basicElems);
            outImage(resImg, qApp->translate("DMMOp", "DMM (closing)").toStdString());
            break;
        }
        case DMMDialog::Dmm:
        {
            dmm(image, basicElems, dialog->getLevels());
            break;
        }
    }

}

inline Image::depth_t getNeighboor(const Image* image, unsigned int i, unsigned int j, unsigned int c, StructElem::Dir dir, Image::depth_t pixel) {
    Image::depth_t neighboor;
    switch(dir) {
        case StructElem::Left:
            neighboor = (i > 1) ? image->getPixel(i - 1, j, c) : pixel;
            break;
        case StructElem::TopLeft:
            neighboor = (i > 1 && j > 1) ? image->getPixel(i - 1, j - 1, c) : pixel;
            break;
        case StructElem::Top:
            neighboor = j > 1 ? image->getPixel(i, j - 1, c) : pixel;
            break;
        case StructElem::TopRight:
            neighboor = (i < (image->getWidth() - 1) && j > 1) ? image->getPixel(i + 1, j - 1, c) : pixel;
            break;
        case StructElem::Right:
            neighboor = (i < (image->getWidth() - 1)) ? image->getPixel(i + 1, j, c) : pixel;
            break;
        case StructElem::BottomRight:
            neighboor = (i < (image->getWidth() - 1)) && (j < (image->getHeight() - 1)) ? image->getPixel(i + 1, j + 1, c) : pixel;
            break;
        case StructElem::Bottom:
            neighboor = (j < image->getHeight() - 1) ? image->getPixel(i, j + 1, c) : pixel;
            break;
        case StructElem::BottomLeft:
            neighboor = (i > 1 && j < (image->getHeight() - 1)) ? image->getPixel(i - 1, j + 1, c) : pixel;
            break;
    }
    return neighboor;
}

Image* DMMOp::dilatation(const imagein::Image* image, std::vector<imagein::MorphoMat::StructElem::Dir> basicElems) {
    const Image* curImg = image;
    Image* outImg;
    for(vector<StructElem::Dir>::iterator it = basicElems.begin(); it < basicElems.end(); ++it) {
        Image* resImg = new Image(image->getWidth(), image->getHeight(), image->getNbChannels());
        for(unsigned int c = 0; c < curImg->getNbChannels(); ++c) {
            for(unsigned int j = 0; j < curImg->getHeight(); ++j) {
                for(unsigned int i = 0; i < curImg->getWidth(); ++i) {
                    Image::depth_t pixel = curImg->getPixel(i, j, c);
                    Image::depth_t neighboor = getNeighboor(curImg, i, j, c, *it, pixel);
                    resImg->setPixel(i, j, c, max(pixel, neighboor));
                }
            }
        }
        if(curImg != image) delete curImg;
        curImg = resImg;
        outImg = resImg;
    }
    return outImg;
}

Image* DMMOp::erosion(const imagein::Image* image, std::vector<imagein::MorphoMat::StructElem::Dir> basicElems) {
    const Image* curImg = image;
    Image* outImg;
    for(vector<StructElem::Dir>::iterator it = basicElems.begin(); it < basicElems.end(); ++it) {
        Image* resImg = new Image(image->getWidth(), image->getHeight(), image->getNbChannels());
        for(unsigned int c = 0; c < curImg->getNbChannels(); ++c) {
            for(unsigned int j = 0; j < curImg->getHeight(); ++j) {
                for(unsigned int i = 0; i < curImg->getWidth(); ++i) {
                    Image::depth_t pixel = curImg->getPixel(i, j, c);
                    Image::depth_t neighboor = getNeighboor(curImg, i, j, c, *it, pixel);
                    resImg->setPixel(i, j, c, min(pixel, neighboor));
                }
            }
        }
        if(curImg != image) delete curImg;
        curImg = resImg;
        outImg = resImg;
    }
    return outImg;
}

Image* DMMOp::opening(const imagein::Image* image, std::vector<imagein::MorphoMat::StructElem::Dir> basicElems) {
//    outText(QString("Opening with %1 basic elements").arg(basicElems.size()).toStdString());
    Image* tmpImg = erosion(image, basicElems);
    Image* resImg = dilatation(tmpImg, basicElems);
    delete tmpImg;
    return resImg;
}

Image* DMMOp::closing(const imagein::Image* image, std::vector<imagein::MorphoMat::StructElem::Dir> basicElems) {
    Image* tmpImg = dilatation(image, basicElems);
    Image* resImg = erosion(tmpImg, basicElems);
    delete tmpImg;
    return resImg;
}

void DMMOp::dmm(const Image* image, vector<StructElem::Dir> basicElems, vector<unsigned int> levels) {
    vector<Image*> components;
    //first component
    Image* component = opening(image, basicElems);
    components.push_back(component);
    Image* resImg = *image - *component;

    for(vector<unsigned int>::reverse_iterator it = levels.rbegin(); it < levels.rend(); ++it) {
        outText(QString("%1").arg(*it).toStdString());
        outText("-------------------------------------------");
        while(basicElems.size() > *it) basicElems.pop_back();
        component = opening(resImg, basicElems);
        components.push_back(component);
        Image* tmpImg = resImg;
        resImg = *resImg - *component;
        delete tmpImg;

    }
    for(unsigned int i = 0; i < components.size(); ++i) {
        outImage(components.at(i), qApp->translate("DMMOp", "DMM component #%1").arg(i).toStdString());
    }
    outImage(resImg, qApp->translate("DMM", "DMM error").toStdString());
}
