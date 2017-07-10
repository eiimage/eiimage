#ifndef IMAGEINSA_CROISSANCE_H
#define IMAGEINSA_CROISSANCE_H

#include <Image.h>
#include <vector>

class Croissance  
{
  public:

	  // �l�ment de la pile de candidats
	struct croi_stackitem {
        int i, j; // coordonn�es du pixel candidat
		int seuil; // seuil (donn� par l'utilisateur au d�but de l'op�ration) -> pourquoi est il dans la structure et pas comme attribut de la classe croissance ?
		int numregion; // num�ro de la region
		float somlum; // luminance cumul�e des pixels contenus dans la r�gion pour laquelle le pixel est candidat
        int nbpregion; // nombre de pixels dans la r�gion pour laquelle le pixel est candidat
		int lum; // luminance du pixel candidat
	};

	Croissance();
	virtual ~Croissance();
    int croissance1a( const imagein::Image *im, int threshhold, imagein::Image **luminance, imagein::Image **colorRgn ); // At origin, | current - mean | < threshold
    int croissance1b( const imagein::Image *im, int threshhold, imagein::Image **luminance, imagein::Image **colorRgn ); //
    int croissance2a( const imagein::Image *im, int threshhold, imagein::Image **luminance, imagein::Image **colorRgn );
    int croissance2b( const imagein::Image *im, int threshhold, imagein::Image **luminance, imagein::Image **colorRgn );

protected:
	void parcours_parcelle1A();
	void parcours_parcelle1B();
	void parcours_parcelle2A();
	void parcours_parcelle2B();
    void pushItem(int i, int j, int seuil, int numregion, float somlum, int nbpregion, int lum);
    void find_min(int *tabmin);

    const imagein::Image::depth_t *tabin; // tableau de valeurs de l'image d'entr�e (luminance) [0 ; size[
    imagein::Image::depth_t *tabout; // tableau de valeurs pour l'image de sortie (luminance) [0 ; size[
    int *tablabel ; // tableau des num�ros de r�gions pour chaque pixel de l'image [0 ; size[
    long nbc,nbl,size ; // nombre de lignes, colonnes, et taille de l'image d'entr�e (et donc de sortie)
    int *tab_min_ij;
    float coefr,coefv,coefb;
    std::vector<croi_stackitem> croi_stack; // pile contenant les pixels candidats a une r�gion
    int *MoyCell, NbPointCell; // tableau des valeurs moyenne des pixels par r�gions [0 ; numregion[, nombre de pixels dans une r�gion (?)
public:
};

#endif // !CROISSANCE_H
