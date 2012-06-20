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

#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <Service.h>
#include <Plugin.h>

#include <Services/AlgorithmService.h>

#include <QToolBar>

#include "PluginService.h"

class PluginManager : public QObject, public genericinterface::Service
{
  Q_OBJECT
  public:
    PluginManager(genericinterface::GenericInterface* gi); 
    void display(genericinterface::GenericInterface* gi);
    void connect(genericinterface::GenericInterface* gi);
    void unloadPlugin(Plugin*);

  public slots:
    void loadPlugin();
    void unloadAllPlugins();

  private:  
    bool preloadPlugin(QString file);
    genericinterface::GenericInterface* _gi;
    QAction* _loadPluginAction;
    QAction* _unloadPluginsAction;
    std::map<std::string, PluginService*> _pluginServices;
    void checkActionsValid();
};

#endif
