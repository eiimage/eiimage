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

#ifndef IMAGEZONESELECTOR_H
#define IMAGEZONESELECTOR_H

#include <Widgets/ImageWidgets/ImageView.h>
#include <QRubberBand>
#include <vector>

class ImageZoneSelector : public genericinterface::ImageView
{
    Q_OBJECT
public:
    explicit ImageZoneSelector(QWidget *parent = 0, const imagein::Image* img = NULL);
    std::vector<imagein::Rectangle> getSelections() const;
    bool isSelectionEmpty() const;
    
signals:
    void selectionEmptinessChanged();

public slots:

protected:

    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseDoubleClickEvent(QMouseEvent * event);
    void wheelEvent(QWheelEvent* event);

    std::vector<QRubberBand*> _rubberBands;
    bool _tracing;
    QPoint _start;
    QRubberBand* _currentRubberBand;
};

#endif // IMAGEZONESELECTOR_H
