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

#ifndef UCHARCONVERTDIALOG_H
#define UCHARCONVERTDIALOG_H

#include <QDialog>
#include <QSpinBox>
#include <QLabel>
#include <QComboBox>
#include <QString>

namespace Ui {
class UCharConvertDialog;
}

class UCharConvertDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UCharConvertDialog(QWidget *parent = 0);
    int getCombo();
    int getOffset();
    QString getText();


public slots:
    void enableOffset(int);
    void changeDescription(int);

protected:
    QLabel* _label2;
    QSpinBox* _spinBox;
    QComboBox* _comboBox;

};

#endif // UCHARCONVERTDIALOG_H
