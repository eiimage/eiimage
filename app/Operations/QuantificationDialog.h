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

#ifndef QUANTIFICATIONDIALOG_H
#define QUANTIFICATIONDIALOG_H

#include <QDialog>
#include <QSpinBox>
#include <QComboBox>
#include "Quantification.h"
#include "QuantificationWidget.h"
#include "Widgets/ImageWidgets/ImageWindow.h"
#include <QPushButton>
#include <QCheckBox>

class QuantificationDialog : public QDialog
{
    Q_OBJECT
public:
    enum QuantMethod {LinearQuant, NonLinearQuant, NonLinearQuantOptimized};
    explicit QuantificationDialog(QWidget *parent = 0, QString imgName = QString());
    Quantification getQuantif(const genericinterface::ImageWindow *currentWnd, unsigned int c, std::string &);
    Quantification getQuantif();
    void setValues(int);
    void setQuantif(int);
signals:

public slots:
    void methodChanged(int);
    void open();
    void save();
protected:
        QSpinBox* _sizeBox;
        QComboBox* _quantBox;
        QSpinBox* _limitQuantifBox1;
        QSpinBox* _limitQuantifBox2;
        QuantificationWidget* _quantWidget;
        QWidget* _editorWidget;
        QPushButton* _saveButton;
        bool _editorOnly;
};

#endif // QUANTIFICATIONDIALOG_H
