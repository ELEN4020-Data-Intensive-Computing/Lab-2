#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define s_size 300000000000

struct parameter{
int* array;
int dimension;
int original_dim;
int thread_number;
int max_threads;
};


void printArray(int *_array, int * dim)  
{
	/* output each array element's value */
	for ( int i = 0; i < *dim; i++ ) {

		for ( int j = 0; j < *dim; j++ ) {
         		printf("a[%d][%d] = %d\t", i,j, *_array );
			_array++;
		}
		
		printf("\n");
	}
}


void *thread(struct parameter *param)
{
	int* _array = (*param).array;
	int dimension = (*param).dimension;
	int original_dim = (*param).original_dim;
	int n = 0;	
	int * temp = &n;
	int* row_ptr = _array;
	int* column_ptr = _array;
	int thread_number =(*param).thread_number;
	int max_threads = (*param).max_threads;
	
	row_ptr = row_ptr+(thread_number);
	column_ptr = column_ptr+((original_dim)*(thread_number));
	if (thread_number<dimension)
	{
		*temp = *row_ptr; 
		*row_ptr = *column_ptr;
		*column_ptr = *temp;
	}
	
	for (int i = 2; i*(max_threads)<dimension; i++)
	{			
		row_ptr = (row_ptr)+(max_threads);
		column_ptr = column_ptr+((max_threads)*(original_dim));
		*temp = *row_ptr; 
		*row_ptr = *column_ptr;
		*column_ptr = *temp;
	}
	_array++;
	_array = _array+original_dim;
	dimension = dimension-1;
	struct parameter new_param;
	new_param.array = _array;
	new_param.dimension = dimension;
	new_param.original_dim = original_dim;
	new_param.thread_number = thread_number;
	new_param.max_threads = max_threads;

	if (dimension!=0)
		thread(&new_param);
	//pthread_exit(NULL);
}


int main(int argc, char *argv[])
{
	
	void * stack;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setstacksize(&attr, s_size);
	
        int max_threads[] = {4,8,16,64,128};
	int num_t = 1;

	
	
	int fail;

	double result[10];
	int ori_dim0 = 1024;
	int ori_dim1 = 128;
	int array[ori_dim0][ori_dim0]; 
	for(int i = 0; i < ori_dim0; i++){
		for(int j = 0; j < ori_dim0;j++) {
			array[i][j] = i*ori_dim0 + j;
		}
	}

	for (int i = 0; i<5; i++)
	{
		struct parameter param[max_threads[i]];
		pthread_t threads[max_threads[i]];
		clock_t begin_parallel = clock();	
		for (int y = 1; y<=max_threads[i]; y++)
		{
			param[y-1].array = array;
			param[y-1].dimension = ori_dim0;
			param[y-1].original_dim =ori_dim0; 
			param[y-1].thread_number = y;
			param[y-1].max_threads =max_threads[i];
			if(pthread_create(&threads[y-1],NULL, thread, &param[y-1])){
				fprintf(stderr, "Error creating thread %d\n", y);
				return 1;	
			}

		}
		clock_t end_parallel = clock();
		result[i] = (double)(end_parallel - begin_parallel)/CLOCKS_PER_SEC;
	}

	int array2[ori_dim1][ori_dim1]; 
	for(int i = 0; i < ori_dim1; i++){
		for(int j = 0; j < ori_dim1;j++) {
			array2[i][j] = i*ori_dim1 + j;
		}
	}

	//for(int v = 0; v<100000000; v++){int f;}

	for (int o = 0; o<5; o++)
	{
		struct parameter param2[max_threads[o]];
		pthread_t threads2[max_threads[o]];
		clock_t begin_parallel = clock();	
		for (int x = 1; x<=max_threads[o]; x++)
		{
			param2[x-1].array = array2;
			param2[x-1].dimension = ori_dim1;
			param2[x-1].original_dim =ori_dim1; 
			param2[x-1].thread_number = x;
			param2[x-1].max_threads =max_threads[o];
			if(pthread_create(&threads2[x-1],NULL, thread, &param2[x-1])){
				fprintf(stderr, "Error creating thread %d\n", x);
				return 1;	
			}

		}
		clock_t end_parallel = clock();
		result[o+5] = (double)(end_parallel - begin_parallel)/CLOCKS_PER_SEC;
		
	}
	
	printf("thread Number:\t\t4 \t\t 8 \t\t 16 \t\t 64 \t\t 128\n");
	printf("Pthread 1024x1024: \t");
	for (int i = 0; i<5; i++) printf("%f\t",result[i]);
	printf("\n");
	printf("Pthread 128x128: \t");
	for (int i = 0; i<5; i++) printf("%f\t",result[i+5]);
	//printArray(array,&ori_dim);
	printf("\n");
	pthread_exit(NULL);
	
	
}
