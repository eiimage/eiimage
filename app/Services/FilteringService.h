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

#ifndef FILTERINGSERVICE_H
#define FILTERINGSERVICE_H

#include <Services/AlgorithmService.h>

#include <Widgets/ImageWidgets/StandardImageWindow.h>

#include "Widgets/FilterChoice.h"
#include "Widgets/FilterEditor.h"

#include <QStringList>
#include <pthread.h>

namespace filtrme
{
  /*!
   * \brief Implementation of AlgorithmService for Filtering application
	*
	* It allows to apply filter on image
	*/
  class FilteringService : public genericinterface::AlgorithmService
  {
  Q_OBJECT
  public:
    void display(genericinterface::GenericInterface* gi);
    void connect(genericinterface::GenericInterface* gi);
    
    void applyAlgorithm(imagein::algorithm::Filtering* algo);


  public slots:
    void applyFiltering();
    void edition();
    void checkActionsValid(const QWidget* activeWidget);

  protected:


  private:  
    FilterChoice* _filterChoice;
    FilterEditor* _filterEditor;
    QAction* _filtering;
    QAction* _filterEdition;
    QStringList _texts;

    genericinterface::ImageWindow* _siw;
  };
}

#endif //FILTERINGSERVICE_H
