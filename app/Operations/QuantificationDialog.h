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

#ifndef QUANTIFICATIONDIALOG_H
#define QUANTIFICATIONDIALOG_H

#include <QDialog>
#include <QSpinBox>
#include <QComboBox>
#include "Quantification.h"
#include "QuantificationWidget.h"
#include <QPushButton>

class QuantificationDialog : public QDialog
{
    Q_OBJECT
public:
    enum QuantMethod {LinearQuant, NonLinearQuant, NonLinearQuantOptimized};
    explicit QuantificationDialog(QWidget *parent = 0, QString imgName = QString());
    Quantification getQuantif(const imagein::Image *image, unsigned int c);
    Quantification getQuantif();
signals:
    
public slots:
    void methodChanged(int);
    void open();
    void save();
protected:
        QSpinBox* _sizeBox;
        QComboBox* _quantBox;
        QuantificationWidget* _quantWidget;
        QWidget* _editorWidget;
        QPushButton* _saveButton;
        bool _editorOnly;
};

#endif // QUANTIFICATIONDIALOG_H
