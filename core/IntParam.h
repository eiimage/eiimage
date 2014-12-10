/*
 * Copyright 2011-2012 INSA Rennes
 * 
 * This file is part of INSAimage.
 * 
 * INSAimage is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * INSAimage is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with INSAimage.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef EIIMAGE_INTPARAM_H
#define EIIMAGE_INTPARAM_H

#include <vector>
#include <string>
#include "Image.h"

#include "Parameter.h"

class QSpinBox;
class QDialog;
/**
 * @brief This class describes a way to get an int parameter from the user.
 *
 */
class IntParam : public Parameter<int> {
  public:
/**
 * @brief Constructor of the input, wille be directly called inside the PlugOperation constructor.
 *
 * @param name Name of the input as it will be displayed to the user
 * @param min Lower bound of the possible values
 * @param max Upper bound of the possible values
 * @param def Default value of the input
 */
    IntParam(std::string name, int min, int max, int def = 0);
    /**
     * @brief Add a QSpinBox with the specified name to the dialog.
     *
     * @param dialog The dialog in which to add the widgets.
     */
    virtual void fillDialog(QDialog* dialog, const genericinterface::ImageWindow* currentWnd, const std::vector<const genericinterface::ImageWindow*>& wndList);

    /**
     * @brief Get the value of the QSpinBox and store it in the location pointerd by Parameter::_ptr
     *
     */
    virtual void pickValue();
    /**
     * @brief Copy constructor
     *
     * @return IntParam A copy of this instance
     */
    virtual IntParam* clone() const;
  protected:
    int _min, _max, _def; /**< The QSpinBox parameters */
    QSpinBox* _spinbox; /**< The QSpinBox widget to get the value from */
};

#endif //!EIIMAGE_INTPARAM_H
