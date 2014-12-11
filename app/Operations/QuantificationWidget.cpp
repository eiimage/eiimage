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

#include <QGridLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <QPainter>
#include <iostream>
#include "QuantificationWidget.h"

using namespace std;

QuantificationWidget::QuantificationWidget(QWidget *parent) :
    QScrollArea(parent)
{
    _centralWidget = new CentralWidget(this);
    this->setWidgetResizable(false);
//    QGridLayout* editorLayout = new QGridLayout(centralWidget);
    this->setWidget(_centralWidget);
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    this->setFrameStyle(0);
}

QuantificationWidget::CentralWidget::CentralWidget(QWidget *parent) :
    QWidget(parent), _nThreshold(1) {
    QLabel* thresholdLabel = new QLabel(tr("Thresholds\n(low to high)"), this);
    thresholdLabel->setAlignment(Qt::AlignCenter);
    QLabel* valueLabel = new QLabel(tr("Values"), this);
    valueLabel->setAlignment(Qt::AlignCenter);


    _firstWidth = 96;
    _secondWidth = 96;
    _yOffset = 48;
    thresholdLabel->setGeometry(0, 0, _firstWidth, 48);
    valueLabel->setGeometry(_firstWidth + 32, 0, _secondWidth, 48);

    for(int i = 0; i < N_MAX_THRESHOLD; ++i) {
        _thresholdBoxes[i] = new QSpinBox(this);
        _thresholdBoxes[i]->setFixedSize(64, 28);
        _thresholdBoxes[i]->move(QPoint((_firstWidth-64)/2, i * 32 + 16 + _yOffset));
        _thresholdBoxes[i]->setRange(-254, 255);
        if(i >= _nThreshold) _thresholdBoxes[i]->setVisible(false);
    }
    for(int i = 0; i < N_MAX_THRESHOLD + 1; ++i) {
        _valueBoxes[i] = new QSpinBox(this);
        _valueBoxes[i]->setFixedSize(64, 28);
        _valueBoxes[i]->move(QPoint(_firstWidth + 32 + (_secondWidth-64)/2, i * 32 + _yOffset));
        _valueBoxes[i]->setRange(-254, 255);
        if(i > _nThreshold) _valueBoxes[i]->setVisible(false);
    }
//    editorLayout->addWidget(_valueBoxes[32], 32, 1);
//    this->setFixedSize(_firstWidth + 32 + _secondWidth, _nThreshold * 32 + _yOffset + 28);
}

void QuantificationWidget::CentralWidget::paintEvent(QPaintEvent *)
{
    QPainter *painter = new QPainter (this);
    QPen pen(Qt::darkGray, 2, Qt::DashLine);
    painter->setPen(pen);
    for(int i = 0; i < this->_nThreshold; ++i) {
        QPoint start = QPoint((_firstWidth-64)/2 + 64, i * 32 + 16 + _yOffset + 28/2);
        QPoint end = QPoint(_firstWidth + 32 +(_secondWidth-64)/2 + 64, i * 32 + 16 + _yOffset + 28/2);
        painter->drawLine (start, end);
    }
    painter->end ();
}

QSize QuantificationWidget::sizeHint () const {
    return this->widget()->size();
}

QSize QuantificationWidget::CentralWidget::sizeHint () const {
    return QSize(_firstWidth + 32 + _secondWidth, _nThreshold * 32 + _yOffset + 28);
}

void QuantificationWidget::setNbThreshold(int n) {
    _centralWidget->setNbThreshold(n - 1);
//    this->widget()->setFixedSize(_firstWidth + 32 + _secondWidth, _nThreshold * 32 + _yOffset + 28);
//    this->widget()->setMinimumSize(_firstWidth + 32 + _secondWidth, _nThreshold * 32 + _yOffset + 28);
    this->widget()->resize(this->widget()->sizeHint());
    QSize sizeHint = this->sizeHint();
    cout << sizeHint.width() << ":" << sizeHint.height() << endl;
    this->updateGeometry();
    this->parentWidget()->adjustSize();
    this->parentWidget()->parentWidget()->adjustSize();
}

void QuantificationWidget::CentralWidget::setNbThreshold(int n) {
    this->_nThreshold = min(N_MAX_THRESHOLD, n);
    for(int i = 0; i < N_MAX_THRESHOLD; ++i) {
        _thresholdBoxes[i]->setVisible(i < _nThreshold);
    }
    for(int i = 0; i < N_MAX_THRESHOLD + 1; ++i) {
        _valueBoxes[i]->setVisible(i <= _nThreshold);
    }
    this->updateGeometry();
}

void QuantificationWidget::setQuantif(Quantification q) {
    this->_centralWidget->setQuantif(q);
}

void QuantificationWidget::CentralWidget::setQuantif(Quantification q) {

    this->_nThreshold = q.nbThresholds();
    for(int i = 0; i < q.nbThresholds(); ++i) {
        _thresholdBoxes[i]->setValue(q.threshold(i));
    }
    for(int i = 0; i < q.nbValues(); ++i) {
        _valueBoxes[i]->setValue(q.value(i));
    }
    for(int i = 0; i < N_MAX_THRESHOLD; ++i) {
        _thresholdBoxes[i]->setVisible(i < _nThreshold);
    }
    for(int i = 0; i < N_MAX_THRESHOLD + 1; ++i) {
        _valueBoxes[i]->setVisible(i <= _nThreshold);
    }
    this->updateGeometry();
}

Quantification QuantificationWidget::getQuantif() const {
    return _centralWidget->getQuantif();
}

Quantification QuantificationWidget::CentralWidget::getQuantif() const {
    Quantification q(_nThreshold + 1);
    for(int i = 0; i < q.nbThresholds(); ++i) {
        q.setThreshold(i, _thresholdBoxes[i]->value());
    }
    for(int i = 0; i < q.nbValues(); ++i) {
        q.setValue(i, _valueBoxes[i]->value());
    }
    return q;
}
