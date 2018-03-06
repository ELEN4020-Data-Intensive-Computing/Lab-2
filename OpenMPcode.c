#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef int bool;
#define true 1
#define false 0

void PrintTranspose(int **arraY, int SquareSize) {

int N = SquareSize;

	for(int j = 0; j < N; j++) {
  		for(int k = 0; k < N; k++) {
			printf("%d ", arraY[j][k]);
		}
		printf("\n");
	}
}

void Transpose(int **arraY, int SquareSize) {
	int nthreads, tid, i, j, k, chunk=SquareSize, count, starterRow=0, starterCol=0;
	int N = SquareSize;

	double start = omp_get_wtime();
		#pragma omp parallel shared(arraY,nthreads,chunk,starterRow,starterCol,i,j,k) private(tid) 
		  {
		 	 tid = omp_get_thread_num();
		 	 if (tid == 0) //thread 0 belongs to master thread
		    	{
		    		nthreads = omp_get_num_threads();
		   		//printf("Number of threads = %d\n", nthreads);
		   	 }
		 // printf("Thread %d starting...\n",tid);
	
	#pragma omp parallel for schedule(dynamic,chunk)
	for(int i = starterRow; i < N+1; i++){
	 // #pragma omp parallel for
		if(i > 0) {
			starterCol++;		
		}
	  for (int j=starterCol; j<N; j++)
	    {
		
		 int temp = arraY[i][j];
	 	arraY[i][j] = arraY[j][i];
		 arraY[j][i] = temp;
	    }
	 }
		  
	}

	double time  = omp_get_wtime() - start;
	printf("Time in seconds = %f",time );	
	printf("\n");
	//PrintTranspose(arraY,SquareSize);
}

int main(){

	int rows = 128;
	int cols = 128;
	for(int i = 0; i < 3; i++) {
	printf("Matrix size = %d\n", rows);
	int **a;
	int threads = 4;
	int i;
	int temp = i + 1; 
	a = malloc(rows * sizeof (int *));

	 // for each row allocate cols int
  	for (int i = 0; i < rows; i++) {
        	a[i] = malloc(cols * sizeof (int));
    	}
	
     //populating the elements of the array
     for ( i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
           int temp = i*rows + j; 
           a[i][j] = temp;
        }
    }

	for(int j = 0; j < 5; j++) {
		omp_set_num_threads(threads);
		Transpose(a,rows);
		threads *=2;
		if(threads/32 == 1)
			threads *=2;
	}
	rows *=8;
	cols *=8;
}

}

