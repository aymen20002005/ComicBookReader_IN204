#ifndef IMAGE_H
#define IMAGE_H
#include <C:/Users/user1/Desktop/ComicBookReader/opencv/include/opencv2/opencv.hpp>
//#include "ArchiveExtraction.h"
#include"Archives.hpp"
class Images
{
protected:
   Archives *archiveParent;
public:
    virtual bool TraiterImage(int NumPage, int TypeImage, cv::Mat& Image) = 0;
    virtual bool RedimensionnerImage(cv::Mat& image_entree, cv::Mat& image_sortie, int type_image)=0;
    virtual bool ChargerArchive(std::string path,int Numeropage)=0;
    Archives * GetArchive(){return archiveParent;}
};

#endif // IMAGE_H
