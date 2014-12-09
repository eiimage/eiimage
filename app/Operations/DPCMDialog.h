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

#ifndef DPCMDIALOG_H
#define DPCMDIALOG_H

#include <QDialog>
#include "Quantification.h"
#include "DPCM.h"

namespace Ui {
class DPCMDialog;
}

class DPCMDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit DPCMDialog(QWidget *parent = 0);
    ~DPCMDialog();
    Quantification* getQuantification() const;
    DPCM::Prediction getPrediction() const;
    double getQ() const;
    
private slots:
    void on_quantBrowseButton_clicked();

    void on_quantEditorButton_clicked();

private:
    Ui::DPCMDialog *ui;
};

#endif // DPCMDIALOG_H
