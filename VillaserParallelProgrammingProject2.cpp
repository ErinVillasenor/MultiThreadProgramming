//Report can be found at https://docs.google.com/document/d/1sTJ5LmEHCYR-0aFnTE5WBG-1mdTPPoaS5SAK8-avxLY/edit?usp=sharing
#include <iostream>
#include <omp.h>
#include <stdio.h>
#include <math.h>
#include <fstream>
using namespace std;

#define XMIN     -1.
#define XMAX      1.
#define YMIN     -1.
#define YMAX      1.

//alters shape
#define N		 4

// setting the number of threads:
#ifndef NUMT
#define NUMT		1
#endif

//Number of runs  to pull MaxMegaNodes from
#ifndef NUMTRIES
#define NUMTRIES	10
#endif

//Number of nodes where grid is N*N big
#ifndef NUMNODES
#define NUMNODES	1000
#endif

double Height(int, int);

int main(int argc, char* argv[])
{
	//double volume;
//	. . .
		// the area of a single quarter-sized tile:
		//double quarterTileArea = 

		// the area of a single half-sized tile:
		//double halfTileArea = 

		// the area of a single full-sized tile:



	// sum up the weighted heights into the variable "volume"
	// using an OpenMP for loop and a reduction:
		omp_set_num_threads(NUMT);
		double maxMegaNodes = 0;
		for (int t = 0; t < NUMTRIES; t++)
		{
			double volume = 0;

			double time0 = omp_get_wtime();

#pragma omp parallel for default(none),reduction(+:volume) //. . .
			for (int i = 0; i < NUMNODES * NUMNODES; i++)
			{
				int iu = i % NUMNODES;
				int iv = i / NUMNODES;
				double z = Height(iu, iv);
				if ((iu == 0 && iv == 0) || (iu == 0 && iv == NUMNODES - 1) || (iu == NUMNODES - 1 && iv == 0) || (iu == NUMNODES - 1 && iv == NUMNODES - 1))
				{//corner
					double quarterTileArea = 1 / 4 * (((XMAX - XMIN) / (double)(NUMNODES - 1)) *
						((YMAX - YMIN) / (double)(NUMNODES - 1)));
					volume += (z * quarterTileArea);

				}

				else if ((iu == 0) || (iv == 0) || (iu == NUMNODES - 1) || (iv == NUMNODES - 1))
				{//edge
					double halfTileArea = 1 / 2 * (((XMAX - XMIN) / (double)(NUMNODES - 1)) *
						((YMAX - YMIN) / (double)(NUMNODES - 1)));
					volume += (z * halfTileArea);

				}
				else
				{//full
					double fullTileArea = (((XMAX - XMIN) / (double)(NUMNODES - 1)) *
						((YMAX - YMIN) / (double)(NUMNODES - 1)));
					volume += (z * fullTileArea);
				}

			}
			double time1 = omp_get_wtime();
			double megaNodes = ((double)NUMNODES * NUMNODES) / (time1 - time0) / 1000000.;
			if (megaNodes > maxMegaNodes)
				maxMegaNodes = megaNodes;
			cout << volume << endl; /// NUMNODES;

		}
		cout << maxMegaNodes << endl;


		return 0;
}

double
Height(int iu, int iv)	// iu,iv = 0 .. NUMNODES-1
{
	double x = -1. + 2. * (double)iu / (double)(NUMNODES - 1);	// -1. to +1.
	double y = -1. + 2. * (double)iv / (double)(NUMNODES - 1);	// -1. to +1.

	double xn = pow(fabs(x), (double)N);
	double yn = pow(fabs(y), (double)N);
	double r = 1. - xn - yn;
	if (r < 0.)
		return 0.;
	double height = pow(1. - xn - yn, 1. / (double)N);
	return height *2; //*2; //To account for height being only half.
}
//Multiply height by 2
//Corner = 1/4 tile
//Edge = 1/2 tile
//Numnodes^2 over time.
