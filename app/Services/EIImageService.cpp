/*
 * Copyright 2011-2012 INSA Rennes
 * 
 * This file is part of EIImage.
 * 
 * EIImage is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * EIImage is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with EIImage.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <GenericInterface.h>

#include "EIImageService.h"

using namespace genericinterface;
using namespace std;

EIImageService::EIImageService(GenericInterface* gi) {
    _gi = gi;
    _operationBar = new OperationBar();
}

void EIImageService::display(GenericInterface* gi)
{
    QDockWidget* _operationDock = new QDockWidget("Opérations", gi);
    _operationDock->setWidget(_operationBar);
    gi->addDockWidget(Qt::LeftDockWidgetArea, _operationDock);
}

void EIImageService::connect(GenericInterface*)
{
}

void EIImageService::addOpSet(OpSet* opSet) {
    for(vector<OpSetService*>::iterator it = _opSetServices.begin(); it != _opSetServices.end(); ++it) {
        if((*it)->getOpSet() == opSet) {
            return;
        }
    }
    OpSetService* opSetService = new OpSetService(opSet);
    _opSetServices.push_back(opSetService);
    _gi->addService(opSetService);
}

void EIImageService::removeOpSet(OpSet* opSet) {
    for(vector<OpSetService*>::iterator it = _opSetServices.begin(); it != _opSetServices.end(); ++it) {
        if((*it)->getOpSet() == opSet) {
            _opSetServices.erase(it);
            _gi->removeService(*it);
            delete *it;
            return;
        }
    }
}
