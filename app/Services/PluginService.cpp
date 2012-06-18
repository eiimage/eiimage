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
    QObject::connect(_gi->windowService(), SIGNAL(subWindowActivated(QMdiSubWindow*)), 
                    this, SLOT(checkActionsValid(QMdiSubWindow*)));
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

void PluginService::checkActionsValid(QMdiSubWindow* activeWindow) {
    StandardImageWindow* window = (activeWindow) ? dynamic_cast<StandardImageWindow*>(activeWindow->widget()) : NULL;
    for(vector<OperationService*>::iterator it = _opServices.begin(); it < _opServices.end(); ++it) {
        if((*it)->getOperation()->needCurrentImg()) {
            (*it)->setEnabled(window != NULL);
        }
    }  
}