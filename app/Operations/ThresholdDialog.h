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

#ifndef THRESHOLDDIALOG_H
#define THRESHOLDDIALOG_H

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

class ThresholdDialog : public QDialog {
  Q_OBJECT
  public:

    ThresholdDialog(const imagein::GrayscaleImage*, bool converted);
    inline int threshold1() { return _spinbox1->value(); }
    inline int threshold2() { return _spinbox2->value(); }
    inline bool blackBand() { return !_blackButton->isChecked(); }
    inline bool doubleThreshold() { return _doubleBox->isChecked(); }

  public slots:
    void marker1Moved(const QPointF&);
    void marker2Moved(const QPointF&);
    void spinbox1Changed(int);
    void spinbox2Changed(int);
    void doubleThreshold(bool);
    void otsu();

  private:
    QwtPlotMarker *_marker1, *_marker2;
    QSpinBox *_spinbox1, *_spinbox2;
    QLabel* _spin1label;
    const imagein::GrayscaleImage* _image;
    QRadioButton* _blackButton;
    QCheckBox* _doubleBox;
};

#endif //!THRESHOLDDIALOG_H
