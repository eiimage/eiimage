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

#ifndef IMAGELISTBOX_H
#define IMAGELISTBOX_H

#include <QComboBox>
#include <map>
#include <string>

#include <Image.h>

template<typename D>
class ImageListBox_t : public QComboBox
{
public:
    explicit ImageListBox_t(QWidget *parent, const imagein::Image_t<D>* img, const std::map<const imagein::Image_t<D>*, std::string>& imgList)
        : QComboBox(parent)
    {
        int i = 0, index = 0;
        for(typename std::map<const imagein::Image_t<D>*, std::string>::const_iterator it = imgList.begin(); it != imgList.end(); ++it) {
            _images.insert(std::pair<std::string, const imagein::Image_t<D>*>(it->second, it->first));
            this->insertItem(i, QString(it->second.c_str()));
            if(it->first == img) index = i;
        }
        this->setCurrentIndex(index);
    }

    const imagein::Image_t<D>* currentImage() {
        std::string name = this->currentText().toStdString();
        typename std::map<std::string, const imagein::Image_t<D>*>::iterator it = _images.find(name);
        if(it != _images.end()) {
            return _images[name];
        }
        return NULL;
    }
protected:
    std::map<std::string, const imagein::Image_t<D>*> _images;
};

typedef ImageListBox_t<imagein::Image::depth_t> ImageListBox;

class MixImageListBox : public QComboBox
{
public:
    enum ImageType {STDIMG, DBLIMG};
    explicit MixImageListBox(QWidget *parent,
                          std::string currentImg,
                          std::map<const imagein::Image*,std::string> stdImgs,
                          std::map<const imagein::Image_t<double>*,std::string> dblImgs)
        : QComboBox(parent)
    {
        int i = 0, index = 0;
        for(typename std::map<const imagein::Image*, std::string>::const_iterator it = stdImgs.begin(); it != stdImgs.end(); ++it) {
            _images.insert(std::pair<std::string, ImageType>(it->second, STDIMG));
            _stdImgs.insert(std::pair<std::string, const imagein::Image*>(it->second, it->first));
            this->insertItem(i, QString(it->second.c_str()));
            if(it->second == currentImg) index = i;
        }
        for(typename std::map<const imagein::Image_t<double>*, std::string>::const_iterator it = dblImgs.begin(); it != dblImgs.end(); ++it) {
            _images.insert(std::pair<std::string, ImageType>(it->second, DBLIMG));
            _dblImgs.insert(std::pair<std::string, const imagein::Image_t<double>*>(it->second, it->first));
            this->insertItem(i, QString(it->second.c_str()));
            if(it->second == currentImg) index = i;
        }
        this->setCurrentIndex(index);
    }

    ImageType currentType() {
        std::string name = this->currentText().toStdString();
        return _images[name];
    }

    const imagein::Image* getStdImage(std::string name) {
        typename std::map<std::string, const imagein::Image*>::iterator it = _stdImgs.find(name);
        if(it != _stdImgs.end()) {
            return _stdImgs[name];
        }
        return NULL;
    }

    const imagein::Image_t<double>* getDblImage(std::string name) {
        typename std::map<std::string, const imagein::Image_t<double>*>::iterator it = _dblImgs.find(name);
        if(it != _dblImgs.end()) {
            return _dblImgs[name];
        }
        return NULL;
    }
protected:
    std::map<std::string, ImageType> _images;
    std::map<std::string, const imagein::Image*> _stdImgs;
    std::map<std::string, const imagein::Image_t<double>*> _dblImgs;

};
#endif // IMAGELISTBOX_H
