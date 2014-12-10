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

#ifndef EIIMAGESERVICE_H
#define EIIMAGESERVICE_H

#include <QTextEdit>

#include <Service.h>
#include <Plugin.h>

#include "OpSetService.h"

#include "../Widgets/OperationBar.h"
#include "Services/WindowService.h"

class EIImageService : public genericinterface::WindowService
{
  Q_OBJECT
  public:
    EIImageService(genericinterface::GenericInterface* gi); 
    void display(genericinterface::GenericInterface* gi);
    void connect(genericinterface::GenericInterface* gi);

  public slots:
    void addOpSet(OpSet* opSet);
    void removeOpSet(OpSet* opSet);
    void outputText(QString text);
    virtual void addText(std::string);

  private:  
    genericinterface::GenericInterface* _gi;
    OperationBar* _operationBar;
    QTextEdit* _statusEdit;
    std::vector<OpSetService*> _opSetServices;
};

#endif
