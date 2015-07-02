#include<string.h>
#include<stdlib.h>
#include "AxesTranspose.h"

// coordinates(X) ---> HilbertLibPosition(X) (changes X from coordinates in n dimensions to HilbertLibPosition)
hilpos_t GetHCoordinate( coord_t* Z, coord_t *X, coord_t *Y, int b, int n) { // position,bits,dimensions, Z,Y is used
	memcpy(X,Z,sizeof(coord_t)*n);
	coord_t M = 1 << (b-1), P, Q, t;
	int i,j;
	for(Q=M;Q>1;Q>>=1) {
		P = Q -1;
		for(i=0;i<n;i++)
			if(X[i]&Q) 
				X[0]^=P;
			else 
				{t = (X[0]^X[i]) & P;X[0] ^= t; X[i] ^= t; }
	}
	for( i=1;i<n;i++) 
		X[i] ^= X[i-1];
	t = 0;
	for(Q=M;Q>1;Q>>=1)
		if(X[n-1] & Q)
			t ^= Q-1;
	for(i=0;i<n;i++) X[i]^=t;
	int nob = sizeof(coord_t)*8;
	int wsk = 0;
	int wskbits =0;
	Y[0] = 0;
	for(i=b-1;i>=0;i--) {
		for(j=0;j<n;j++) {
			Y[wsk] *= 2;
			Y[wsk] += ((X[j]>>i)&1);
			wskbits++;
			if(wskbits == nob) {
				wskbits = 0;
				wsk++;
				Y[wsk] = 0;
			}
		}
	}
	
	hilpos_t res = 0;
	hilpos_t akt_val = 1;
	for(i=n-1;i>=0;i--) {
		res += akt_val * Y[i];
		akt_val *= (hilpos_t)(1<<b);
	}

	return res;
}
