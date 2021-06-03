#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include <math.h>
#include "mpi.h"

int main(int argc , char * argv[])
{
    int my_rank;		/* rank of process	*/
	int p;			/* number of process	*/
    int n;
    int i;
    int* arr;
    float globalMean = 0;
    float globalSum;
    float localSum=0;
    float localSumOfSDM=0;
    float globalSumOfSDM=0;
    float meanOfSD=0;

	MPI_Status status;	/* return status for 	*/
				/* recieve		*/

	/* Start up MPI */
	MPI_Init( &argc , &argv );

	/* Find out process rank */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	/* Find out number of process */
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	if( my_rank == 0)
	{
	    printf("Enter number of elements per each process: ");
        scanf("%d", &n);

    }

    MPI_Bcast(&n , 1 , MPI_INT, 0,MPI_COMM_WORLD);

 if(my_rank!=0)
	{

	  i=0;
      arr =  malloc (n * sizeof (int));
       srand(time(0));
        for (; i<n;i++){
          arr[i] = (rand() % 60) +1;
        }
        i = 0;
        for(;i<n;i++){
          localSum+=(float)arr[i];
        }

	}
   MPI_Allreduce(&localSum,&globalSum,1,MPI_FLOAT,MPI_SUM,MPI_COMM_WORLD);


   if(my_rank!=0)
   {
        globalMean=globalSum/(n*(p-1));

        i=0;

        for(;i<n;i++){

          localSumOfSDM += pow((arr[i]-globalMean), 2);
        }


   }
   MPI_Reduce(&localSumOfSDM,&globalSumOfSDM,1,MPI_FLOAT,MPI_SUM,0,MPI_COMM_WORLD);


  if (my_rank == 0 ){

       meanOfSD=globalSumOfSDM/(n*(p-1));

       float Standard_deviation=pow(meanOfSD,.5);

    printf("Standard deviation is %f\n",Standard_deviation );
  }
	/* shutdown MPI */
	MPI_Finalize();
    return 0;
}
