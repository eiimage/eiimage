#include <QApplication>

#include <GenericInterface.h>

#include <iostream>

#include "Services/PluginManager.h"
#include "Services/EIImageService.h"

using namespace genericinterface;
using namespace std;

int main(int argc, char** argv)
{
  qDebug("HW!");
  QApplication app(argc, argv);

  Log::configure(true, false, 0);

  GenericInterface m("ImageEII", Qt::RightDockWidgetArea);

  m.addService(new PluginManager(&m));
  m.addService(new EIImageService(&m));

  m.run();

  return app.exec();
}
