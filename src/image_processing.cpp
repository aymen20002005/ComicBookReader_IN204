#include "image_processing.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc_c.h>

TraitementImage:: TraitementImage()
{
         std::cout<<"Dans constructeur traitementimage sans paramétre "<<std::endl;


}
//------------------------------------------------------------------------------------------------
TraitementImage::TraitementImage(Archives  & ar1)
{
    std::cout<<"Dans constructeur traitementimage avec paramétre archive "<<std::endl;
    archiveParent = &ar1;

}
//----------------------------------------------------------------------------------------------
TraitementImage& TraitementImage::operator = (const TraitementImage &imageT )
   {
       if (&imageT != this)
       {
          archiveParent=imageT.archiveParent;
          nbr_page=imageT.nbr_page;

       }
       return *this;
   }
//-----------------------------------------------------------------------------------------
void TraitementImage::SetArchiveParent(std::string path)
{
    archiveParent=new ArchiveExtraction(path);
}
//----------------------------------------------------------------------------------------------
bool TraitementImage::ChargerArchive(std::string path,int Numeropage)
{
    archiveParent=new ArchiveExtraction(path);
    bool test = archiveParent->DecompresserArchive(Numeropage,path);
    if(!test)
        return false;
    nbr_page = archiveParent->get_page_number();
    return true;
}
//-------------------------------------------------------------------------------------------------
bool TraitementImage::TraiterImage(int NumPage, int TypeImage, cv::Mat& Image)
{

    if(!(archiveParent->ChargerImage( NumPage,Image)))
    {
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------------------------
bool TraitementImage::RedimensionnerImage(cv::Mat& image_entree, cv::Mat& image_sortie, int type_image)
{
    return true;
}
