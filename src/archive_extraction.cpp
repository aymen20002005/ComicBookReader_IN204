#include "archive_extraction.hpp"
#include <opencv2/opencv.hpp>
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
#include <vector>
#include <map>
#include<filesystem>




static void	messageErreur(const char *);
static void	Echouer(const char *, const char *, int);
static int	CopierDonnees(struct archive *, struct archive *);
static void	message(const char *);
static void	Notifier(const char *, const char *);
static int verbose= 0;

static void message(const char *m)
{
    write(1, m, strlen(m));
}

static void messageErreur(const char *m)
{
    write(2, m, strlen(m));
}

static void Notifier(const char *f, const char *m)
{
    messageErreur(f);
    messageErreur(" failed: ");
    messageErreur(m);
    messageErreur("\n");
}

static void Echouer(const char *f, const char *m, int r)
{
    Notifier(f, m);
    exit(r);
}
static int CopierDonnees(struct archive *ar, struct archive *aw)
{
    int r;
    const void *buff;
    size_t size;
#if ARCHIVE_VERSION_NUMBER >= 3000000
    int64_t offset;
#else
    off_t offset;
#endif

    for (;;) {
        r = archive_read_data_block(ar, &buff, &size, &offset);
        if (r == ARCHIVE_EOF)
            return (ARCHIVE_OK);
        if (r != ARCHIVE_OK)
            return (r);
        r = archive_write_data_block(aw, buff, size, offset);
        if (r != ARCHIVE_OK) {
            Notifier("archive_write_data_block()",
                archive_error_string(aw));
            return (r);
        }
    }
}


//Definition des fonctions de la classe Archives
Archives::Archives()
{
    nombreTotalPages=0;
}
Archives::Archives(std::string path)
         {
             CheminArchive=path;
             nombreTotalPages=0;
         }
std::string Archives::get_archive_path()
{
    return CheminArchive;
}
int Archives::get_page_number()
{
    return nombreTotalPages;
}
std::map <int,std::string> Archives::GetListeFichier()
{
    return ListeFichier;
}

//Definition des fonctions de la classe ArchiveExtraction

ArchiveExtraction::~ArchiveExtraction()
{}

ArchiveExtraction::ArchiveExtraction( ArchiveExtraction& ar1)
{
        CheminArchive=ar1.get_archive_path();
        nombreTotalPages=ar1.get_page_number();
}

ArchiveExtraction::ArchiveExtraction(std::string path1)
{

    CheminArchive=path1;
    nombreTotalPages=0;
}

void ArchiveExtraction::SetNombreTotalPages(int nombre)
{

    nombreTotalPages=nombre;
}
//Lire le contenu d'un fichier et remplir la liste des fichiers
void ArchiveExtraction::LireArchive(std::string PathArchive)
{

    ListeFichier.clear();
    const char *path_file = PathArchive.c_str();

   struct archive *arch;
   struct archive_entry *entry;//structure permet d'index chaque document dans archive

    int r;
    int counteurNmbrPages=0;

    arch = archive_read_new();//initialise arch pour la lecture
    archive_read_support_filter_all(arch);
    archive_read_support_format_all(arch);

    if (path_file != NULL && strcmp(path_file, "-") == 0) //path_file n'est pas null
        path_file = NULL;
    if ((r = archive_read_open_filename(arch, path_file, 10240)))// si on a bien ouvrir le file ou pas
        Echouer("archive_read_open_filename()",archive_error_string(arch), r);
    for(;;) {
        r = archive_read_next_header(arch, &entry);//lire chaque document dans l'archive

        if (r == ARCHIVE_EOF)
            break;
        if (r != ARCHIVE_OK)
            Echouer("archive_read_next_header()",archive_error_string(arch), 1);

                std::string file_name=(std::string)archive_entry_pathname(entry);
                //Avant d'ajouter le fichier dans le map, on s'assure qu'il est de type ".jpg" ou ".png" ou ".bmp".
                size_t PositionType = file_name.rfind(".");
                std::string TypeFichier(file_name.begin() + PositionType + 1, file_name.end()); // pour avoir type de fichieer cbr ou cbz
                std::transform(TypeFichier.begin(), TypeFichier.end(), TypeFichier.begin(), ::tolower); //transforme chaine type en miniscule
                if (TypeFichier == "png" || TypeFichier == "jpg" || TypeFichier == "bmp")
                {

                    //Ajouter Fichier dans map
                    ListeFichier.insert(std::pair<int,std::string>(counteurNmbrPages,file_name));
                    counteurNmbrPages+=1;
                }

    }
    SetNombreTotalPages(counteurNmbrPages);
    archive_read_close(arch);
    archive_read_free(arch);

}
//Cette fonction permet de décompresser une page spécifique d'une archive.
bool ArchiveExtraction::DecompresserArchive(int page_num,std::string path)
{
    if (page_num<0 || page_num>nombreTotalPages)
    {
        return false;
    }

    const char *archive_path_name = path.c_str();
    const char *file_path = archive_path_name;

    int flags,mode;
    mode = 'x';
    verbose = 0;
    flags = ARCHIVE_EXTRACT_TIME;

    switch (mode) {
    case 't':
        Extract(file_path, 0, flags,page_num);  //utilisation de la bibliothèque libarchive
        break;
    case 'x':
        Extract(file_path, 1, flags,page_num);  //utilisation de la bibliothèque libarchive
        break;
    }
    return true;

}

//Cette fonction utilise la bibliothèque de compression et de décompression libarchive.
void ArchiveExtraction:: Extract(const char *filename, int DoExtract, int flags,int page_num)
{

    struct archive *arch;
    struct archive *ext;
    struct archive_entry *entry;//indexer chaque document dans l'archive

    int r;
    int number_page_counter=0;

    arch = archive_read_new();

    ext = archive_write_disk_new();
    archive_write_disk_set_options(ext, flags);

    archive_read_support_filter_all(arch);//pour pouvoir lire tous les formats
    archive_read_support_format_all(arch);

    if (filename != NULL && strcmp(filename, "-") == 0)
        filename = NULL;
    if ((r = archive_read_open_filename(arch, filename, 10240)))// bonne ouverture du fichier
        Echouer("archive_read_open_filename()",archive_error_string(arch), r);

    for (;;)
    {
        r = archive_read_next_header(arch, &entry);//lire chaque document dans l'archive
        if(number_page_counter>page_num)
            break;
        if (r == ARCHIVE_EOF)//archive end of file
            break;
        if (r != ARCHIVE_OK)
            Echouer("archive_read_next_header()", archive_error_string(arch), 1);
        if (verbose && DoExtract)
            message("x ");
        if (verbose || !DoExtract)
            message(archive_entry_pathname(entry));
        if (DoExtract)
        {
             if(page_num==number_page_counter){
           r = archive_write_header(ext, entry);
          if (r != ARCHIVE_OK)
                Notifier("archive_write_header()",archive_error_string(ext));
            else {
                        CopierDonnees(arch, ext);//copier un seul fichier de l'archive dans disque
                        r = archive_write_finish_entry(ext);
                        break;
                        if (r != ARCHIVE_OK)
                            Echouer("archive_write_finish_entry()",archive_error_string(ext), 1);

                }
        }
        number_page_counter+=1;
        }
        if (verbose || !DoExtract)
            message("\n");
    }


    archive_read_close(arch);
    archive_read_free(arch);

    archive_write_close(ext);
    archive_write_free(ext);



}

//Cette fonction charge une image à partir d'un fichier à l'aide de la bibliothèque OpenCV

bool ArchiveExtraction::ChargerImage(int page_num,cv::Mat& image)
{

    std::string PathFile;
    if(page_num>nombreTotalPages || page_num<0 )
    {
        return false;
     }
    PathFile=std::filesystem::current_path().string();
    char *chaine= (char*) PathFile.c_str();
    for(size_t i=0;i<strlen(chaine);i++)
    {
        if(chaine[i]=='\\') PathFile[i]='/';
    }
    PathFile=PathFile +"/"+ListeFichier[page_num];
    DecompresserArchive(page_num,CheminArchive); //extraire l'archive contenant l'image correspondante.
    cv::Mat temp_image=cv::imread(PathFile,cv::IMREAD_COLOR); //Charge une image à partir d'un fichier et la stocker dans une variable cv::Mat "temp_image"

    if(temp_image.empty() == false)
    {
        cv::cvtColor(temp_image, image, cv::COLOR_BGR2RGB); //convertir le format d'image de BGR en RGB
        return true; //L'image est chargée avec succès
    }

    return false;
}


