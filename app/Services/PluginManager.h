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

#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <Service.h>
#include <Plugin.h>

#include <Services/AlgorithmService.h>

#include <QToolBar>
class QLibrary;

#include "OpSetService.h"

class PluginManager : public QObject, public genericinterface::Service
{
  Q_OBJECT
  public:
    PluginManager(genericinterface::GenericInterface* gi);
    void display(genericinterface::GenericInterface* gi);
    void connect(genericinterface::GenericInterface* gi);
    void unloadPlugin(Plugin*);

  public slots:
    void choosePlugin();
    void unloadAllPlugins();

  signals:
    void addPlugin(OpSet*);
    void removePlugin(OpSet*);

  private slots:
    void checkActionsValid();

  private:
    bool loadPlugin(QString file, bool silent = true);
    genericinterface::GenericInterface* _gi;
    QAction* _loadPluginAction;
    QAction* _unloadPluginsAction;
    std::map<std::string, Plugin*> _plugins;
    std::map<Plugin*, QLibrary*> _libraries;
};

#endif
