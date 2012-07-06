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

class ImageListBox : public QComboBox
{
    Q_OBJECT
public:
    explicit ImageListBox(QWidget *parent, const imagein::Image*, const std::map<const imagein::Image*, std::string>&);

    const imagein::Image* currentImage();
signals:
    
public slots:

protected:
    std::map<std::string, const imagein::Image*> _images;
};

#endif // IMAGELISTBOX_H
