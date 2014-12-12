
# TODO List

Cette liste rassemble les améliorations et corrections qui pourraient ou devraient être apportées à ImageINSA.

## Modifications prioritaires

- Croissance de régions : fait planter le logiciel : pb d’initialisation d’après Franck (corrigé dans son soft)
- DPCM
    - Pouvoir faire du sans pertes : case à cocher qui donne une loi de quantification identité
- Quantification
    - Pouvoir quantifier une image double : actuellement, pas de bouton « apply » pour les images de double
- Filtrage
    - Normalisation : pouvoir cocher / décocher et choisir le facteur (par défaut, somme des valeurs du filtre)
- Pyramid
    - Changer « Step » en « level/niveau » dans l’interface
    - Expliciter que le menu reconstruction suppose une pyramide Laplacienne (l’appeler reconstruction Laplacienne ou ne pas proposer les choix autres que Laplacien à la reconstruction)
- Opérations sur les images
    - Principe général lorsqu’une opération génère des valeurs en dehors de l’intervalle [0..255] à décider, justifier et à documenter dans la doc utilisateur et programmeur. possibilités :
        - Clipper sur 0..255 : c’est ce qui est fait actuellement
Avantage : on peut enchainer les opérations sans étape de conversion intermédiaire
Inconvénient : on perd les valeurs de sortie, et leur sens
        - Conserver les valeurs calculées en sortie et proposer un affichage éventuellement avec clipping ou recalage, de façon à pouvoir enchaîner les étapes de calculs.
Avantage : on conserve les vraies valeurs
Inconvénient : il faut éventuellement rajouter des étapes intermédiaires de clipping, warping pour enchainer certaines opérations qui prennent des images [0..255] en entrée
=> proposition : faire le mode sale et facile par défaut et pouvoir le désactiver :
avoir un bouton coché par défaut qui fait le clipping, et si on le décoche, on conserve les valeurs en sortie

- Affichage des images
    - Si une image (double ou int) contient des valeurs négatives, proposer dans l’interface d’affichage le décalage sur 128 ou clipping des valeurs négatives (par défaut..) ou valeur absolue, mais bien sur l’histogramme doit etre fait sur les valeurs réelles.

## Autres modifications

- Général
    - Ajouter dans le menu Help une entrée pour afficher un compte rendu des raccourcis clavier utilisables
    - pouvoir utiliser Ctr-C Ctrl-V pour pouvoir récupérer les images dans un compte-rendu
    - traduction en anglais de « troncature » dans le menu
- Pixel Grid
    - Pixel grid pas à la bonne taille : il faut redimensionner la fenêtre pour pouvoir observer la dernière ligne et la dernière colonne : il faudrait la dimensionner directement à la taillle de l’image.
- Histogramme
    - Pouvoir zoomer sur l’histogramme (en particulier pour pouvoir fixer intercativement un seuil)
    - Histo sur une image double : pouvoir choisir la taille des bins
    - Histo cumulé pour les images double
    - Clic droit : projection histogram et line/column profile/profile histogram : résultat bizarre => à revoir.
Column profil = histogramme sur les pixels de la colonne => le renommer « Current column Histogram »
éclaircir ce qui est fait dans le logiciel, et faire un  « vrai profil » : ie somme des valeurs sur la ligne/colonne
    - Calcul de l’histogramme sur zone sélectionnée et dynamiquement pour les images double

- Scaling :
    - Option spline : donne le même résultat que le plus proche voisin => vérifier l’implémentation de l’interpolation par spline
    - Soustraction : ne pas recaler sur 128 l’image de sortie : la conserver en mémoire avec des valeurs négatives (la gérer comme une image double, avec des valeurs quelconques, et pouvoir cumuler les opérations, comme après un filtrage)
D’après le code (pointOp.cpp, ligne 280) :
 - si une des 2 images d’entrée est en double, le résultat est en double
Du coup : si on soustrait 2 images Grayscale, le résultat sera un Grayscale
test à revoir : le supprimer et sortir toujours des images en double (quitte à ce que l’utilisateur fasse un « convert to Grayscale » ensuite)
    - Conversion : compléter avec
        - Double => Grayscale
        - Double=> int (signé)
        - Proposer plusieurs modes de conversions : clipping (défaut), linear stretch, offset,
- Huffman
    - Afficher les valeurs dans l’ordre de probabilité croissante dans la fenêtre de log
- Structure de donnée Image
    - Pouvoir sauvegarder une image double
- Morphomath
    - Améliorer l’ergonomie pour clarifier que la définition de l’élément structurant, sert pour toutes les opérations : voir si il faut fusionner avec le menu DMM qui redonne toutes les opérations et une autre façon de définir l’élément structurant, et qui permet de faire un élément arbitraire en cliquant tous les points indépendemment.
- Classification
    - Clarifier comment sont prises en compte les interactions utilisateur (en particulier influence ou pas de l’ordre dans lequel sont sélectionnées les zones de l’image lors de la définition des classes et lors de du calcul des scores de classif) => voir avec Joseph si c’est lui qui a conçu l’exercice, il aura les idées claires.
- Cas de plantage :
    - Lors de la fermeture des images…. Circonstances exactes ?

- simuler des erreurs de transmission sur le DPCM (ajout bruit impulsionnel)
- pouvoir modifier le nom des images dans l'interface
- faire une fenêtre d'information sur les raccourcis clavier
- implémenter le filtre médian dans le menu filtres
- DPCM : ajouter une option sans quantification (est-ce utile?)
- fonction Entropy : pouvoir l'utiliser sur une image d'erreur (avec valeurs signées), ou sur une image double (du coup en incluant la quantification)
- quantification customizée : ne pas limiter les valeurs dans l'intervalle[- 255,255]
- quantification : la rendre possible sur une image d'entrée signée entière ou sur une image double
- revoir l'IHM des lois de quantification custumisées (séquence d'actions pas intuitive):
proposition : faire 3 boutons : appliquer / sauver loi / charger loi
-  rajouter une fonctionalité de conversion explicite : double <- > int (avec différents modes)
-  question : faut- il créer une structure de données pour les images à valeurs entières signées?
ou est- ce qu'il faut continuer à les gérer comme des images double?
=> alorence 11/12/2014: Je pense qu'il faut garder la structure image double pour les images avec des int signés, mais qu'il faudrait faire une passe sur tout le logiciel pour que les images double puissent être prises en compte partout où c'est possible.

## Idées pour TP

- profil ligne : pour le TP sur l’analyse des documents
- profiter du calcul dynamique de l’histogramme sur une zone sélectionnée