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
void *sort_1(void *param);
void *sort_2(void *param);
void *merge(void *param);
//-------------------------------------------------------------------------
void main(int argc, char* argv[])
{
	N = argc;
	argi = (int*)argv;
	argumrnts_to_int(argv);
	argumants_info(N,argi);

	/* the thread identifier */
	pthread_t tid_sort_1;
	pthread_t tid_sort_2;
	pthread_t tid_merge;				
	/* set of thread attributes */
	pthread_attr_t attr_sort_1;
	pthread_attr_t attr_sort_2;
	pthread_attr_t attr_merge;		
	// 	/* set the default attributes of the thread */
	pthread_attr_init(&attr_sort_1);
	pthread_attr_init(&attr_sort_2);
	// 	/* create the thread */
	pthread_create(&tid_sort_1,&attr_sort_1,sort_1,argi);
	pthread_create(&tid_sort_2,&attr_sort_2,sort_2,argi);
	// 	/* wait for the thread to exit */
	pthread_join(tid_sort_1, NULL);
	pthread_join(tid_sort_2, NULL);

    // merge thread waits sort_1 & sort_2 finishes
	pthread_attr_init(&attr_merge);
	pthread_create(&tid_merge,&attr_merge,merge,argi);
	pthread_join(tid_merge, NULL);
	
    printf("sorted:{ ");
    for(int i=0;i<(N-1);i++)
    {
        printf("%d ",argi[i]);
    }
    printf("}\n");
    
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

void *sort_1(void *param) // O(N^2)
{
	int* arr= (int*)param;
    int temp;
	for(int i =0; i<(N-1)/2; i++)
	{
        for(int j=i+1; j<(N-1)/2; j++)
            {
                if(arr[j]<arr[i])
                    {
                        temp = arr[i];
                        arr[i]=arr[j];
                        arr[j]=temp;
                    }
            }
	}
}
 
void *sort_2(void *param)
{
	int* arr = (int*)param;
	int temp;
	for(int i =(N-1)/2; i<(N-1); i++)
	{
        for(int j=i+1; j<(N-1); j++)
            {
                if(arr[j]<arr[i])
                    {
                        temp = arr[i];
                        arr[i]=arr[j];
                        arr[j]=temp;
                    }
            }
	}
}

void *merge(void *param)
{
	int* arr = (int*)param;
	int temp[N-1];
    int s1=0;
    int s2=(N-1)/2;
	for(int i =0; i<(N-1); i++)
	{
        if(s1==(N-1)/2)
        {
            while (i<(N-1))
            {
                temp[i] = arr[s2];
                s2++;
                i++;    
            }
        }
        else if(s2==(N-1))
        {
            while (i<(N-1))
            {
                temp[i] = arr[s1];
                s1++;
                i++;    
            }
        }
        else if(arr[s1]<arr[s2])
        {
            temp[i] = arr[s1];
            s1++;
        }
        else if(arr[s1]>arr[s2])
        {
            temp[i] = arr[s2];
            s2++;
        }
        else if(arr[s1]==arr[s2])
        {
            temp[i] = arr[s1];
            temp[i+1] = arr[s2];
            s1++;
            s2++;
            i++;
        }

	}
    argi = temp;
}
