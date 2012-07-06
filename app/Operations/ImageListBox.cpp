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

#include "ImageListBox.h"

using namespace std;
using namespace imagein;

ImageListBox::ImageListBox(QWidget *parent, const Image* img, const map<const Image*, string>& imgList) :
    QComboBox(parent)
{
    int i = 0, index = 0;
    for(map<const Image*, string>::const_iterator it = imgList.begin(); it != imgList.end(); ++it) {
        _images.insert(pair<string, const Image*>(it->second, it->first));
        this->insertItem(i, QString(it->second.c_str()));
        if(it->first == img) index = i;
    }
    this->setCurrentIndex(index);
}

const Image* ImageListBox::currentImage() {
    string name = this->currentText().toStdString();
    map<string, const Image*>::iterator it = _images.find(name);
    if(it != _images.end()) {
        return _images[name];
    }
    return NULL;
}
