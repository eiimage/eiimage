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

#include "OpSetService.h"

#include <GenericInterface.h>

#include <QMessageBox>
#include <QLibrary>
#include <string>
#include <typeinfo>
#include <sstream>

using namespace genericinterface;
using namespace std;

OpSetService::OpSetService(OpSet* opSet) : _opSet(opSet){
    
}

void OpSetService::display(GenericInterface* gi)
{
    _gi = gi;
    
    _menu = gi->menu(_opSet->getName().c_str());
     _menu->menuAction()->setVisible(true);
    
    vector<GenericOperation*> operations = _opSet->getOperations();
    for(vector<GenericOperation*>::iterator it = operations.begin(); it < operations.end(); ++it) {
        OperationService* opService = new OperationService(*it, _menu);
        _opServices.push_back(opService);
        opService->display(gi);
        opService->setEnabled(!(*it)->needCurrentImg());
    }
}

void OpSetService::connect(GenericInterface* gi)
{
    for(vector<OperationService*>::iterator it = _opServices.begin(); it < _opServices.end(); ++it) {
        (*it)->connect(gi);
        QObject::connect(*it, SIGNAL(outputText(QString)), this, SIGNAL(outputText(QString)));
    }
	//connexion des changements d'images
    QObject::connect(_gi->windowService(), SIGNAL(activeWidgetChanged(const QWidget*)), 
                    this, SLOT(checkActionsValid(const QWidget*)));
}

OpSetService::~OpSetService() {
    for(vector<OperationService*>::iterator it = _opServices.begin(); it < _opServices.end(); ++it) {
        delete *it;
    }
    _opServices.clear();
    if(_menu->isEmpty()) {
        //_gi->removeAction(_menu->menuAction());
        _menu->menuAction()->setVisible(false);
    }
}

void OpSetService::checkActionsValid(const QWidget* activeWidget) {
    const ImageWindow* window = dynamic_cast<const ImageWindow*>(activeWidget);
    for(vector<OperationService*>::iterator it = _opServices.begin(); it < _opServices.end(); ++it) {
        if((*it)->getOperation()->needCurrentImg()) {
            (*it)->setEnabled((*it)->getOperation()->isValidImgWnd(window));
        }
    }  
}
