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

#ifndef MYQLINEEDIT_H
#define MYQLINEEDIT_H

#include <QLineEdit>
#include <QRegExp>
#include <QRegExpValidator>
#include <QIntValidator>
#include <QDoubleValidator>
#include "Widgets/ImageWidgets/MyDoubleValidator.h"

class MyQLineEdit : public QLineEdit {

  Q_OBJECT

  public:

    explicit MyQLineEdit(QWidget *parent = nullptr) : QLineEdit(parent){};
    explicit MyQLineEdit(const QString &contents, QWidget *parent = nullptr) : QLineEdit(contents, parent){};

  public slots:
    void updateValidator(const QString &text){
        /*Allow only 0 and 1 as input*/
        QRegExp regLogicalExp("^[1]d*|0$");
        /*Allow positive integer as input*/
        QRegExp regShiftExp("^\\d+$");
        auto* logicalValidator = new QRegExpValidator(regLogicalExp, nullptr);
        auto* shiftValidator = new QRegExpValidator(regShiftExp,nullptr);
        auto* intValidator = new QIntValidator();
//        auto* doubleValidator = new genericinterface::MyDoubleValidator();
        auto* doubleValidator = new QDoubleValidator(-999.0,999.0,5, nullptr);
        //! Permet d'utilier le point comme séparateur décimal
        doubleValidator->setLocale(QLocale("C"));

        if(text.contains("NOT")){
            this->setEnabled(false);
        }else{
            this->setEnabled(true);
        }

        if(text.contains("logical")){
            this->setValidator(logicalValidator);
        }else if(text.contains("<<") || text.contains(">>")){
            this->setValidator(shiftValidator);
        }else if(text.contains("bit-wise")){
            this->setValidator(intValidator);
        }else{
            this->setValidator(doubleValidator);
        }
    }
};

#endif // MYQLINEEDIT_H
