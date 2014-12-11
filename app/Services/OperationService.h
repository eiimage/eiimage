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

#ifndef OPERATIONSERVICE_H
#define OPERATIONSERVICE_H

#include <Service.h>
#include <Plugin.h>

#include <QToolBar>
#include <QMenu>

class OperationService : public QObject, public genericinterface::Service
{
  Q_OBJECT
  public:
    OperationService(GenericOperation* operation, QMenu* menu);
    void display(genericinterface::GenericInterface* gi);
    void connect(genericinterface::GenericInterface* gi);
    inline GenericOperation* getOperation() { return _operation; }
    ~OperationService();
    void setEnabled(bool);

  public slots:
    void operation();

  signals:
    void outputText(QString);

  protected:
    genericinterface::GenericInterface* _gi;
    GenericOperation* _operation;
    QMenu* _menu;
    QAction* _action;

};

#endif
