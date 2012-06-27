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

#include "PluginService.h"

#include <GenericInterface.h>

#include <QMessageBox>
#include <QLibrary>
#include <string>
#include <typeinfo>
#include <sstream>

using namespace genericinterface;
using namespace std;

PluginService::PluginService(Plugin* plugin) : _plugin(plugin){
    
}

void PluginService::display(GenericInterface* gi)
{
    _gi = gi;
    
    _menu = gi->menu(_plugin->getName().c_str());
     _menu->menuAction()->setVisible(true);
    
    vector<Operation*> operations = _plugin->getOperations();
    for(vector<Operation*>::iterator it = operations.begin(); it < operations.end(); ++it) {
        OperationService* opService = new OperationService(*it, _menu);
        _opServices.push_back(opService);
        opService->display(gi);
        opService->setEnabled(!(*it)->needCurrentImg());
    }
}

void PluginService::connect(GenericInterface* gi)
{
    for(vector<OperationService*>::iterator it = _opServices.begin(); it < _opServices.end(); ++it) {
        (*it)->connect(gi);
    }
	//connexion des changements d'images
    QObject::connect(_gi->windowService(), SIGNAL(activeWidgetChanged(const QWidget*)), 
                    this, SLOT(checkActionsValid(const QWidget*)));
}

Plugin* PluginService::getPlugin() {
    return _plugin;
}

PluginService::~PluginService() {
    for(vector<OperationService*>::iterator it = _opServices.begin(); it < _opServices.end(); ++it) {
        delete *it;
    }
    _opServices.clear();
    if(_menu->isEmpty()) {
        //_gi->removeAction(_menu->menuAction());
        _menu->menuAction()->setVisible(false);
    }
}

void PluginService::checkActionsValid(const QWidget* activeWidget) {
    const StandardImageWindow* window = dynamic_cast<const StandardImageWindow*>(activeWidget);
    for(vector<OperationService*>::iterator it = _opServices.begin(); it < _opServices.end(); ++it) {
        if((*it)->getOperation()->needCurrentImg()) {
            (*it)->setEnabled(window != NULL);
        }
    }  
}
