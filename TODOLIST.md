
# TODO List

Cette liste rassemble les améliorations et corrections qui pourraient ou devraient être apportées à ImageINSA.
La majorité de ces actions sont également retrouvable dans les issues Git.

## Modifications prioritaires

- DPCM
    - Pouvoir faire du sans pertes : case à cocher qui donne une loi de quantification identité
- Quantification
    - Pouvoir quantifier une image double : actuellement, pas de bouton « apply » pour les images de double
    - quantification customizée : ne pas limiter les valeurs dans l'intervalle[- 255,255]
- Pyramid
    - Changer « Step » en « level/niveau » dans l’interface
    - Expliciter que le menu reconstruction suppose une pyramide Laplacienne (l’appeler reconstruction Laplacienne ou ne pas proposer les choix autres que Laplacien à la reconstruction)
-  Rendre possible toutes les opérations pour les images doubles 

## Autres modifications

- Général
    - Ajouter dans le menu Help une entrée pour afficher un compte rendu des raccourcis clavier utilisables
    - pouvoir utiliser Ctr-C Ctrl-V pour pouvoir récupérer les images dans un compte-rendu
    -  rajouter une fonctionalité de conversion explicite : int -> double (dans le style de la conversion en Uchar)
- Histogramme
    - Pouvoir zoomer sur l’histogramme (en particulier pour pouvoir fixer intercativement un seuil)
    - Histo sur une image double : pouvoir choisir la taille des bins
    - Histo cumulé pour les images double
- Huffman
    - Afficher les valeurs dans l’ordre de probabilité croissante dans la fenêtre de log
- Structure de donnée Image
    - Pouvoir sauvegarder une image double
- Morphomath
    - Améliorer l’ergonomie pour clarifier que la définition de l’élément structurant, matérialiser visuellement le centre de l'élément et pouvoir le déplacer intéractivement
- Classification
    - Clarifier comment sont prises en compte les interactions utilisateur (en particulier influence ou pas de l’ordre dans lequel sont sélectionnées les zones de l’image lors de la définition des classes et lors de du calcul des scores de classif) => voir avec Joseph si c’est lui qui a conçu l’exercice, il aura les idées claires.
- Cas de plantage 
    - Lors de la fermeture des images (arrive rarement circonstance et raison inconnue...)
- DPCM 
    - simuler des erreurs de transmission (ajout bruit impulsionnel)
- LogScale 
    - Revoir le fonctionnement de la logScale dans les images doubles pour rendre son utilisation plus clair. Et améliorer le contraste apporté.
- Profile Ligne/Colonne 
    - Dans l'affichage de Profile Ligne/Colonne, permettre à l'utilisateur de sélectionner dynamiquement la ligne à observer.



## Idées pour TP

- profil ligne : pour le TP sur l’analyse des documents
- profiter du calcul dynamique de l’histogramme sur une zone sélectionnée