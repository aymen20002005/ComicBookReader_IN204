#include "archive_extraction.hpp"

Archives::Archives()
{
    nombreTotalPages=0;
}
//---------------------------------------------------------------------------
Archives::Archives(std::string path)
         {
             CheminArchive=path;
             nombreTotalPages=0;
         }
//----------------------------------------------------------------------------
std::string Archives::get_archive_path()
{
    return CheminArchive;
}
//----------------------------------------------------------------------------
int Archives::get_page_number()
{
    return nombreTotalPages;
}
//----------------------------------------------------------------------------
std::map <int,std::string> Archives::GetListeFichier()
{
    return ListeFichier;
}
