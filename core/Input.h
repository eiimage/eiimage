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

#ifndef IMAGEINSA_INPUT_H
#define IMAGEINSA_INPUT_H

#include <vector>
#include <map>
#include <string>
#include <Image.h>

class QDialog;
namespace genericinterface {
    class ImageWindow;
}
/**
 * @brief Input represents a data input from the user.
 *
 * Input is an interface describing the behavior of all kind of input used in a PlugOperation.
 * An input must implement the method fillDialog() in order to fill a given dialog with all the widgets needed to get the specific data from the user.
 * Then, the input must implement the pickValue() method to get the data and store it in a given lcoation.
 *
 */
class Input {
  public:
    /**
     * @brief Copy constructor
     *
     * @return Input A copy of this instance
     */
    virtual Input* clone() const = 0;
    /**
     * @brief Fill the dialog with all necessary widgets
     *
     * This method is called right before the dialog is shown to the user.
     *
     * @param dialog The dialog to put all the necessary widgets in.
     * @param imgList The list of all the images opened in the application.
     */
    virtual void fillDialog(QDialog* dialog, const genericinterface::ImageWindow* currentWnd, const std::vector<const genericinterface::ImageWindow*>& wndList) = 0;
    /**
     * @brief Pick the data from the widgets an store it in a given location.
     *
     * This method is called right after the user validate the dialog.
     *
     */
    virtual void pickValue() = 0;
};

#endif //!IMAGEINSA_INPUT_H
