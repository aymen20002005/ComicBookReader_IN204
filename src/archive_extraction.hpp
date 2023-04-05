#ifndef ARCHIVE_EXTRACTION_H
#define ARCHIVE_EXTRACTION_H
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
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


class ArchiveExtraction: public Archives
{
public:
    ArchiveExtraction():Archives(){};

    ~ArchiveExtraction();
    ArchiveExtraction( ArchiveExtraction& ar1);
    ArchiveExtraction(std::string path1);
    void SetNombreTotalPages(int nombre);

    bool ChargerImage(int numeroPage,cv::Mat& image);
    void Extract(const char *filename, int do_extract, int flags,int numPage);
    void LireArchive(std::string path);
    bool DecompresserArchive(int numPage,std::string ArchivePathName1);

};
#endif // ARCHIVE_EXTRACTION_H

