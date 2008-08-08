/*
Empty headed
 */
#define DIMENSION 64
#define BASELINE 0

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/* make and image, and then add noise to it */
#include <math.h>
#include <limits.h>
#include "VLib.h"
#include "ImgLib.h"

// float noise(long *idum, short noiseLevel);
/* calls GASDEV.c from nr and creates gaussian noise of s.d. = noiseLevel */

int gMagnitude;

void AddSignal( IMAGE *im, float *data, float signal );
void AddSignal( IMAGE *im, float *data, float signal )
{
/* make activation regions of various size */
	short	c, r, i, xs, ys;

/* generate four rows of well-defined activation areas */
/* row 1&3................
          ..xx...x..xx..x.
          ..xz......xx....
*/
	xs = im->dim.x;
	ys = im->dim.y;
	
	for( r=xs*ys/8; r<xs*ys*3/16; r+=xs ) {
		for( c=xs/8; c<xs/4; c++ ) {
			data[r+c] = fabs( data[r+c] + signal );
		}
		for( c=7*xs/16; c<xs/2; c ++ ) {
			data[r+c] = fabs( data[r+c] + signal );
		}
		for( c=5*xs/8; c<3*xs/4; c++ ) {
			data[r+c] = fabs( data[r+c] +signal );
		}
		for( c=7*xs/8; c<xs*15/16; c++ ) {
			data[r+c] = fabs( data[r+c] + signal );
		}
	}
	for( r=xs*ys*3/16; r<xs*ys/4; r+=xs ) {
		for( c=xs/8; c<xs/4; c++ ) {
			data[r+c] = fabs( data[r+c] + signal );
		}
		for( c=xs*5/8; c<xs*3/4; c++ ) {
			data[r+c] = fabs( data[r+c] +signal );
		}
	}
	
	for( r=xs*ys*5/8; r<xs*ys*11/16; r+=xs ) {
		for( c=xs/8; c<xs/4; c++ ) {
			data[r+c] = fabs( data[r+c] + signal );
		}
		for( c=7*xs/16; c<xs/2; c ++ ) {
			data[r+c] = fabs( data[r+c] + signal );
		}
		for( c=5*xs/8; c<xs*3/4; c++ ) {
			data[r+c] = fabs( data[r+c] + signal );
		}
		for( c=7*xs/8; c<xs*15/16; c++ ) {
			data[r+c] = fabs( data[r+c] + signal );
		}
	}
	for( r=xs*ys*11/16; r<xs*ys*3/4; r+=xs ) {
		for( c=xs/8; c<xs/4; c++ ) {
			data[r+c] = fabs( data[r+c] + signal );
		}
		for( c=5*xs/8; c<xs*3/4; c++ ) {
			data[r+c] = fabs( data[r+c] +signal );
		}
	}
/* row 2 and 4 ................
               ...x...x...x...x
*/
	for( r=xs*ys*7/16; r<xs*ys/2; r+=xs ) {
		for( i=3; i<16; i+=4 ) {
			for( c=i*xs/16; c<(i+1)*xs/16; c++ ) {
				data[r+c] = fabs( data[r+c] + signal );
			}
		}
	}
	for( r=xs*ys*7/8; r<xs*ys*15/16; r+=xs ) {
		for( i=3; i<16; i+=4 ) {
			for( c=i*xs/16; c<(i+1)*xs/16; c++ ) {
				data[r+c] = fabs( data[r+c] + signal );
			}
		}
	}
}

int main(void)
{
	int		i;
	FILE 	*fp, *parafile;
	int		c, r, count=0;
	float	*data;
	long		myLong;
	float	NL=50;
	float	activationMagnitude = 10;
	float	signal;
	IMAGE	im;
	short	xs, ys;
	char		filename[256], string[256], paraFileName[256];
	unsigned short *imData;

	gMagnitude = 1000;

	srand(time(NULL));

	printf( "Simulation of a noisy time series\n" );
	printf( "Creates a series of images with mean signal of %d\n", gMagnitude );
	printf( " in a background of mean amplitude %d\n\n", BASELINE );
	printf( "Please enter a name for the noise series (no extension ): " );
	scanf( "%s", filename );
	printf( "Enter the activation magnitude: " );
	scanf( "%f", &activationMagnitude );
	printf( "Enter a noise intensity: " );
	scanf( "%f", &NL );
	sprintf( string, "%s.bshort", filename );
	printf( "\nYour data will appear in %s, with a noise level of %0.3f,\n", string, NL );
	printf( "and an activation intensity of %0.3f\n", activationMagnitude );
	
	printf( "Select a paradigm file (controls the activation time course): " );
	scanf( "%s", paraFileName );
	parafile = fopen( paraFileName, "r" );
	
	fp = fopen( string, "wb" );
	
	fscanf( parafile, "%f", &signal );
	 
	im.dim.x = DIMENSION;
	im.dim.y = DIMENSION;
	
	xs=im.dim.x;
	ys=im.dim.y;
	
	data = (float *)malloc( xs * ys * sizeof( float ));
	imData = (unsigned short *)malloc( xs * ys * sizeof( short ));
	
	myLong=(long)rand()*rand();

/* make the images (a square box in the center */
	while(!feof( parafile ) )
	{
		signal *= activationMagnitude;
		printf( "%f\n", signal );
		
		for( r=0; r<ys/4; r++ ) {
			for( c=0; c<xs; c++ ) {
				data[c + r*xs] = BASELINE;
			}
		}

		for( r=ys/4; r<3*ys/4; r++ ) {
			for( c=0; c<xs/4; c++ ) {
				data[c + r*xs] = BASELINE;
			}
			for( c=xs/4; c<3*xs/4; c++ ) {
				data[c + r*xs] = gMagnitude;
			}
			for( c=3*xs/4; c<xs; c++ ) {
				data[c + r*xs] = BASELINE;
			}
		}
		
		for( r=3*ys/4; r<ys; r++ ) {
			for( c=0; c<xs; c++ ) {
				data[c + r*xs] = BASELINE;
			}
		}
/* add signal to this image */
		AddSignal( &im, data, signal );
		fscanf( parafile, "%f", &signal );
		
/* add noise to this image */
		for( i=0; i<xs*ys; i++ ) {
			data[i] = fabs( data[i]+noise( &myLong, NL ) );
			imData[i] = (unsigned short)(data[i] + 0.5);
		}
		
		count++;
		
		fwrite( imData, sizeof( short ), xs*ys, fp );
	}
	fclose( fp );
	fclose( parafile );
	sprintf( string, "%s.hdr", filename );
	fp = fopen( string, "w" );
	fprintf( fp, "%d %d %d %d", ys, xs, count, macByteOrder() );
	fclose( fp );	
	return 0;
}
