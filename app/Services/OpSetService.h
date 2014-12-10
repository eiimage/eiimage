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

#ifndef OPSETSERVICE_H
#define OPSETSERVICE_H

#include <Service.h>
//#include <Plugin.h>
#include "OperationService.h"

#include <Services/AlgorithmService.h>

#include <QToolBar>
#include <QMenu>


class OpSetService : public QObject, public genericinterface::Service
{
  Q_OBJECT
  public:
    OpSetService(OpSet*); 
    void display(genericinterface::GenericInterface* gi);
    void connect(genericinterface::GenericInterface* gi);
    inline OpSet* getOpSet() { return _opSet; }
    ~OpSetService();

  public slots:
    void checkActionsValid(const QWidget* activeWidget);

  signals:
    void outputText(QString);

  private:  
    OpSet* _opSet;
    genericinterface::GenericInterface* _gi;
    QMenu* _menu;
    //std::vector<QAction*> _actions;
    std::vector<OperationService*> _opServices;
};

#endif
