#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H
#include <opencv2/opencv.hpp>
#include "archive_extraction.hpp"
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

class TraitementImage: public Images
{
private:

    int nbr_page;

public:
    TraitementImage();
    TraitementImage(Archives  & ar1);
    bool TraiterImage(int NumPage, int TypeImage, cv::Mat& Image);
    bool RedimensionnerImage(cv::Mat& ImageEntree, cv::Mat& ImageSortie, int TypeImage);
    bool ChargerArchive(std::string path,int Numeropage);
    int getNbrTotalPage(){return nbr_page;}
    void SetArchiveParent(std::string path);
    TraitementImage &operator = (const TraitementImage &imageT );

};
#endif // IMAGE_PROCESSING_H
