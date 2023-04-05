#ifndef CACHE_HPP
#define CACHE_HPP
#include <C:/Users/user1/Desktop/ComicBookReader/opencv/include/opencv2/opencv.hpp>
#include <QCache>
#include <QReadWriteLock>
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
#endif // CACHE_HPP
