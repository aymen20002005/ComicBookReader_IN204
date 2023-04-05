#include "image_provider.hpp"



ImgProvider::ImgProvider(): QQuickImageProvider(QQuickImageProvider::Image)
{//constructeur avec iniatilisation du thread pour préchargement en parallèle des pages

    path = false; //Cette variable sera utilisée pour savoir si un chemin d'accès pour les images a été défini ou non.

    CheminArchiveChange = false;

    nbr_page = 0; //Cette variable sera utilisée pour stocker le nombre total de pages d'images.

    ThreadChargement = std::thread(&ChargerImageCache::ChagerParalleleImage, &ChargerDansCache);
    CacheImage.setMaxCost(100);
}
//Destructeur qui vide le cache aussi
ImgProvider::~ImgProvider(){
    SortirProgramme = true;
    CacheImage.clear();
    EtatPrechargement.notify_all();
    ThreadChargement.join();
}
//Initialiser les paramètres de préchargement d'une image
void ImgProvider::prechargement_image(int page1)
{
    ParametresChargement.page_num=page1;
    ParametresChargement.left_page_number=3;
    ParametresChargement.right_page_number=3;
    ParametresChargement.TypePage=1;
    EtatPrechargement.notify_all();
}


QObject* ImgProvider::object_property()
{
    return rootObjectPointeur;
}

//prend en paramètre un pointeur vers un objet de type "cv::Mat" (une image dans le format OpenCV Mat),
//et retourne un objet QImage.
QImage ImgProvider::ConvertirCvMatEnQimage(const cv::Mat* image)
{
    return QImage((uchar*)image->data, image->cols, image->rows, image->step, QImage::Format_RGB888) ;
}


//fournir une nouvelle image au qml
QImage ImgProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{

    QStringList shape = id.split("/");
    int page_number = shape[0].toInt();
    if(path==false)
    {
        return QImage();  //renvoie une image vide si le path est faux
    }

    DonneeImage* imagePtr;
    VerrouillerCache.lockForRead();
    if(CacheImage.contains(page_number))
    {
        imagePtr=CacheImage.object(page_number);
        VerrouillerCache.unlock();
        prechargement_image(page_number);
        return ConvertirCvMatEnQimage(imagePtr->PointeurImage);
    }
    else
    {
        VerrouillerCache.unlock();
        imagePtr= new DonneeImage;
        TraitementImg.TraiterImage(page_number,1,*(imagePtr->PointeurImage));
        imagePtr->TypeImage=1;
        VerrouillerCache.lockForWrite();
        CacheImage.insert(page_number,imagePtr);
        VerrouillerCache.unlock();
        prechargement_image(page_number);
        return ConvertirCvMatEnQimage(imagePtr->PointeurImage);
    }


}
//-------------------------------------------------------------
void ImgProvider::update_path(const QString &p)
{
    //recuperation du chemin de l'archive choisi
    QStringList listeChemin = p.split("///");
    CheminGlobalArchive = listeChemin[1].toStdString();

    //chargement
    TraitementImg.ChargerArchive(CheminGlobalArchive,0);
    TraitementImg.GetArchive()->LireArchive(CheminGlobalArchive);
    ChargerDansCache.SetTraitementImg(TraitementImg);

    VerrouillerCache.lockForWrite();
    CacheImage.clear();    //suppression des images précédemment stockées dans le cache
    VerrouillerCache.unlock();

    path = true; //le chemin de l'archive a été défini.

    nbr_page = TraitementImg.GetArchive()->get_page_number();
    QObject* slidbar_ptr = rootObjectPointeur->findChild<QObject*>("SlideBar");
    slidbar_ptr->setProperty("to", nbr_page-1);

    QObject* show_image_ptr = rootObjectPointeur->findChild<QObject*>("ShowImage");
    show_image_ptr->setProperty("pageNum", 0);

}


//Cette fonction recherche un objet QObject nommé "SlideBar" à partir de rootObjectPointeur et
//définit sa propriété "to" à 0.
void ImgProvider::object_property(QObject* Pt)
{
    rootObjectPointeur = Pt;
    QObject* slidbar_ptr = rootObjectPointeur->findChild<QObject*>("SlideBar");
    slidbar_ptr->setProperty("to",0);
}
