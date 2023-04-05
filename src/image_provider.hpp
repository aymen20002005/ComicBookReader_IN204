#ifndef IMAGE_PROVIDER_H
#define IMAGE_PROVIDER_H
#include<QQuickImageProvider>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <string.h>
#include <filesystem>
#include <QObject>
#include <QString>
#include <iostream>
#include <QCache>
#include <QReadWriteLock>
#include "image_processing.hpp"
#include "charge_image_cache.hpp"
#include "page_settings.hpp"


extern bool path;
class ImgProvider : public QQuickImageProvider
{
    Q_OBJECT
private:

    QObject *rootObjectPointeur;
    TraitementImage TraitementImg;
    std::thread ThreadChargement;
    ChargerImageCache ChargerDansCache;

public:
    ImgProvider();
    ~ImgProvider();
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
    void object_property(QObject* Pointeur);
    QObject* object_property();
    void prechargement_image(int page1);
    QImage ConvertirCvMatEnQimage(const cv::Mat * image1);
public slots:
    void update_path(const QString &p);
};

#endif // IMAGE_PROVIDER_H
