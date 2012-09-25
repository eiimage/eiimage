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

#include <QSpinBox>
#include <QDialog>
#include <QLayout>
#include <QFormLayout>
#include <QWidget>

#include "IntParam.h"

using namespace imagein;
using namespace std;
using namespace genericinterface;

IntParam::IntParam(std::string name, int min, int max, int def) : Parameter<int>(name), _min(min), _max(max) {
    _def = std::min(def, _max);
    _def = std::max(def, _min);
}

void IntParam::fillDialog(QDialog* dialog, const ImageWindow*, const vector<const ImageWindow*>&) {
    QWidget *widget = new QWidget();
    QFormLayout* layout = new QFormLayout();
    _spinbox = new QSpinBox();
    _spinbox->setRange(_min, _max);
    _spinbox->setValue(_def);
    layout->insertRow(1, QString(_name.c_str()) + " : ", _spinbox);
    widget->setLayout(layout);
    dialog->layout()->addWidget(widget);
}

void IntParam::pickValue() {
    int value = _spinbox->value();
    value = std::min(value, _max);
    value = std::max(value, _min);
    *_ptr = value;
}

IntParam* IntParam::clone() const {
    return new IntParam(*this);
}
