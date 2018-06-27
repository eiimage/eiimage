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

#include "FilterEditorItem.h"

#include <QLineEdit>
#include <QButtonGroup>
#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QSpinBox>
#include <QMdiArea>
#include <QPushButton>

#include <QTimer>
#include <QFile>
#include <QtXml/QDomDocument>
#include <QtXml/QDomImplementation>
#include <QtXml/QDomElement>
#include <QTextStream>

#include <iostream>
using namespace std;
using namespace filtrme;
using namespace imagein::algorithm;

FilterEditorItem::FilterEditorItem()
{
  _width = 3;
  _height = 3;
  initUI();
}


void FilterEditorItem::initUI()
{
  QSpinBox *spinBoxWidth;
  QSpinBox *spinBoxHeight;
  QLabel *label;
  QLabel *label_2;

  _filter = new QTableWidget(_width, _height, this);
  _filter->setGeometry(QRect(210, 60, 350, 260));
  _filter->resizeColumnsToContents();
  _filter->resizeRowsToContents();
  
  int w = 0,  h = 0;  
  for(int i = 0 ; i < _filter->rowCount(); i++)  
    h += _filter->rowHeight(i);  
  for(int i = 0 ; i < _filter->columnCount(); i++)  
    w += _filter->columnWidth(i);  

  _filter->setMinimumSize(w, h);  
  
  spinBoxWidth = new QSpinBox(this);
  spinBoxWidth->setGeometry(QRect(100, 167, 60, 27));
  spinBoxWidth->setValue(_width);
  spinBoxWidth->setMinimum(1);
  spinBoxHeight = new QSpinBox(this);
  spinBoxHeight->setGeometry(QRect(100, 207, 60, 27));
  spinBoxHeight->setValue(_height);
  spinBoxHeight->setMinimum(1);
  label = new QLabel(this);
  label->setText(tr("Width:"));
  label->setGeometry(QRect(30, 170, 66, 17));
  label_2 = new QLabel(this);
  label_2->setText(tr("Height:"));
  label_2->setGeometry(QRect(30, 210, 66, 17));

  this->setMinimumSize(600, 330);

  QObject::connect(spinBoxWidth, SIGNAL(valueChanged(int)), this, SLOT(widthChanged(int)));
  QObject::connect(spinBoxHeight, SIGNAL(valueChanged(int)), this, SLOT(heightChanged(int)));
  QObject::connect(this, SIGNAL(insertRow(int)), _filter, SLOT(insertRow(int)));
  QObject::connect(this, SIGNAL(removeRow(int)), _filter, SLOT(removeRow(int)));
  QObject::connect(this, SIGNAL(insertColumn(int)), _filter, SLOT(insertColumn(int)));
  QObject::connect(this, SIGNAL(removeColumn(int)), _filter, SLOT(removeColumn(int)));

}

void FilterEditorItem::widthChanged(const int width)
{
  if(_width < width){
      while(_width!=width){
          emit(insertColumn(width-_width));
          _width++;
      }
  }
  else{
      while(_width!=width){
          emit(removeColumn(_width-width));
          _width--;
      }
   }
  _filter->resizeColumnsToContents();
}

void FilterEditorItem::heightChanged(const int height)
{
    if(_height < height){
        while(_height!=height){
            emit(insertRow(height-_height));
            _height++;
        }
    }
    else{
        while(_height!=height){
            emit(removeRow(_height-height));
            _height--;
        }
     }
  _filter->resizeRowsToContents();
}

imagein::algorithm::Filter* FilterEditorItem::validFilter()
{
  Filter* filter = new Filter(_width, _height);
  bool ok;
  for(int w = 0; w < _width; w++)
  {
    for(int h = 0; h < _height; h++)
    {
      double i;
      QTableWidgetItem* item = _filter->item(h, w);
      if(!item) {
          cout << "item " << w << ":" << h << " not set !" << endl;
        ok = false;
      }
      else
        i = item->text().toDouble(&ok);
        
      if(ok)
        filter->setPixelAt(w, h, i);
    }
    if(!ok)
      break;
  }
  if(!ok)
    filter = NULL;
  
  return filter;
}
