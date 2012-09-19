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

#ifndef DCTDIALOG_H
#define DCTDIALOG_H

#include <QDialog>

namespace Ui {
class DCTDialog;
}

class DCTDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit DCTDialog(QWidget *parent = 0);
    ~DCTDialog();
    bool isTruncMode() const;
    int getTruncLimit() const;
    int getNbBitInit() const;
    double getSlope() const;
    
private:
    Ui::DCTDialog *ui;
};

#endif // DCTDIALOG_H
