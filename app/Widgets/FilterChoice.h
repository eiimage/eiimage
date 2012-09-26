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

#ifndef FILTERINGCHOICE_H
#define FILTERINGCHOICE_H

#include <Services/AlgorithmService.h>

#include <Algorithm/Filter.h>
#include <Algorithm/Filtering.h>

#include <vector>
#include <QWidget>
#include <QComboBox>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QStringList>
#include <Algorithm/Filtering.h>
#include <QRadioButton>

namespace filtrme
{
  /*!
   * \brief Implementation of AlgorithmService for Filtering application
	*
	* It allows to apply filter on image
	*/
  class FilterChoice : public QDialog
  {
  Q_OBJECT
  
  public:
    FilterChoice(QWidget *parent);
    inline imagein::algorithm::Filtering* getFiltering() { return _filtering; }
    inline bool doubleResult()  { return _dblResButton->isChecked(); }
    inline void setDoubleResult(bool c)  { _dblResButton->setChecked(c); _stdResButton->setChecked(!c);}

  signals:
    void choiceValidate(imagein::algorithm::Filtering* filtering);
  
  private slots:
    void currentBlurChanged(int);
    void dataChanged(const QString&);
    void validate();
    void cancel();
    void deleteFilter();

  private:
    void initUI();
    void updateDisplay();
    QStringList initFilters();
    
    std::vector< std::vector< imagein::algorithm::Filter* > > _filters;
    
    QComboBox* _blurChoices;
    QComboBox* _policyChoices;
    
    QLabel* _labelNumber;
    QSpinBox* _number;
    QDoubleSpinBox* _stdDevBox;
    QLabel* _stdDevLabel;
    
    QTableWidget* _filterView;
    QPushButton* _deleteButton;
    imagein::algorithm::Filtering* _filtering;
    QRadioButton* _dblResButton;
    QRadioButton* _stdResButton;
  };
}

#endif //FILTERINGCHOICE_H

