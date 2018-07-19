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

#ifndef TRANSLATION_H
#define TRANSLATION_H

#include <QCoreApplication>

#include <vector>
#include <QDialog>
#include <QSpinBox>
#include <QLabel>
#include <QCheckBox>
#include <QRadioButton>

#include "Operation.h"
#include "Image.h"

class QWidget;

class TranslateOp : public Operation {

  public:

    TranslateOp();

    void operator()(const imagein::Image*, const std::map<const imagein::Image*, std::string>&);

    bool needCurrentImg() const;

    void setTest(bool);

    void setXVal(int);

    void setYVal(int);


  private:
    bool _test;
    int _xval;
    int _yval;

};


#endif //!TRANSLATION_H
