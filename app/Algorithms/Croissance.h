#ifndef EIIMAGE_CROISSANCE_H
#define EIIMAGE_CROISSANCE_H

#include <Image.h>
#include <vector>

class Croissance  
{
  public:
	struct croi_stackitem {
        int i, j;
		int seuil;
		int numregion;
		float somlum;
        int nbpregion;
		int lum;
	};

	Croissance();
	virtual ~Croissance();
    int croissance1a( const imagein::Image *im, int threshhold, imagein::Image **luminance, imagein::Image **colorRgn );
    int croissance1b( const imagein::Image *im, int threshhold, imagein::Image **luminance, imagein::Image **colorRgn );
    int croissance2a( const imagein::Image *im, int threshhold, imagein::Image **luminance, imagein::Image **colorRgn );
    int croissance2b( const imagein::Image *im, int threshhold, imagein::Image **luminance, imagein::Image **colorRgn );

protected:
	void parcours_parcelle1A();
	void parcours_parcelle1B();
	void parcours_parcelle2A();
	void parcours_parcelle2B();
    void pushItem(int i, int j, int seuil, int numregion, float somlum, int nbpregion, int lum);
    void find_min(int *tabmin);

    const imagein::Image::depth_t *tabin;
    imagein::Image::depth_t *tabout;
    int *tablabel ;
    long nbc,nbl,size ;
    int *tab_min_ij;
    float coefr,coefv,coefb;
    std::vector<croi_stackitem> croi_stack;
    int *MoyCell, NbPointCell;
public:
};

#endif // !CROISSANCE_H
