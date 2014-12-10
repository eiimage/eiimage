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

#include "ImageZoneSelector.h"

using namespace std;
using namespace imagein;
using namespace genericinterface;

ImageZoneSelector::ImageZoneSelector(QWidget *parent, const Image *img) :
    ImageView(parent, img)
{
    _tracing = false;
    _currentRubberBand = NULL;
}

void ImageZoneSelector::mousePressEvent(QMouseEvent * event) {
    _currentRubberBand = new QRubberBand(QRubberBand::Rectangle, _imgWidget);
    _start = mapToWidget(event->pos());
    if(_start.x() < 0) _start.setX(0);
    if(_start.y() < 0) _start.setY(0);
    if(_start.x() >= _imgWidget->width()) _start.setX(_imgWidget->width() - 1);
    if(_start.y() >= _imgWidget->height()) _start.setY(_imgWidget->height() - 1);
    _currentRubberBand->setGeometry(QRect(_start, _start));
    _currentRubberBand->setVisible(true);
    _tracing = true;
}

bool ImageZoneSelector::isSelectionEmpty() const {
    return _rubberBands.empty();
}

void ImageZoneSelector::mouseReleaseEvent(QMouseEvent * event) {
    _tracing = false;
    _rubberBands.push_back(_currentRubberBand);
    emit selectionEmptinessChanged();
    _currentRubberBand = NULL;
    _start = QPoint();
}

void ImageZoneSelector::mouseMoveEvent(QMouseEvent * event) {
    if(_tracing && _currentRubberBand != NULL) {
        QPoint pos = mapToWidget(event->pos());
        if(pos.x() < 0) pos.setX(0);
        if(pos.y() < 0) pos.setY(0);
        if(pos.x() >= _imgWidget->width()) pos.setX(_imgWidget->width() - 1);
        if(pos.y() >= _imgWidget->height()) pos.setY(_imgWidget->height() - 1);
        QRect rect = QRect(_start, pos);
        _currentRubberBand->setGeometry(rect.normalized());
    }
}

void ImageZoneSelector::mouseDoubleClickEvent(QMouseEvent * event) {
    event->ignore();
}

void ImageZoneSelector::wheelEvent(QWheelEvent* event) {
    event->ignore();
}

vector<Rectangle> ImageZoneSelector::getSelections() const {
    vector<Rectangle> selections;
    for(vector<QRubberBand*>::const_iterator it = _rubberBands.begin(); it < _rubberBands.end(); ++it) {
        QRect rect = _imgWidget->mapToPixmap((*it)->geometry());
        selections.push_back(Rectangle(rect.x(), rect.y(), rect.width(), rect.height()));
    }
    return selections;
}
