#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
//-------------------------------------------------------------------------
int N;		// N = argc
int* argi;  // argi[i] = *(int)argv[i]
int Nmax=0,Nmin=0,Navg=0;
//-------------------------------------------------------------------------
void argumrnts_to_int(char* argv[]);
void argumants_info();
void *max(void *param);
void *min(void *param);
void *avg(void *param);
//-------------------------------------------------------------------------
void main(int argc, char* argv[])
{
	N = argc;
	argi = (int*)argv;
	argumrnts_to_int(argv);
	argumants_info(N,argi);	
	/* the thread identifier */
	pthread_t tid_max;
	pthread_t tid_min;
	pthread_t tid_avg;				
	/* set of thread attributes */
	pthread_attr_t attr_max;
	pthread_attr_t attr_min;
	pthread_attr_t attr_avg;		
	// 	/* set the default attributes of the thread */
	pthread_attr_init(&attr_max);
	pthread_attr_init(&attr_min);
	pthread_attr_init(&attr_avg);
	
	// 	/* create the thread */
	pthread_create(&tid_max,&attr_max,max,argi);
	pthread_create(&tid_min,&attr_min,min,argi);
	pthread_create(&tid_avg,&attr_avg,avg,argi);
	// 	/* wait for the thread to exit */
	pthread_join(tid_max, NULL);
	pthread_join(tid_min, NULL);
	pthread_join(tid_avg, NULL);

	printf("max: %d \n",Nmax);
	printf("min: %d \n",Nmin);
	printf("avg: %d \n",Navg);
	return;
}
//-------------------------------------------------------------------------
void argumrnts_to_int(char* argv[])
{
	for(int i =0; i<(N-1); i++)
	{
		argi[i] = atoi(argv[i+1]);
	}
}

void argumants_info()
{
printf("No.argumants: %d\n",N-1);
	printf("Argumants:{ ");
	for(int i =0; i<(N-1);i++)
	{
		printf("%d ",argi[i]);
	}
	printf("}\n");
}

void *max(void *param)
{
	int* arr= (int*)param;
	Nmax = arr[1];  // arr[0] = programe name
	for(int i =0; i<(N-1); i++)
	{
		if(arr[i]>=Nmax)
			Nmax = arr[i];
	}
}
 
void *min(void *param)
{
	int* arr = (int*)param;
	Nmin = arr[1];  // arr[0] = programe name
	for(int i =0; i<(N-1); i++)
	{
		if(arr[i]<=Nmin)
			Nmin = arr[i];
	}
}

void *avg(void *param)
{
	int* arr = (int*)param;
	Navg = 0;
	for(int i =0; i<(N-1); i++)
	{
		Navg += (arr[i]); // parsing char to int 
	}
	
	Navg /= (N-1);  // N-1 due to arr[0] = programe name ,  
}
