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

#ifndef QUANTIFICATIONWIDGET_H
#define QUANTIFICATIONWIDGET_H

#include <QWidget>
#include <QSpinBox>

#include <QScrollArea>
#include "Quantification.h"

class QuantificationWidget : public QScrollArea
{
    Q_OBJECT
public:
    explicit QuantificationWidget(QWidget *parent = 0);
    virtual QSize sizeHint () const;
    
signals:
    void resized();
    
public slots:
    void setNbThreshold(int n);
    void setQuantif(Quantification q);
    Quantification getQuantif() const;

protected:
    class CentralWidget : public QWidget {
        public:
            CentralWidget(QWidget* parent = 0);
            virtual QSize sizeHint() const;
            void setNbThreshold(int n);
            void setQuantif(Quantification q);
            Quantification getQuantif() const;
        protected:
            virtual void paintEvent (QPaintEvent *e);
            int _nThreshold;
            int _firstWidth, _secondWidth, _yOffset;
            QSpinBox* _thresholdBoxes[N_MAX_THRESHOLD];
            QSpinBox* _valueBoxes[N_MAX_THRESHOLD + 1];
    };
    CentralWidget* _centralWidget;
};

#endif // QUANTIFICATIONWIDGET_H
