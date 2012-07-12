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

#ifndef THRESHOLDOP_H
#define THRESHOLDOP_H

#include <QCoreApplication>

#include <vector>
#include <qwt_plot_marker.h>
#include <QDialog>
#include <QSpinBox>
#include <QLabel>
#include <QCheckBox>
#include <QRadioButton>

#include "Operation.h"
#include "Image.h"
#include "GrayscaleImage.h"

class QWidget;

class ThresholdOp : public Operation {

  public:
    
    ThresholdOp();

    std::vector<QWidget*> operator()(const imagein::Image*, const std::map<const imagein::Image*, std::string>&);

    bool needCurrentImg() const;

    static inline QString tr(const char* str) { return QCoreApplication::tr(str); }


  private:

};

#endif //!THRESHOLDOP_H
