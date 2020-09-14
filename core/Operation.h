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

#ifndef IMAGEINSA_OPERATION_H
#define IMAGEINSA_OPERATION_H

#include <map>
#include <string>

#include "Image.h"

class QWidget;
namespace genericinterface {
class ImageWindow;
class WindowService;
}

/**
 * @brief The base class for all kind of operation.
 *
 * A GenericOperation run an image-processing algorithm, it may take some images and parameters as input and return some images, text or other widgets as output.\n
 * For implementing an operation working with standard 8bit unsigned integer Image (ie Image_t<uint8_t> or simply Image), see the Operation class.\n
 * For implementing an operation working with double-precision floating point Image (ie Image_t<double>), see the DoubleOperation class.\n
 * This class is a generic class and should not be directly used except for creating a generic operation (ie an operation working on different type of Image).\n
 * For an exemple of a true generic operation class, see the PointOp implementation in imageinsa source code.
 */
class GenericOperation {
public:
/**
 * @brief Constructor
 *
 * @param name The name of the operation, as it should be displayed to the user.
 */
GenericOperation(std::string name) : _name(name), _ws(NULL), _curImgWnd(NULL) {
}

/**
 * @brief Name accessor
 *
 * @return std::string The name of the operation.
 */
inline std::string getName() {
        return _name;
}

/**
 * @brief Pure virtual function call operator containing the implementation of the operation process.
 *
 * @param currentWnd A pointer to the ImageWindow having the user's focus if such exists, a NULL pointer otherwise.
 * @param imgWndList A vector containing all the ImageWindow existing in the application.
 */
virtual void operator()(const genericinterface::ImageWindow* currentWnd, const std::vector<const genericinterface::ImageWindow*>& imgWndList) = 0;
/**
 * @brief The function call operator which is called by the application, it is the entry point of this operation and it calls the pure virtual function call operator.
 *
 * @param ws The imageinsa implementation of the genericinterface::WindowService
 */
virtual void operator()(genericinterface::WindowService* ws);

/**
 * @brief This method should return wether this Operation need the currentWnd parameter.
 *
 * If this method returns true, the application guarantees that this operation cannot be called without the user having the focus on a valid image window (see isValidImgWnd()), therefore the parameter currentWnd given to the function call operation cannot be NULL. \n
 * If this method returns false, this operation can be called wether or not there is a valid current image window, and so the currentWnd parameter may be a NULL pointer.
 *
 * @return bool Wether this operation need a valid current ImageWindow.
 * \sa isValidImgWnd()
 */
virtual bool needCurrentImg() const = 0;
/**
 * @brief This method should determine wether an ImageWindow is valid or not for this operation.
 *
 * This method is used to check the current ImageWindow if needCurrentImg() returns true.
 * It is also used to generate the ImageWindow list given to the function call operation, it is guarenteed that all ImageWindow contained in the imgWndList passed this test.
 *
 * @param imgWnd The ImageWindow to check.
 * @return bool True if imgWnd is valid for this operation, false otherwise.
 * \sa needCurrentImg()
 */
virtual bool isValidImgWnd(const genericinterface::ImageWindow* imgWnd) const = 0;


protected:


/**
 * @brief %Output a standard Image to the user interface.

 * This method is used inside the operation process to output a standard Image to the user interface.
 *
 * @param img The image to output
 * @param title A title to distinguished this image from other images.
 */
void outImage(imagein::Image* img, std::string title = "");


/**
 * @brief %Output a floating point Image to the user interface.

 * This method is similar to outImage but is used to output a floating point Image while specifying the display parameters.

 * @param img The floating point image to output
 * @param title A title to distinguished this image from other images
 * @param norm Wether to display a normalized version of the Image (doesn't affect the data contained in the image, compatible with any other options).
 * @param log Wether to display a log scaled version of the Image (doesn't affect the data contained in the image, compatible with any other options).
 * @param logScale The logarithm scale constant to apply if the Image is displayed using log. scale (value from 8^-3 to 8^3, see genericinterface::DoubleImageWindow for more details).
 * @param abs wether to display an absolute value version of the Image (doesn't affect the data contained in the image, compatible with any other options).
 */
void outDoubleImage(imagein::ImageDouble* img, std::string title = "", bool norm=false, bool log=false, double logScale = 1., bool abs = false);


/**
 * @brief %Output some text to the user interface.
 *
 * @param std::string The text to output, may contain multiple lines.
 */
void outText(std::string);

std::string _name;     /**< The name of the operation */
genericinterface::WindowService* _ws;     /**< A pointer to the window service, only valid inside the function call operator */
genericinterface::ImageWindow* _curImgWnd;     /**< A pointer to the current image window, only valid inside the function call operator */

private:
/**
 * @brief Private method used to output an ImageWindow, this method is called inside outImage and outDoubleImage.
 *
 * @param imgWnd The ImageWindow to output
 * @param title The title of the ImageWindow
 */
void outImgWnd(genericinterface::ImageWindow* imgWnd, std::string title);
};

/**
 * @brief Implementation of GenericOperation specialized in standard Image.
 *
 * An Operation only gets as parameters only StandardImageWindow containing standard Image. \n
 * Therefore the manipulation of ImageWindow is not need anymore, an implementation of Operation only manipulates standard Image instead of ImageWindow.
 *
 */
class Operation : public GenericOperation {
public:
/**
 * @brief Constructor
 *
 * @param name The name of the operation
 */
Operation(std::string name) : GenericOperation(name) {
}

/**
 * @brief Implementation of GenericOperation::operator()()
 * This implementation get the standard images contained in the image windows and pass them to the other function call operator.
 *
 * @param currentWnd see GenericOperation::operator()()
 * @param wndList  see GenericOperation::operator()()
 * \sa operator()(const imagein::Image*, const std::map<const imagein::Image*, std::string>&)
 */
virtual void operator()(const genericinterface::ImageWindow* currentWnd, const std::vector<const genericinterface::ImageWindow*>& wndList);

/**
 * @brief Pure virtual function call operator containing the implementation of the operation process.
 *
 * @param img A pointer to the Image having the user's focus if such exists, a NULL pointer otherwise.
 * @param imgList A map containing all the opened Image in the application and their respective names.
 */
virtual void operator()(const imagein::Image* img, const std::map<const imagein::Image*, std::string>& imgList) = 0;

/**
 * @brief Implementation of GenericOperation::isValidImgWnd()
 *
 * @param imgWnd The ImageWindow to check
 * @return bool True if imgWnd is a StandardImageWindow instance, false otherwise.
 */
virtual bool isValidImgWnd(const genericinterface::ImageWindow* imgWnd) const;
protected:
imagein::Image* _currentImg;     /**< The current image, only valid inside the function call operator */
};

/**
 * @brief Implementation of GenericOperation specialized in double-precision floating point Image.
 *
 */
class DoubleOperation : public GenericOperation {
public:
/**
 * @brief Constructor
 *
 * @param name Name of the operation
 */
DoubleOperation(std::string name) : GenericOperation(name) {
}
/**
 * @brief Implementation of GenericOperation::operator()()
 * This implementation get the floating point images contained in the image windows and pass them to the other function call operator.
 *
 * @param currentWnd see GenericOperation::operator()()
 * @param wndList  see GenericOperation::operator()()
 * \sa operator()(const imagein::Image*, const std::map<const imagein::Image*, std::string>&)
 */
virtual void operator()(const genericinterface::ImageWindow* currentWnd, const std::vector<const genericinterface::ImageWindow*>&);
/**
 * @brief Pure virtual function call operator containing the implementation of the operation process.
 *
 * @param img A pointer to the Image having the user's focus if such exists, a NULL pointer otherwise.
 * @param imgList A map containing all the opened Image in the application and their respective names.
 */
virtual void operator()(const imagein::Image_t<double>*, const std::map<const imagein::Image_t<double>*, std::string>&) = 0;
/**
 * @brief Implementation of GenericOperation::isValidImgWnd()
 *
 * @param imgWnd The ImageWindow to check
 * @return bool True if imgWnd is a DoubleImageWindow instance, false otherwise.
 */
virtual bool isValidImgWnd(const genericinterface::ImageWindow* imgWnd) const;
protected:
imagein::Image_t<double>* _currentImg;     /**< The current image, only valid in the function call operator */
};

#endif //!IMAGEINSA_OPERATION_H
