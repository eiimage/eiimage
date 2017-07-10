#include "Croissance.h"
#include <RandomLib/Random.hpp>
#include <GrayscaleImage.h>
#include <RgbImage.h>
#include <QColor>

using namespace std;
using namespace imagein;
using namespace RandomLib;

Croissance::Croissance()
{
	tabin = NULL;
	tabout = NULL;
	tablabel = NULL;
	MoyCell = NULL;
	NbPointCell = 0;
	tab_min_ij = NULL;
}

Croissance::~Croissance()
{
}

int Croissance::croissance1a( const Image *im, int threshhold, Image **luminance, Image **colorRgn ) {

   // Random random;
    NbPointCell=0;

    int seuil;
    int numregion;
    long nbpregion;
    int lum = 0;
    float somlum;
  
    seuil = threshhold;
    nbc = im->getWidth(); // nombre de colonnes de l'image d'entrée
    nbl = im->getHeight(); // nombre de lignes de l'image d'entrée
    size = nbc * nbl; // taille de l'image

//    BasicImageFun bif;
    tabin = im->begin();

    tabout = new Image::depth_t[size*sizeof(Image::depth_t)]; // tableau de valeurs pour l'image de sortie (luminance) [0 ; size[
    tablabel = new int[size*sizeof(int)]; // tableau des numéros de régions pour chaque pixel de l'image [0 ; size[
    MoyCell = new int[size*sizeof(int)]; // tableau des valeurs moyenne des pixels par régions [0 ; numregion[
		
//	 initialisation de tablabel a zero (nécessaire ?)
	for (int i = 0; i < size; i++) {
		tablabel[i] = 0;
	}

	// initialisation des paramètres
    numregion=0 ; // aucune région créée au début de l'algorithme
    nbpregion=0 ; // aucun pixel dans la région courante au début de l'algorithme
    somlum=0; // luminance cumulée des pixels dans la région couratne nulle au début de l'algorithme

    for(int i=0 ; i<nbl ; i++)
        for(int j=0 ; j<nbc ; j++)
        {
            if(tablabel[i*nbc+j] == 0) // si le pixel courant n'appartient pas a une region
            {
				// On cree une nouvelle region
                numregion++;
                MoyCell[numregion] = 0;
                pushItem(i,j,seuil,numregion,somlum,nbpregion,lum);//on ajoute le pixel dans la pile des candidats (ce sera la seed)
                while( !croi_stack.empty() ) {
                    parcours_parcelle1A(); //ajoute les pixels qui valident le critère dans la région, et les pixels voisins dans la pile des candidats
                } // while : on sort de cette boucle quand la région est complète (pile de candidats vide)
				//numregion++;
                MoyCell[numregion] = MoyCell[numregion]/NbPointCell; // calcul de la luminance moyenne des pixels dans la région
            } // if : fin de la création de la région liée au pixel courant
			// on réinitialise les paramètres pour la création de la prochaine région
            NbPointCell = 0;
            somlum=0;
            nbpregion=0;
			// on ne réinitialise pas lum ? (entier passé en paramètre pour la création d'un stackitem)
        }// for : parcours de l'image d'entrée
	
	// remplissage de tabout
	for (int i = 0; i < size; i++)
		// chaque pixel de l'image de sortie prend la valeur de la valeur moyenne des pixels de la région à laquelle il appartient
		 tablabel[i] < size ? tabout[i] = MoyCell[tablabel[i]] : tabout[i] = 0;
	//	tabout[i] = MoyCell[tablabel[i]];

	// Construction de l'image de sortie en luminance (en utilisant les valeurs de tabout)
    *luminance = new GrayscaleImage(nbc, nbl, tabout);

	RgbImage* ic = new RgbImage(nbc, nbl);
	for (int j = 0; j<nbl; j++) {
		for (int i = 0; i<nbc; i++) {
			int n = tablabel[j*nbc + i] - 1; /* € [0, numregion[ */
			const int nhue = 360;
			const int ngrad = ceil((double)numregion / (double)nhue);
			const int hue = floor(n * nhue / numregion); /* € [0, nhue[ */
			const int grad = n - ceil((double)hue * (double)numregion / (double)nhue); /* € [0, ngrad[ */
			if (grad < 0 || grad >= ngrad) {
				cout << "grad = " << grad << endl;
			}
			QColor color = QColor::fromHsl(hue, 255, (grad + 1) * 255 / (ngrad + 1));
			ic->setPixel(i, j, 0, color.red());
			ic->setPixel(i, j, 1, color.green());
			ic->setPixel(i, j, 2, color.blue());
		}
	}

    *colorRgn = ic;


    tabin = NULL;
    delete[] tabout;
    delete[] tablabel;
    delete[] MoyCell;
    tabout = NULL;
    tablabel = NULL;
    MoyCell = NULL;

    return numregion;
}

void Croissance::parcours_parcelle1A()
{
     if( croi_stack.empty() ) {
        throw("Error in Croissance::parcours_parcelle1B:\ncroi_stack.empty() = TRUE");
    }

	// on dépile un élément
    croi_stackitem csi;
    csi = croi_stack.back();
    croi_stack.pop_back();

    int i = csi.i;
    int j = csi.j;
    int seuil = csi.seuil;
    int numregion = csi.numregion;
    float somlum = csi.somlum;
    int nbpregion = csi.nbpregion;
    int lum = csi.lum;

	// si le pixel dépilé est le premier pixel de la region
    if(nbpregion == 0)
    {
		// on ajoute le pixel a la region
        tablabel[i*nbc+j] = numregion;
        MoyCell[numregion] += tabin[i*nbc+j] ;
        NbPointCell ++ ;
        somlum += tabin[i*nbc+j];
        lum = tabin[i*nbc+j] ; // remplace la valeur de csi.lum (=0 au début de l'algorithme, puis a la valeur précédente de tabin[i*nbc+j]
        nbpregion++;

		// on empile les pixels voisins (s'ils existent)
        if(j<nbc-1) pushItem(i,j+1,seuil,numregion,somlum,nbpregion,lum);
        if(i<nbl-1) pushItem(i+1,j,seuil,numregion,somlum,nbpregion,lum);
        if(j>0) pushItem(i,j-1,seuil,numregion,somlum,nbpregion,lum);
        if(i>0) pushItem(i-1,j,seuil,numregion,somlum,nbpregion,lum);
    }
	// si le pixel dépilé est candidat
    else if(fabs((double)(tabin[i*nbc+j]-somlum/nbpregion)) < (double)(seuil) && tablabel[i*nbc+j]==0) // | current - mean | < threshold && pixel n'appartient pas a une région
    {
		// on ajoute le pixel a la region (s'ils existent)
        tablabel[i*nbc+j] = numregion;
        MoyCell[numregion] += tabin[i*nbc+j] ;
        NbPointCell ++ ;
        somlum += tabin[i*nbc+j];
        nbpregion++;

		// on empile les pixels voisins (si on est pas au bord de l'image)
        if(j<nbc-1) pushItem(i,j+1,seuil,numregion,somlum,nbpregion,lum);
        if(i<nbl-1) pushItem(i+1,j,seuil,numregion,somlum,nbpregion,lum);
        if(j>0) pushItem(i,j-1,seuil,numregion,somlum,nbpregion,lum);
        if(i>0) pushItem(i-1,j,seuil,numregion,somlum,nbpregion,lum);
  }
}

int Croissance::croissance1b( const Image *im, int threshhold, Image **luminance, Image **colorRgn  ) {
    Random random;
    NbPointCell=0;

    int seuil;
    int numregion;
    long nbpregion;
    int lum = 0;
    float somlum;

    seuil = threshhold;
    nbc = im->getWidth();
    nbl = im->getHeight();
    size = nbc * nbl;

//    BasicImageFun bif;
    tabin = im->begin();

    tabout = new Image::depth_t[size];
    tablabel = new int[size];
    MoyCell = new int[size];

    numregion=1 ;
    nbpregion=0 ;
    somlum=0;

    for(int i=0 ; i<nbl ; i++)
        for(int j=0 ; j<nbc ; j++)
        {
            if(tablabel[i*nbc+j] == 0)
            {
                MoyCell[numregion] = 0;
                pushItem(i,j,seuil,numregion,somlum,nbpregion,lum);
                while( !croi_stack.empty() ) {
                    parcours_parcelle1B();
                }
                MoyCell[numregion] = MoyCell[numregion]/NbPointCell;
                numregion++;
            }

            NbPointCell = 0;
            somlum=0;
            nbpregion=0;
        }

    for(int i=0 ; i<size ; i++)
        tabout[i] = MoyCell[tablabel[i]];



    *luminance = new GrayscaleImage(nbc, nbl, tabout);

    RgbImage* ic = new RgbImage(nbc, nbl);
    for(int j=0 ; j<nbl ; j++) {
        for(int i=0 ; i<nbc ; i++) {
            int n = tablabel[j*nbc + i] - 1; /* € [0, numregion[ */
            const int nhue = 360;
            const int ngrad = ceil((double)numregion / (double)nhue);
            const int hue = floor(n * nhue / numregion); /* € [0, nhue[ */
            const int grad = n - ceil((double)hue * (double)numregion / (double)nhue); /* € [0, ngrad[ */
            if(grad < 0 || grad >= ngrad) {
                cout << "grad = " << grad << endl;
            }
            QColor color = QColor::fromHsl(hue, 255, (grad + 1) * 255 / (ngrad + 1));
            ic->setPixel(i, j, 0, color.red());
            ic->setPixel(i, j, 1, color.green());
            ic->setPixel(i, j, 2, color.blue());
        }
    }

    *colorRgn = ic;


    tabin = NULL;
    delete tabout;
    delete tablabel;
   // delete MoyCell;
    tabout = NULL;
    tablabel = NULL;
    MoyCell = NULL;

    return numregion;
}

void Croissance::parcours_parcelle1B() {
     if( croi_stack.empty() ) {
        throw("Error in Croissance::parcours_parcelle1B:\ncroi_stack.empty() = TRUE");
    }
    croi_stackitem csi;
    csi = croi_stack.back();
    croi_stack.pop_back();
    int i = csi.i;
    int j = csi.j;
    int seuil = csi.seuil;
    int numregion = csi.numregion;
    float somlum = csi.somlum;
    int nbpregion = csi.nbpregion;
    int lum = csi.lum;

    if(nbpregion == 0)
    {
        tablabel[i*nbc+j] = numregion;
        MoyCell[numregion] += tabin[i*nbc+j] ;
        NbPointCell ++ ;
        somlum += tabin[i*nbc+j];
        lum = tabin[i*nbc+j] ;
        nbpregion++;
        if(j<nbc-1) pushItem(i,j+1,seuil,numregion,somlum,nbpregion,lum);
        if(i<nbl-1) pushItem(i+1,j,seuil,numregion,somlum,nbpregion,lum);
        if(j>0) pushItem(i,j-1,seuil,numregion,somlum,nbpregion,lum);
        if(i>0) pushItem(i-1,j,seuil,numregion,somlum,nbpregion,lum);
    }
    else if(fabs((double)(tabin[i*nbc+j]-lum)) < (double)(seuil) && tablabel[i*nbc+j]==0)
    {
        tablabel[i*nbc+j] = numregion;
        MoyCell[numregion] += tabin[i*nbc+j] ;
        NbPointCell ++ ;
        somlum += tabin[i*nbc+j];
        nbpregion++;
        if(j<nbc-1) pushItem(i,j+1,seuil,numregion,somlum,nbpregion,lum);
        if(i<nbl-1) pushItem(i+1,j,seuil,numregion,somlum,nbpregion,lum);
        if(j>0) pushItem(i,j-1,seuil,numregion,somlum,nbpregion,lum);
        if(i>0) pushItem(i-1,j,seuil,numregion,somlum,nbpregion,lum);
  }
}

int Croissance::croissance2a( const Image *im, int threshhold, Image **luminance, Image **colorRgn  ) {
    Random random;
    NbPointCell=0;

    int seuil;
    int numregion;
    long nbpregion;
    int lum = 0;
    float somlum;

    seuil = threshhold;
    nbc = im->getWidth();
    nbl = im->getHeight();
    size = nbc * nbl;

//    BasicImageFun bif;
    tabin = im->begin();

    tabout = new Image::depth_t[size];
    tablabel = new int[size];
    MoyCell = new int[size];

    numregion=1 ;
    nbpregion=0 ;
    somlum=0;
    int min_i, min_j ,*tab_min_ij;
    tab_min_ij = new int[2*size];
    find_min(tab_min_ij) ;


    int i = 0 ;
    do
    {
        min_i = tab_min_ij[i];
        min_j = tab_min_ij[i+1];

        if(tablabel[min_i*nbc+min_j] == 0)
        {
            numregion++;
            pushItem(min_i,min_j,seuil,numregion,somlum,nbpregion,lum);
            while( !croi_stack.empty()) {
                parcours_parcelle1A();
            }
            MoyCell[numregion] = MoyCell[numregion]/NbPointCell;
        }

        NbPointCell = 0;
        somlum=0;
        nbpregion=0;
        i +=2 ;
    }while(i < 2*size);

    for(int i=0 ; i<size ; i++)
        tabout[i] = MoyCell[tablabel[i]];

    *luminance = new GrayscaleImage(nbc, nbl, tabout);

    RgbImage* ic = new RgbImage(nbc, nbl);
    for(int j=0 ; j<nbl ; j++) {
        for(int i=0 ; i<nbc ; i++) {
            int n = tablabel[j*nbc + i] - 1; /* € [0, numregion[ */
            const int nhue = 360;
            const int ngrad = ceil((double)numregion / (double)nhue);
            const int hue = floor(n * nhue / numregion); /* € [0, nhue[ */
            const int grad = n - ceil((double)hue * (double)numregion / (double)nhue); /* € [0, ngrad[ */
            if(grad < 0 || grad >= ngrad) {
                cout << "grad = " << grad << endl;
            }
            QColor color = QColor::fromHsl(hue, 255, (grad + 1) * 255 / (ngrad + 1));
            ic->setPixel(i, j, 0, color.red());
            ic->setPixel(i, j, 1, color.green());
            ic->setPixel(i, j, 2, color.blue());
        }
    }

    *colorRgn = ic;


    tabin = NULL;
    delete[] tabout;
    delete[] tablabel;
    delete[] MoyCell;
    tabout = NULL;
    tablabel = NULL;
    MoyCell = NULL;

    delete[] tab_min_ij;
    tab_min_ij = NULL;

    return numregion;

}

int Croissance::croissance2b( const Image *im, int threshhold, Image **luminance, Image **colorRgn  ) {
    Random random;
    NbPointCell=0;

    int seuil;
    int numregion;
    long nbpregion;
    int lum = 0;
    float somlum;

    seuil = threshhold;
    nbc = im->getWidth();
    nbl = im->getHeight();
    size = nbc * nbl;

//    BasicImageFun bif;
    tabin = im->begin();

    tabout = new Image::depth_t[size];
    tablabel = new int[size];
    MoyCell = new int[size];

    numregion=1 ;
    nbpregion=0 ;
    somlum=0;
    int min_i, min_j ,*tab_min_ij;
    tab_min_ij = new int[2*size];
    find_min(tab_min_ij) ;


    int i = 0 ;
    do
    {
        min_i = tab_min_ij[i];
        min_j = tab_min_ij[i+1];

        if(tablabel[min_i*nbc+min_j] == 0)
        {
            numregion++;
            pushItem(min_i,min_j,seuil,numregion,somlum,nbpregion,lum);
            while( !croi_stack.empty()) {
                parcours_parcelle1B();
            }
            MoyCell[numregion] = MoyCell[numregion]/NbPointCell;
        }

        NbPointCell = 0;
        somlum=0;
        nbpregion=0;
        i +=2 ;
    }while(i < 2*size);

    for(int i=0 ; i<size ; i++)
        tabout[i] = MoyCell[tablabel[i]];

    *luminance = new GrayscaleImage(nbc, nbl, tabout);

    RgbImage* ic = new RgbImage(nbc, nbl);
    for(int j=0 ; j<nbl ; j++) {
        for(int i=0 ; i<nbc ; i++) {
            int n = tablabel[j*nbc + i] - 1; /* € [0, numregion[ */
            const int nhue = 360;
            const int ngrad = ceil((double)numregion / (double)nhue);
            const int hue = floor(n * nhue / numregion); /* € [0, nhue[ */
            const int grad = n - ceil((double)hue * (double)numregion / (double)nhue); /* € [0, ngrad[ */
            if(grad < 0 || grad >= ngrad) {
                cout << "grad = " << grad << endl;
            }
            QColor color = QColor::fromHsl(hue, 255, (grad + 1) * 255 / (ngrad + 1));
            ic->setPixel(i, j, 0, color.red());
            ic->setPixel(i, j, 1, color.green());
            ic->setPixel(i, j, 2, color.blue());
        }
    }

    *colorRgn = ic;


    tabin = NULL;
    delete[] tabout;
    delete[] tablabel;
    delete[] MoyCell;
    tabout = NULL;
    tablabel = NULL;
    MoyCell = NULL;

    delete[] tab_min_ij;
    tab_min_ij = NULL;

    return numregion;
}

void Croissance::pushItem(int i, int j, int seuil, int numregion, float somlum, int nbpregion, int lum) {
	croi_stackitem csi;
	csi.i = i;
	csi.j = j;
	csi.seuil = seuil;
	csi.numregion = numregion;
	csi.somlum = somlum;
	csi.nbpregion = nbpregion;
	csi.lum = lum;
    croi_stack.push_back( csi );
}

void Croissance::find_min(int *tabmin) {
	int i, j, z, cmpt ;
    const Image::depth_t *val ;

	z = 0 ;

	for(cmpt=0 ; cmpt<256 ; cmpt++)
	{
		val = tabin ;
		for(i=0 ; i<nbl ; i++)
		{
			for(j=0 ; j<nbc ; j++)
			{
				if( *val == cmpt)
				{
					*(tabmin+z)   = i ;
					*(tabmin+z+1) = j ;
					z += 2 ;
				}
				val++ ;
			}
		}
	}
}
