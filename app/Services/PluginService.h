#ifndef PLUGINSERVICE_H
#define PLUGINSERVICE_H

#include <Service.h>
#include <Plugin.h>
#include "OperationService.h"

#include <Services/AlgorithmService.h>

#include <QToolBar>
#include <QMenu>


class PluginService : public QObject, public genericinterface::Service
{
  Q_OBJECT
  public:
    PluginService(Plugin*); 
    void display(genericinterface::GenericInterface* gi);
    void connect(genericinterface::GenericInterface* gi);
    Plugin* getPlugin();
    ~PluginService();

  public slots:
    void checkActionsValid(QMdiSubWindow* activeWindow);

  private:  
    Plugin* _plugin;
    genericinterface::GenericInterface* _gi;
    QMenu* _menu;
    //std::vector<QAction*> _actions;
    std::vector<OperationService*> _opServices;
};

#endif
