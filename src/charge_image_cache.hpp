#ifndef CHARGE_IMAGE_CACHE_H
#define CHARGE_IMAGE_CACHE_H
#include "image_processing.hpp"
#include <mutex>
#include <thread>
#include<condition_variable>
#include <QCache>
#include <QReadWriteLock>
#include <opencv2/opencv.hpp>
#include "page_settings.hpp"

extern bool page_modified;
extern bool CheminArchiveChange;
extern bool  image_precharge;
extern int nbr_page;
extern std::string CheminGlobalArchive;
extern ParametresPage ParametresChargement;

extern std::mutex PrechargementMutex;
extern std::condition_variable EtatPrechargement;

extern bool SortirProgramme;

class DonneeImage
{
public:

    int TypeImage;
    cv::Mat *PointeurImage;

    DonneeImage() {
        PointeurImage = new cv::Mat;
    }
    ~DonneeImage(){
        delete PointeurImage;
    }
};
extern QCache<int,DonneeImage> CacheImage; //definition de Qcache qui stocke les données de type donneeImage et clé le numero de page
extern QReadWriteLock VerrouillerCache;//Pour protéger l'accès au cache


class ChargerImageCache
{
    Images* imageTraitement;
public:
    ChargerImageCache();
    ChargerImageCache(Images& imageT);
    void MettreImageDansCache(const int page_num, const int page_type);
    void ChagerParalleleImage();
    Images* GetTraitementImg();

    void SetTraitementImg(Images& imageT);

};

#endif // CHARGE_IMAGE_CACHE_H
