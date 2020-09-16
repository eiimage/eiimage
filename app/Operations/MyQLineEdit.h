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

class MyQLineEdit : public QLineEdit {

  Q_OBJECT

  public:
    MyQLineEdit(QWidget *parent = nullptr) : QLineEdit(parent){};
    MyQLineEdit(const QString &contents, QWidget *parent = nullptr) : QLineEdit(contents, parent){};

  public slots:
    void updateValidator(const QString &text){
        /*Allow only 0 and 1 as input*/
        QRegExp regLogicalExp("^[1]d*|0$");
        /*Allow positive integer as input*/
        QRegExp regShiftExp("^\\d+$");
        QRegExpValidator* logicalValidator = new QRegExpValidator(regLogicalExp,0);
        QRegExpValidator* shiftValidator = new QRegExpValidator(regShiftExp,0);
        QIntValidator* intValidator = new QIntValidator();
        QDoubleValidator* doubleValidator = new QDoubleValidator();

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
