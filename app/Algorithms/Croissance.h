#ifndef IMAGEINSA_CROISSANCE_H
#define IMAGEINSA_CROISSANCE_H

#include <Image.h>
#include <vector>

class Croissance  
{
  public:

	// element de la pile de candidats
	struct croi_stackitem {
        int i, j; // coordonnees du pixel candidat
		int seuil; // seuil (donne par l'utilisateur au debut de l'operation) -> pourquoi est il dans la structure et pas comme attribut de la classe croissance ?
		int numregion; // numero de la region
		float somlum; // luminance cumulee des pixels contenus dans la region pour laquelle le pixel est candidat
        int nbpregion; // nombre de pixels dans la region pour laquelle le pixel est candidat
		int lum; // luminance du pixel candidat
	};

	Croissance();
	virtual ~Croissance();
    int croissance1a( const imagein::Image *im, int threshhold, imagein::Image **luminance, imagein::Image **colorRgn ); // At origin, | current - mean | < threshold
    int croissance1b( const imagein::Image *im, int threshhold, imagein::Image **luminance, imagein::Image **colorRgn ); // At origin | current - initial | < threshold
    int croissance2a( const imagein::Image *im, int threshhold, imagein::Image **luminance, imagein::Image **colorRgn ); // Point of lowest luminance | current - mean | < threshold
    int croissance2b( const imagein::Image *im, int threshhold, imagein::Image **luminance, imagein::Image **colorRgn ); // Point of lowest luminance | current - initial | < threshold

protected:
	void parcours_parcelle1A();
	void parcours_parcelle1B();
	void parcours_parcelle2A();
	void parcours_parcelle2B();
    void pushItem(int i, int j, int seuil, int numregion, float somlum, int nbpregion, int lum);
    void find_min(int *tabmin);

    const imagein::Image::depth_t *tabin; // tableau de valeurs de l'image d'entree (luminance) [0 ; size[
    imagein::Image::depth_t *tabout; // tableau de valeurs pour l'image de sortie (luminance) [0 ; size[
    int *tablabel ; // tableau des numeros de regions pour chaque pixel de l'image [0 ; size[
    long nbc,nbl,size ; // nombre de lignes, colonnes, et taille de l'image d'entree (et donc de sortie)
    int *tab_min_ij;
    float coefr,coefv,coefb;
    std::vector<croi_stackitem> croi_stack; // pile contenant les pixels candidats a une region
    int *MoyCell, NbPointCell; // tableau des valeurs moyenne des pixels par regions [0 ; numregion[, nombre de pixels dans une region (?)
public:
};

#endif // !CROISSANCE_H
