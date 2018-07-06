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

#ifndef IMAGEINSA_IMGPARAM_H
#define IMAGEINSA_IMGPARAM_H

#include <vector>
#include <string>
#include <map>
#include "Image.h"

#include "Parameter.h"

template<typename D>
class ImageListBox_t;
/**
 * @brief This class describes a way to get an Image parameter from the user
 *
 */
class ImgParam : public Parameter<imagein::Image> {
public:
/**
 * @brief Constructor, will be directly called in the PlugOperation.
 *
 * @param name Name of the input.
 * @param ptr Pointer in which to store the given input.
 */
ImgParam(std::string name, imagein::Image* ptr = NULL) : Parameter<imagein::Image >(name, ptr) {
}
virtual void fillDialog(QDialog* dialog, const genericinterface::ImageWindow* currentWnd, const std::vector<const genericinterface::ImageWindow*>& wndList);
virtual void pickValue();
virtual Parameter<imagein::Image >* clone() const;
protected:
ImageListBox_t<uint8_t>* _imgListBox;
};

#endif //!IMAGEINSA_IMGPARAM_H
