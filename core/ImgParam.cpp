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

#include <Widgets/ImageListBox.h>
#include <QWidget>
#include <QFormLayout>
#include <QDialog>
#include "ImgParam.h"
#include <Widgets/ImageWidgets/StandardImageWindow.h>
#include <Widgets/ImageWidgets/DoubleImageWindow.h>

#include "Image.h"

using namespace std;
using namespace imagein;
using namespace genericinterface;

void ImgParam::fillDialog(QDialog* dialog, const ImageWindow* currentWnd, const vector<const ImageWindow*>& wndList) {
        QString currentImgName = currentWnd->windowTitle();
        map<const Image*,string> imgList;
        for(vector<const ImageWindow*>::const_iterator it = wndList.begin(); it != wndList.end(); ++it) {
                if((*it)->isStandard()) {
                        const StandardImageWindow* stdImgWnd = dynamic_cast<const StandardImageWindow*>(*it);
                        imgList.insert(pair<const Image*, string>(stdImgWnd->getImage(), stdImgWnd->windowTitle().toStdString()));
                }
        }
        QWidget *widget = new QWidget();
        QFormLayout* layout = new QFormLayout();
        _imgListBox = new ImageListBox(widget, NULL, imgList);
        layout->insertRow(1, QString(this->_name.c_str()) + " : ", _imgListBox);
        widget->setLayout(layout);
        dialog->layout()->addWidget(widget);
}

void ImgParam::pickValue() {
        *this->_ptr = *_imgListBox->currentImage();
}

Parameter<Image>* ImgParam::clone() const {
        return new ImgParam(*this);
}
