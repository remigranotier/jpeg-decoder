Pour décoder une image jpeg, faire "./bin/jpeg2ppm <chemin_vers_l'image>" dans
le répertoire racine. L'image .ppm résultat sera envoyée au même endroit que
l'image décodée.

Pour compiler l'éxecutable, lancer simplement la commande "make".

Si l'envie vous prend de compiler les fichiers de test, faites
"make test OBJET=<nom>" où <nom> est "idct" si l'on veut tester idct.c par
exemple.

Pour compiler un fichier .c en particulier en fichier .o, faire
"make obj OBJET=<nom>" où <nom> est "idct" si l'on veut tester idct.c par
exemple.

Pour supprimer l'éxecutable et les fichiers objets, lancer "make clean".

SI PROBLEME :
 -> Etape 1 : relisez les lignes ci-dessus
 -> Etape 2 : si il y a un problème étrange à la compilation,
 vérifiez que le dossier "obj" existe toujours : il est un peu
 capricieux et on ne sait pas pourquoi, des fois il se supprime de lui-même
 -> Etape 3 : vérifiez le nom de votre image, si vous entrez un nom invalide,
 ça ne marchera pas (segmentation fault)
