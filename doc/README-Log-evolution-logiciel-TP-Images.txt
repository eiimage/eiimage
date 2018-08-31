===========================================================================================
HISTORIQUE DU LOGICIEL IMAGE
-avant 2012 : logiciel IMAGE_EII : logiciel maison INSA-INFO/EII (Ivan Leplumey?)
destiné à l'enseignement : en C sous Visual sudio 2005
amélioré au cours du temps par stagiaires et enseignants (Véronique Coat, Olivier Deforges,...)

-année 2012-2013 : projet 4INFO encadé par Marie Babel
étudiants : Benoit Averty, Samuel Babin, Matthieu Bergère, Thomas Letan, Sacha Percot-Tétu, Florian Teyssier
objectif : interface graphique (QT) + librairie Imagein (C++) : bien conçu et générique
résultat : librairie DETIQ-T

-été 2013 : stage été Sacha Percot
objectif : refactoring IMAGE_EII sur la base du projet
réalisation : logiciel EIIMAGE intégrant Imagein et ajout de tous les menus de IMAGE_EII
en utilisant le code C existant pour les algos + principe des plugin

-rentrée 2013 : EIIMAGE utilisé en TP en EII
pb : perte sources correspondant à l'exe utilisé en TP

-hivers 2014-2015 : amélioration par Antoine Lorence, Ingé de recherche CDD
synchronisation souce/exe
correction de plusieurs bug
compilation sous CMake
dépot sous GitHub
changement de nom en ImageINSA (pour utilisation en dehors de l'INSA, à l'étranger)

-Eté 2016 : stage de Antoine Pazat:
des bugs corrigés
améliorations
pb: ne compile pas sous windows / pas de doc ormi rapprot très succint

-automne 2016 : patch par Alexandre Sanchez
(en raison modif de la version du compilateur de code-blocks)

-printemps 2017 : compilation sous windows + création environnement de dvt/compilatin
sur machine virtuelle par Alexandre Sanchez 

-été 2017 : stage 3INFO de Quentin Bigot :
correction amélioration des fonctionnalités + mode d'emploi compilation sur
machine virtuelle

-3 octobre 2017 : compilation de la verison TP-2017-2018 par Luce sur la machine virtuelle
32 bits et utilisation en TP COTR automne 2017

-décembre 2017 : utilisation ancienne version des squelettes plugin car la nouvelle version ne fonctionne plus

TODO : 
-plugin : identifier version à jour et voir lien avec le répertoire plugin dans les répertoire source de ImageINSA (Luce)
-machine virtuelle : nettoyage des répertoires sur la machine virtuelle (Alex)

-installation environnement développement sur machine virtuelle 64 bits, avec login TestingAdmin, 
en suivant le mode d'emploi de Antoine Laurence (GitHub)
-mise-à-jour documentation de l'installation de la machine virtuelle
-compilation avec login Tesing, sur machine virtuelle en 64 bits 
-mise à jour du mode d'emploi de Quentin Bigot
-github : merge de la branche qbigo sur le master
-créer les zip (sources, exe, trames-plugin) avec la version du master
-faire un mécanisme/processus (script?) de création de ces zip avec tag de la date de création
-réflexion sur l'archivage long terme pour maintenance (mise-à-jour régulière du logiciel)
élements de réflexion : 
*documents concernés : sources,trames plugin etudiants/enseignants, documents non évolutifs (rapports), 
modes d'emploi (contenant parfois des login et mdp), tableau des updates,zip récupérables pour les TP.
*lieux d'archivage potentiels : GitHub, répertoires locaux sur le serveur EII, GitLab, ftp,...
(Julien dans la boucle pour tous les points ci-dessus)
==============================================================================================
Décembre 2017 : pb de compilation des squelettes de plugin
Version code-Blocks en salle TP : 16.01
Version GCC (c'est ca qui compte) : ??

Squelettes plugin:
version moodle COTR : compile (vieux cpb 12/2014, vieux algo 2011-2012)
version moodle AI : compile pas (nouveau cpb 11/2016, nouveau algo 2015-2016)

version : trames-plugins-tp-2017-12-12.zip
utilisation vieux cpb (12/2014) et nouveau algo (2015-2016)

=> mise sur moodle-COTR et moodle AI et public/TPCOTR
=> nouvelle version de ImageINSA (sources + exe) mise sur moodle AI
=> pas mis les nouveaux zip sur Serveur EII:\Logiciels\Windows (pas les droits)
===============
Stage de Quentin Bigot
-Bilan des bugs présents dans la version TP et dans la version apazat 
-Création d'un environnement de développement et compilation de ImageINSA sur machine virtuelle
-Ecriture d'un guide de compilation + script pour compilation facile par les enseignants
-Intégration des modifications de apazat et nouvelles corrections

================
Avril 2017
Test de Alexandre Sanchez :
-la version "apazat" s'execute sous linux et windows 
-les trames de plugin code-blocks ne compilent pas avec la nouvelle version "apazat" 
-la version "alorence" (donc celle utilisée en TP en 2016-2017) 
-fonctionne parfaitement sur Windows7 64bits.Y compris la compilation des plugins. 
==================
fin 2016
Alexandre Sanchez a récupéré et compilé sous linux la version de Antoine Pazat
==================
Octobre 2016
Alexandre Sanchez a modifié quelque chose (?) pour pouvoir compiler les plug-in 
avec la nouvelle version de code-block 
==================
Debut 2016
ajout modif souhaitées (issues forum enseignant sur moodle) dans fichier update
===================
Octobre 2015
module COMP
-modification du plugin DPCM squelette et solution => fichiers dpcm-squelette2 et dpcm-solution2
integres dans le zip sur public/2015-2016/COMP
-quelques bugs notés dans le logiciel ImageINSA

=========================================
Début 2015:
information après mis à joru du logiciel par Antoine Lorence : 
-restoration des sources correspondantes a l'exe
-mise sur GitHub

Tout est sur Github à l'adresse https://github.com/eiimage/eiimage/releases
Antoine a archivé des zip et les a associés à une release (= un tag git). 
Il y a :
- Une version compilée pour windows : ImageINSA-Win32-2014-12-18.zip
- Un dossier avec les projets Code::Blocks à fournir aux étudiants pour le développement de plugins : trames-plugins-tp.zip
- Un dossier avec toutes les sources ImageInsa + Detiq-T : sources_ImageINSA_AND_Detiq-T.zip"

Une copie de ces dossier est sur le serveur enseignement Public:
Y:\Logiciels des salles de TP\Analyse d'images (répertoire non accessible aux étudiants)


====================================================
CONTENU DU REPERTOIRE

1-UTILES:

images: toutes les images utilisées en TP

plugin-fournis-etudiants : squelettes plugins
plugin-version-enseignant : solutions plugin + tests en cours

Log Eiimages 2012-11-29 : sources (sans les corrections faites par Sacha et Marie le 11/10/2012) + exe + rapport sacha + rapport projet 4INFO 
Eiimage-exe-salle-101 : exe utilisé en salle TP 101 : exe de la version avec les corrections faites par Marie et Sacha le 11/10/2012
(les sources n'ont pas été retrouvées) **

2-ARCHIVES:
Eiimage.zip : archive datant du 29/11/2012 (sources sans les corrections)
LogImages 2012-11-29-old-incomplet : executable
eiimage10.12.2012 : executable (qui date en fait du 10 octobre et non du 10 décembre! erreur de nommage)
LogImages 2012-11-29 : archive faite par Sacha (même contenu que Log Eiimages 2012-11-29 sans le rapport detiq-t)
=================================================
PLUGIN

Les plugin fournis aux étudiants contiennent des dll deja compilées qui peuvent être 
chargées et exécutées dans le logiciel (mais fournissent des résultats faux, bien sûr).

(pb : aucun exemple de plugin ne compile ni sur vieux PC, ni sur le nouveau
=> à voir si compile en salle de TP)

Entropy :
squelette : fait 
correction : fait

DPCM
squelette : fait
correction : en cours 
voir code dans DPCMtestLuce, à placer dans le répertoire plugins

WU
squelette entièrement vide : fait (dans plugin DPCM)

Estim mvt:
rien fait

Exemples de plugins:
Eiimage\eiimage\plugins\Segmentation
Eiimage\eiimage\plugins\Entropy

=> Entropy et Segmentation et SegmentationTest marchent


Consignes pour les étudiants
*récupérer (via moodle,fileZ,public)
le squelettePlugin-Entropy-etudiants.zip
Eiimage.zip

*Placer le répertoire Entropy contenant le projet Code::Blocks à modifier dans \Eiimage\eiimage\plugins

*Compléter le code.

*Compiler pour générer un fichier Entropy.dll dans Entropy\bin\Release

*Lancer application "eiimage.exe", charger le plug-in en sélectionnant le fichier Entropy.dll généré 

=============================================================
doc en ligne

http://www.cplusplus.com/reference
===============================================

===== echanges de mail
** voir le MAIL de SACHA du 22/07/2013



