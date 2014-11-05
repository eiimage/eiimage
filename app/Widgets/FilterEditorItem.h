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

#ifndef WIDGET_FILTEREDITORITEM
#define WIDGET_FILTEREDITORITEM

#include <iostream>

#include <Algorithm/Filter.h>
#include <Algorithm/Filtering.h>

#include <QWidget>
#include <QVBoxLayout>
#include <QString>
#include <QList>
#include <QTableWidget>
#include <QMessageBox>

class QLineEdit;

namespace filtrme
{
  class FilterEditorItem : public QWidget
  {
  Q_OBJECT
  
  public:
    FilterEditorItem();
    imagein::algorithm::Filter* validFilter();
    int getWidth() const { return _width; }
    int getHeight() const { return _height; }
  
  signals:
    void insertRow(int);
    void removeRow(int);
    void insertColumn(int);
    void removeColumn(int);
    
  private slots:
    void widthChanged(const int);
    void heightChanged(const int);
    
  private:
    void initUI();

    QTableWidget* _filter;
    int _width;
    int _height;
  };
}

#endif //WIDGET_FILTEREDITORITEM
