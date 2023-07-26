/*
 * Copyright 2011-2012 Benoit Averty, Samuel Babin, Matthieu Bergere, Thomas Letan, Sacha Percot-Tétu, Florian Teyssier
 * 
 * This file is part of DETIQ-T.
 * 
 * DETIQ-T is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * DETIQ-T is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with DETIQ-T.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "StructElemViewer.h"

#include <QGraphicsRectItem>
#include <QBrush>
#include <QPainter>
#include <sstream>

using namespace std;
using namespace imagein;

StructElemViewer::StructElemViewer(MorphoMat::StructElem* elem, bool editable)
: QGraphicsScene(0, 0, elem->getWidth() * PIXEL_S * elem->getScale(), elem->getHeight() * PIXEL_S*elem->getScale()),
  _scale(elem->getScale()), _row(elem->getWidth()*elem->getScale()), _col(elem->getHeight()*elem->getScale()),
  _editable(editable) {
  _views.push_back(ElemView(elem, 0, 0));
  _rectItem = nullptr;
  allocRects();
}

void StructElemViewer::allocRects() {
    this->clear();
    for(vector<ElemView>::iterator it = _views.begin(); it < _views.end(); ++it) {
        _row = max((it->x + it->elem->getWidth()), _row);
        _col = max((it->y + it->elem->getHeight()), _col);
    }
    _rects = new QGraphicsRectItem[_row * _col];
    for(unsigned int i = 0; i < _row; i++)
    {
        for(unsigned int j = 0; j < _col; j++)
        {
            QGraphicsRectItem& r = at(i, j);
            r.setRect(i * PIXEL_S, j * PIXEL_S, PIXEL_S, PIXEL_S);
            addItem(&r);
        }
    }
    this->setSceneRect(0, 0, _row*PIXEL_S, _col*PIXEL_S);
}
StructElemViewer::~StructElemViewer() {
    delete[] _rects;
    delete _rectItem;
}

void StructElemViewer::addStructElem(imagein::MorphoMat::StructElem* elem, int dx, int dy) {
    if(dx < 0 || dy < 0) {
        for(vector<ElemView>::iterator it = _views.begin(); it < _views.end(); ++it) {
            it->x -= min(dx, 0);
            it->y -= min(dy, 0);
        }
        dx = max(dx, 0);
        dy = max(dy, 0);
    }
    ElemView view = ElemView(elem, dx, dy);
    _views.push_back(view);
    _row = max(_row, (view.x + elem->getWidth()) * elem->getScale());
    _col = max(_col, (view.y + elem->getHeight()) * elem->getScale());

    delete[] _rects;
    allocRects();
}

void StructElemViewer::mousePressEvent (QGraphicsSceneMouseEvent* event) 
{
    QPointF point = event->scenePos(); // Get the location of the click
    unsigned int px = (point.x()/PIXEL_S)/_scale;
    unsigned int py = (point.y()/PIXEL_S)/_scale;
    ElemView view = _views.front();
    if(px >= view.x && py >= view.y && px< view.x + view.elem->getWidth() && py < view.y + view.elem->getHeight()) {
        if(event->button()==Qt::LeftButton) {
            view.elem->setPixel(px - view.x, py - view.y, !view.elem->getPixel(px - view.x, py - view.y));
        }
        else if(event->button() == Qt::RightButton) {
            view.elem->setCenter(px - view.x, py - view.y);
        }
    }
    this->draw(0,0);
}


void StructElemViewer::draw(int x, int y) {
    QColor high(255, 255, 255, 0);
    QColor red(255, 0, 0);
    for(int i = _views.size()-1; i >= 0; --i) {
        QColor low = i <= 1 ? QColor(50,50,50) : QColor::fromHsv(((i-2)*60)%360, 255, 230);

        ElemView view = _views.at(i);

        for(unsigned int j = 0; j < _col; j++) {
            for(unsigned int i = 0; i < _row; i++) {
                QGraphicsRectItem& r = at(i, j);

                int px = (i + x - view.x)/(view.elem->getScale());
                int py = (j + y - view.y)/view.elem->getScale();

                if(px >= 0 && py >= 0 && px < static_cast<int>(view.elem->getWidth()) && py < static_cast<int>(view.elem->getHeight())) {
                    if(view.elem->getPixel(px, py)){
                        r.setBrush(QBrush(low, Qt::SolidPattern));
                    }
                    else if(!view.elem->getPixel(px,py)){
                        r.setBrush(QBrush(high, Qt::SolidPattern));
                    }
                }
            }
        }
//! On retire l'ancien marqueur du centre de l'élément structurant
        if (_rectItem != nullptr) {
            this->removeItem(_rectItem);
        }
//! On place le nouveau marqueur du centre de l'élément structurant
        _rectItem = new QGraphicsRectItem(0, 0, (double) PIXEL_S / 2, (double) PIXEL_S / 2);
        _rectItem->setBrush(QBrush(red, Qt::SolidPattern));
        //! Dans le menu "Element structurant", on peut sélectionner le centre de l'ES
        if(_editable) {
            _rectItem->setPos(view.elem->getCenterX() * PIXEL_S + (PIXEL_S - (double) PIXEL_S / 2) / 2,
                              view.elem->getCenterY() * PIXEL_S + (PIXEL_S - (double) PIXEL_S / 2) / 2);
        }
        //! Dans le menu DMM, le centre de l'ES est fixe
        else{
            _rectItem->setPos((double(view.x)/(view.elem->getScale())) * PIXEL_S + (PIXEL_S - (double) PIXEL_S / 2) / 2,
                              (double(view.y)/view.elem->getScale()) * PIXEL_S + (PIXEL_S - (double) PIXEL_S / 2) / 2);
        }
        this->addItem(_rectItem);
    }
}


QGraphicsRectItem& StructElemViewer::at(int i, int j)
{
  return _rects[j * _row + i];
}

