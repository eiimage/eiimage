# Informations

## Compilation

*ImageINSA* se compose de 4 modules de compilation:
 1. *ImageIn*, librairie de traitement d'images
 2. *GenericInterface*, interface graphique pour manipuler les images
 3. *core*, librairie principale fournissant le coeur de métier de *ImageINSA*
 4. *ImageINSA*, executable final

## Dépendances internes
 - *GenericInterface* dépend de *ImageIn*
 - *core* dépend de *ImageIn* et de *GenericInterface*
 - *ImageINSA* dépend de *core*, de *ImageIn* et de *GenericInterface*

## Contraintes

*core* doit être compilée en tant que librairie dynamique (dll sous Windows), afin de pouvoir être réutilisée comme base pour la compilation de plugins additionnels à *ImageINSA*

## Le problème

Les dépendances entre les modules compliquent la compilation. Si *ImageIn* et *GenericInterface* sont des librairies statiques, et *core* une librairie dynamique, au moment du link de *ImageINSA*, plusieurs méthodes de *ImageIn* et *GenericInterface* sont définis en double (celles qui sont utilisées à la fois par *ImageINSA* et dans *core*).

## Solution choisie

Dans CMake, *ImageIn* et *GenericInterface* sont définies commes des librairies 'object'. Chaque fichier source est compilé, mais n'est pas linké. Le résultat est une liste de fichiers objets (*.obj sous Windows) qui peut être utilisée comme source pour la compilation de *core*. Le resultat est une librairie *core* qui est dynamique et qui contient l'intégralité de *ImageIn* et *GenericInterface*. Le link de *ImageINSA* ne repose plus sur *ImageIn* et *GenericInterface*, mais seulement sur *core*

Voir les commits 86db68e9ffae de eiimage et 9ba87b46e de Detiq-T

## Autres solutions possibles

 - Faire en sorte que tous les modules soient des librairies dynamiques
 - Réarchtecturer *ImageINSA* et refaire un découpage dont la logique suit les relations de dépendances entre les modules plutôt que l'historique du logiciel.
