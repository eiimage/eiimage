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

#include "PluginManager.h"

#include <GenericInterface.h>

#include <QMessageBox>
#include <QLibrary>
#include <string>
#include <typeinfo>
#include <sstream>

using namespace genericinterface;
using namespace std;

PluginManager::PluginManager(GenericInterface* gi) {

    _gi = gi;
    QDir directory("Plugins");
    QStringList files = directory.entryList();
    std::cout << files.size() << " files in plugins' directory" << std::endl;
    for(QStringList::iterator it = files.begin(); it != files.end(); ++it) {
        if(QLibrary::isLibrary(*it)) {
            std::cout << "library found : " << it->toStdString() << std::endl;
            loadPlugin((directory.path() + "/") + *it);
        }
    }

    
}

void PluginManager::display(GenericInterface* gi)
{
    
    QMenu* menu = gi->menu("&Plugin");
    
    _loadPluginAction = menu->addAction("&Load plugin");
    _unloadPluginsAction = menu->addAction("&Unload all plugins");
    checkActionsValid();
}

void PluginManager::connect(GenericInterface* gi)
{

    QObject::connect(_loadPluginAction, SIGNAL(triggered()), this, SLOT(choosePlugin()));
    QObject::connect(_unloadPluginsAction, SIGNAL(triggered()), this, SLOT(unloadAllPlugins()));
}

/*void PluginManager::checkActionsValid(QMdiSubWindow* activeWindow) {

	StandardImageWindow* window = (activeWindow) ? dynamic_cast<StandardImageWindow*>(activeWindow->widget()) : NULL;
	if(window) {
      _erosion->setEnabled(true);
    }
    else {
      _erosion->setEnabled(false);
    }
}*/
    
void PluginManager::choosePlugin() {
    QString file = QFileDialog::getOpenFileName(_gi, "Open a file", QString(), "Images (*.dll *.so *.dylib)");
    if(file.size()==0) return;
    std::map<std::string, Plugin*>::iterator it = _plugins.find(file.toStdString());
    if(it != _plugins.end()) {
        unloadPlugin(it->second);
        /*QMessageBox::information (_gi, "Library already loaded", "The library you are trying to load has already been loaded.");*/
        
        //return;
    }

    loadPlugin(file, false);
    
    //QLibrary* library = new QLibrary(file);
    //if(!library->load()) {
        //QMessageBox::critical (_gi, "Error loading plugin", library->errorString());
        //return;
    //}
    //else {
        ////QMessageBox::information (_gi, "Library loaded", "The plugin's library has been loaded successfully");
    //}
    
    //void* ptr = library->resolve("getPlugin");
    
    //if(ptr==0) {
        //QMessageBox::critical (_gi, "Error loading plugin", "Could not find the plugin's entry point \"getPlugin\"");
        //return;
    //} 
    //ostringstream oss;
    
    //typedef Plugin* (*entryPoint)();
    //entryPoint getPlugin = reinterpret_cast<entryPoint>(ptr);
    //Plugin* plugin = getPlugin();
    //if(plugin==NULL) {
        //QMessageBox::critical (_gi, "Error loading plugin", "The getPlugin entry point does not return a valid Plugin");
        //return;
    //}
    
    //vector<Operation*> operations = plugin->getOperations();
    
    
    ////oss << "Found " << operations.size() << " operations in the plugin !";
    //_plugins.insert(pair<string,Plugin*>(file.toStdString(), plugin));
    //emit addPlugin(plugin);
    ////QMessageBox::information (_gi, "Library loaded", oss.str().c_str());
    ////PluginService* pluginService = new PluginService(plugin);
    ////_pluginServices.insert(pair<string,PluginService*>(file.toStdString(), pluginService));
    ////_gi->addNewService(pluginService);
    ////checkActionsValid();
     
    //std::cout << "Plugin " << plugin->getName() << " loaded : " <<  operations.size() << " operations "<< std::endl;
}
void PluginManager::unloadAllPlugins() {
    for(std::map<string, Plugin*>::iterator it = _plugins.begin(); it != _plugins.end(); ++it) {
        emit removePlugin(it->second);
    }
    _plugins.clear();
    checkActionsValid();
}

void PluginManager::unloadPlugin(Plugin* plugin) {
    for(std::map<std::string, Plugin*>::iterator it = _plugins.begin(); it != _plugins.end(); ++it) {
        if(it->second==plugin) {
            //delete it->second;
            //_pluginServices.erase(it);
            ////it = _pluginServices.begin();
            _plugins.erase(it->first);
            emit removePlugin(plugin);
            checkActionsValid();
            return;
        }
    }
}

void PluginManager::checkActionsValid() {
    _unloadPluginsAction->setEnabled(_plugins.size()>0);
}

bool PluginManager::loadPlugin(QString file, bool silent) {

    QLibrary* library = new QLibrary(file);
    if(!library->load()) {
        if(!silent) {
            QMessageBox::critical (_gi, "Error loading plugin", library->errorString());
        }
        return false;
    }
    
    std::cout << file.toStdString() << " loaded" << std::endl;
    
    void* ptr = library->resolve("loadPlugin");
    
    if(ptr==0) {
        if(!silent) {
            QMessageBox::critical (_gi, "Error loading plugin", "Could not find the plugin's entry point \"loadPlugin\"");
        }
        return false;
    } 
    
    std::cout << file.toStdString() << ":  entry point found" << std::endl;
    
    typedef Plugin* (*entryPoint)();
    entryPoint getPlugin = reinterpret_cast<entryPoint>(ptr);
    Plugin* plugin = getPlugin();
    if(plugin==NULL) {
        if(!silent) {
            QMessageBox::critical (_gi, "Error loading plugin", "The getPlugin entry point does not return a valid Plugin");
        }
        return false;
    }
    std::cout << file.toStdString() << ":  got Plugin" << std::endl;
    
    vector<Operation*> operations = plugin->getOperations();
    std::cout << "Plugin " << plugin->getName() << " loaded : " <<  operations.size() << " operations "<< std::endl;
    
    //PluginService* pluginService = new PluginService(plugin);
    _plugins.insert(pair<string,Plugin*>(file.toStdString(), plugin));
    //_gi->addService(pluginService);
    emit addPlugin(plugin);
    checkActionsValid();
    return true;
}
