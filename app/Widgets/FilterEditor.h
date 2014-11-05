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

#ifndef WIDGET_FILTEREDITION
#define WIDGET_FILTEREDITION

#include <iostream>
#include <vector>

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
  class FilterEditor : public QDialog
  {
  Q_OBJECT
  
  public:
    FilterEditor();

  signals:
    void applyFiltering(imagein::algorithm::Filtering*);
    void cancelAction();
    
  private slots:
    void save();
    void cancel();
    void nbFiltersChanged(const int);
    void nameChanged(QString);
    
  private:
    void initUI();
    void saveXML(std::vector<imagein::algorithm::Filter*> filter);
    std::vector<imagein::algorithm::Filter*> validFilters(bool* ok);

    QVBoxLayout* _filterLayout;
    QLineEdit* _name;
    int _nbFilters;
    QPushButton* _saveButton;
  };
}

#endif
