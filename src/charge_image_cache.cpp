#include "charge_image_cache.hpp"
#include <QMutexLocker>

ChargerImageCache::ChargerImageCache()
{
    page_modified = false;
    image_precharge = false;
}
//---------------------------------------------------------------------------------------------------
ChargerImageCache::ChargerImageCache(Images& imageT)
{
    imageTraitement=&imageT;
    page_modified = false;
    image_precharge = false;
}
//-----------------------------------------------------------------------------------------------
Images* ChargerImageCache:: GetTraitementImg(){

    return imageTraitement;

}
//-----------------------------------------------------------------------------------------------
void ChargerImageCache::SetTraitementImg(Images& imageT)
 {
     imageTraitement=&imageT;
 }

//----------------------------------------------------------------------------------------------------

void ChargerImageCache::MettreImageDansCache(const int NumPage, const int TypePage)
{
    DonneeImage* image_pointer;

    VerrouillerCache.lockForRead();
    //Vérifier que l'image n'existe pas dans le cache
    if(CacheImage.contains(NumPage))
    {
        image_pointer = CacheImage.object(NumPage);
        if(image_pointer->TypeImage!=TypePage)
        {
            VerrouillerCache.unlock();
            VerrouillerCache.lockForWrite();
            CacheImage.remove(NumPage);
            VerrouillerCache.unlock();
        }
       else{
         VerrouillerCache.unlock();
        }

    }
    else {//Si l'image n'existe pas dans le cache
        VerrouillerCache.unlock();
        image_pointer = new DonneeImage;
         imageTraitement->TraiterImage(NumPage, TypePage,*(image_pointer->PointeurImage));
         image_pointer->TypeImage = TypePage;
          VerrouillerCache.lockForWrite();
          CacheImage.insert(NumPage,image_pointer);
         VerrouillerCache.unlock();
}
}

//Fonction appelée en parallèle pour charger les images de manière asynchrone
void ChargerImageCache::ChagerParalleleImage()
{

    while (1)
    {

        std::unique_lock <std::mutex> lck(PrechargementMutex);
        EtatPrechargement.wait(lck);

        if(SortirProgramme == true)
        {
            return;
        }
        if (CheminArchiveChange == true) {
            imageTraitement->ChargerArchive(CheminGlobalArchive,0);
            CheminArchiveChange = false;
        }
        image_precharge = true;

        //Declaration des variables qui nous renseignent sur l'état des images (si elles sont chargées ou non)
        bool left_exceed = false;
        bool right_exceed = false;
        int type = ParametresChargement.TypePage;
        int page_num = ParametresChargement.page_num;
        int left_page_number = ParametresChargement.left_page_number;
        int right_page_number = ParametresChargement.right_page_number;


        //Boucle pour charger les images à droite et à gauche
        for (int i = 1; i <= qMax<int>(left_page_number, right_page_number); i++)
        {


            if (page_num + i <= nbr_page && i <= right_page_number)
            {
                MettreImageDansCache(page_num + i, type);
            }
            else right_exceed = true;

            if (page_num - i >= 1 && i <= left_page_number)
            {
                MettreImageDansCache(page_num - i, type);
            }
            else left_exceed = true;

            if (left_exceed == true && right_exceed == true)
            {

                break;
            }

        }
        image_precharge = false;
    }
    return;


}

