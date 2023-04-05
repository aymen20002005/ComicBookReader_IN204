# ComicBookReader_IN204
Pour compiler ce programme, il faut utiliser Qt creator 9.0.1 sur windows 10(64 bit) basé sur
Qt 6.4.0 avec le compilateur MSVC 2019 64bit.

On vous a fourni dans ce projet, 3 dossiers essentiels à l'execution du programme:
opencv (essentiel à la lecture des fichiers .cbz et .cbr en les convertissant à des images d'une certaine format que peut lire opencv)
libarchive (essentiel aux compressions et décompressions des archives et à l"extraction des images)
QtQuick (essentiel à la partie QML du projet et donc à l'interaction avec l'UI du programme)

Une fois que vous avez cet environnement, vous ouvrez alors le fichier .pro :
Une fois que Qt est ouvert, changez dans ComicBookReader.pro comme suit : (ceci vous permettera d'accéder aux fichiers supplémentaires requis pour exécuter le programme)
INCLUDEPATH +=  <chemin vers le dossier comicBookReader>/opencv/include
LIBS += <chemin vers le dossier comicBookReader>/opencv/bin/libopencv_core454.dll
LIBS +=  <chemin vers le dossier comicBookReader>/opencv/bin/libopencv_imgcodecs454.dll
LIBS +=  <chemin vers le dossier comicBookReader>/opencv/bin/libopencv_imgproc454.dll
LIBS +=  <chemin vers le dossier comicBookReader>/opencv/bin/libopencv_highgui454.dll
LIBS +=  <chemin vers le dossier comicBookReader>/opencv/bin/libopencv_features2d454.dll
LIBS +=  <chemin vers le dossier comicBookReader>/opencv/bin/libopencv_calib3d454.dll

Il faut aussi copier le fichier archive.dll (il se trouve dans le dossier libarchive/bin) dans le même dossier que le dossier du build du projet.

Il faut aussi verifier si votre version de Qt contient QtQuick, sinon vous pouvez copier le dossier dans le dossier source de Qt.
