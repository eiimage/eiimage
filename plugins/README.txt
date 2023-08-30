PLUGIN : mode d'emploi pour générer et utiliser un plugin à patir des trames fournies

1 - Récupérer les 3 archives
	+ trames-plugins-AAAA-MM-JJ.zip : contient plungins/ et le fichier README.txt que vous êtes en train de lire
	+ ImageINSA-Win32-AAAA-MM-JJ.zip : contient ImageINSA/ : c'est la version la plus récente possible de ImageINSA, compilé pour Windows
	+ sources_ImageINSA_AND_Detiq-T.zip : contient sources/ : ce sont les sources de ImageINSA

2 - Décompresser dans un dossier
Voici à quoi doit ressembler le dossier de travail :
<ce dossier>
	├── ImageINSA
	├── plugins
	│   ├── Convolution
	│   ├── DPCM
	│   ├── Entropy
	│   ├── GammaCorrection
	│   └── Median
	└── sources
ATENTION! Le nom des différents dossiers est important et doit être respecté, de même que leurs positions respectives dans l'arborescence
	
3 - Compilation des squelettes
Dans chacun des 5 dossiers de plugins, il y a un fichier CMakeLists.txt.
- ouvrir ce fichier dans CLion et lancer la compilation. 
- les DLL ainsi compilées seront crées dans les dossiers DPCM/cmake-build-release ou cmake-build-debug de chacun des projets, suivant le type de compilation choisi
Au besoin vérifier que CLion est bien configuré pour utiliser le compilateur MinGW version 32 bits (C:\mingw32).
(menu Files/Settings/Build/Toolchains/)
Si vous ne disposez pas de compilateur 32 bits, vous pouvez télécharger "MinGW-W64 GCC-8.1.0 - i686-win32-sjlj"

4 - Chargement et exécution d'un plugin
- lancer imageinsa.exe
- dans le menu "plugin", sélectionner "load plugin"
- sélectionner le fichier dll généré:
	+ DPCM.dll généré dans DPCM/cmake-build-debug
	+ Entropy.dll généré dans Entropy/cmake-build-debug
	+ etc...
- un nouveau menu apparait que vous pouvez utiliser 

5 - Modifier les sources du projet CLion
	+ compilation (nécessite de décharger le plugin)
	+ chargement
jusqu'à convergence vers le bon résultat...!
