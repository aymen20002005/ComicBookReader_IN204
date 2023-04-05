#include <QQmlApplicationEngine>
#include <QGuiApplication>

#include <iostream>
#include <archive.h>
#include <archive_entry.h>

#include <sys/types.h>

#include <sys/stat.h>

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#include <opencv2/opencv.hpp>

#include "charge_image_cache.hpp"
#include "image_processing.hpp"
#include "archive_extraction.hpp"
#include "image_provider.hpp"
#include "page_settings.hpp"

bool page_modified;
bool CheminArchiveChange;
bool image_precharge;
int nbr_page;
std::string CheminGlobalArchive;
ParametresPage ParametresChargement;

std::mutex PrechargementMutex;
std::condition_variable EtatPrechargement;
bool SortirProgramme;
bool path;


QCache<int,DonneeImage> CacheImage;
QReadWriteLock VerrouillerCache;



int main(int argc, char *argv[])
{


 QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);


 QGuiApplication app(argc, argv);
 QQmlApplicationEngine engine;

  path = false;
  qRegisterMetaType<ParametresPage>("ImagePreloadParams");

 ImgProvider *imgProvider= new ImgProvider();
 engine.addImageProvider(QStringLiteral("myimg"), imgProvider);


 engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

 if (engine.rootObjects().isEmpty())
     return -1;

imgProvider->object_property(engine.rootObjects().first());

QObject* fileDiag_ptr = engine.rootObjects().first()->findChild<QObject*>("FileDialog");
QObject::connect(fileDiag_ptr,SIGNAL(setFilePathSignal(QString)),imgProvider,SLOT(update_path(QString)));


 return app.exec();
}
