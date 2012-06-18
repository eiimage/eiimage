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
