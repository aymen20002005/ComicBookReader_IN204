#ifndef ARCHIVE_H
#define ARCHIVE_H
#include <vector>
#include <string>
#include <C:/Users/user1/Desktop/ComicBookReader/opencv/include/opencv2/opencv.hpp>
#include <map>
class Archives
{
protected:
    std::string CheminArchive;
    int nombreTotalPages;
    std::map <int,std::string> ListeFichier;
public:
    Archives();
    Archives(std::string path);

    virtual bool ChargerImage(int numeroPage,cv::Mat& image)=0;
    virtual void LireArchive(std::string path)=0;
    virtual bool DecompresserArchive(int numPage,std::string ArchivePathName1)=0;

    std::string get_archive_path();
    int get_page_number();
    std::map <int,std::string> GetListeFichier();

};


#endif // ARCHIVE_H
