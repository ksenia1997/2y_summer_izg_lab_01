/******************************************************************************
 * Laborator 01 - Zaklady pocitacove grafiky - IZG
 * ihulik@fit.vutbr.cz
 *
 * $Id: xbolsh00 $
 * 
 * Popis: Hlavicky funkci pro funkce studentu
 *
 * Opravy a modifikace:
 * - ibobak@fit.vutbr.cz, orderedDithering
 */

#include "student.h"
#include "globals.h"

#include <time.h>
#include <assert.h>

const int M[] = {
    0, 204, 51, 255,
    68, 136, 187, 119,
    34, 238, 17, 221,
    170, 102, 153, 85
};

const int M_SIDE = 4;

/******************************************************************************
 ******************************************************************************
 Funkce vraci pixel z pozice x, y. Je nutne hlidat frame_bufferu, pokud 
 je dana souradnice mimo hranice, funkce vraci barvu (0, 0, 0).
 Ukol za 0.25 bodu */
S_RGBA getPixel(int x, int y)
{
    // todo
    assert(frame_buffer != NULL);
    if ((x >= 0)&&(x < width)&&(y >= 0)&&(y < height))
		return frame_buffer[width*y+x];  // 1D pole
	else     
    	return COLOR_BLACK; //vraci barvu (0, 0, 0)
}
/******************************************************************************
 ******************************************************************************
 Funkce vlozi pixel na pozici x, y. Je nutne hlidat frame_bufferu, pokud 
 je dana souradnice mimo hranice, funkce neprovadi zadnou zmenu.
 Ukol za 0.25 bodu */
void putPixel(int x, int y, S_RGBA color)
{
    // todo
    if ((x >= 0)&&(x < width)&&(y >= 0)&&(y < height))
  		*(frame_buffer+width*y + x) = color;
}
/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na odstiny sedi. Vyuziva funkce GetPixel a PutPixel.
 Ukol za 0.5 bodu */
void grayScale()
{
    // todo
    int x, y;
    S_RGBA colour;
    unsigned char intensity;
    for (y = 0; y < height; y++) {
    	for (x = 0; x < width; x++) {
    		colour = getPixel(x, y);
    		intensity = ROUND(colour.red*0.299+colour.green*0.587+colour.blue*0.114);
    		colour.red = intensity;
    		colour.green = intensity;
    		colour.blue = intensity;
    		putPixel(x, y, colour);
    	}
    }
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci algoritmu maticoveho rozptyleni.
 Ukol za 1 bod */

void orderedDithering()
{
    // todo
    grayScale();
    int xm, ym;
    for(int y = 0; y<height; ++y) {
    	for(int x = 0; x < width; ++x) {
    		xm = x%M_SIDE;
    		ym = y%M_SIDE;
    		S_RGBA pix = getPixel(x, y);
    		if(pix.red > M[ym*M_SIDE+xm]) putPixel(x, y, COLOR_WHITE);
    		else putPixel(x, y, COLOR_BLACK);
    	}
    }

}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci algoritmu distribuce chyby.
 Ukol za 1 bod */

void disttribute(int x, int y, int err) {
	S_RGBA pix = getPixel(x, y);
	int intensity = pix.green + err;
	if (intensity > 255)
		intensity = 255;
	else if (intensity < 0) 
		intensity = 0;
	pix.red = pix.green = pix.blue = intensity;
	putPixel(x, y, pix);
}

void errorDistribution()
{   
    // todo
    // prevedeme obrazek na sedotonovy
    grayScale();
    for(int x = 0; x < width; x++) {
    	for(int y = 0; y < height; y++) {
    		S_RGBA pix = getPixel(x, y);
    		int intensity = pix.green; //vezmeme intenzitu jakekoliv barvy (vsude je stejna)
    		pix = (intensity > 127) ? COLOR_WHITE : COLOR_BLACK;
    		putPixel(x, y, pix);
    		int err = intensity - pix.green;
    		disttribute(x+1, y, 3.0/8.0*err);
    		disttribute(x, y+1, 3.0/8.0*err);
    		disttribute(x+1, y+1, 2.0/8.0*err);
    	} 
    }
    
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci metody prahovani.
 Demonstracni funkce */
void thresholding(int Threshold)
{
	/* Prevedeme obrazek na grayscale */
	grayScale();

	/* Projdeme vsechny pixely obrazku */
	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
		{
			/* Nacteme soucasnou barvu */
			S_RGBA color = getPixel(x, y);

			/* Porovname hodnotu cervene barevne slozky s prahem.
			   Muzeme vyuzit jakoukoli slozku (R, G, B), protoze
			   obrazek je sedotonovy, takze R=G=B */
			if (color.red > Threshold)
				putPixel(x, y, COLOR_WHITE);
			else
				putPixel(x, y, COLOR_BLACK);
		}
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci nahodneho rozptyleni. 
 Vyuziva funkce GetPixel, PutPixel a GrayScale.
 Demonstracni funkce. */
void randomDithering()
{
	/* Prevedeme obrazek na grayscale */
	grayScale();

	/* Inicializace generatoru pseudonahodnych cisel */
	srand((unsigned int)time(NULL));

	/* Projdeme vsechny pixely obrazku */
	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
		{
			/* Nacteme soucasnou barvu */
			S_RGBA color = getPixel(x, y);
			
			/* Porovname hodnotu cervene barevne slozky s nahodnym prahem.
			   Muzeme vyuzit jakoukoli slozku (R, G, B), protoze
			   obrazek je sedotonovy, takze R=G=B */
			if (color.red > rand()%255)
			{
				putPixel(x, y, COLOR_WHITE);
			}
			else
				putPixel(x, y, COLOR_BLACK);
		}
}
/*****************************************************************************/
/*****************************************************************************/