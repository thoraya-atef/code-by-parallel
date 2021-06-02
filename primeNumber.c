#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

//id=20180078,,name=Thoraya Atef,,Group=S2
int main(int argc , char * argv[])
{
   int my_rank;		// rank of process
	int p;//number of process

	MPI_Status status;	/* return status for 	*/

	/* Start up MPI */
	MPI_Init( &argc , &argv );

	/* Find out process rank */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	/* Find out number of process */
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	if( my_rank == 0)
	{

        // master process

            //to lower bound and upper
            int x_lowerBound,y_upperBound;
            int range;

            //total count of primes
            int totalCount=0;

            printf("Enter lower bound: ");
            scanf("%d", &x_lowerBound);
            printf("Enter upper bound: ");
            scanf("%d", &y_upperBound);

            //to lower bound and upper
            int x=x_lowerBound;
            int y=y_upperBound;
            range=y-x;

            int* arr =  malloc ((range) * sizeof (int));

        int i = 0;
        int x1=x;
        for (; i<range;i++){
          arr[i] = x1++;
        }

        //Calculate the sub range size
        int sub_range=(y-x)/(p-1);
         for(i=1;i<p;i++)
            {
                MPI_Send(&sub_range,1, MPI_INT,i,0,MPI_COMM_WORLD);
                MPI_Send( arr+((i-1)*sub_range),sub_range, MPI_INT,i,0,MPI_COMM_WORLD);
            }
            for(i=1;i<p;i++)
            {
                //Receive sub count from each slave process
                int subCount;
                MPI_Recv( &subCount,1, MPI_INT,i,0,MPI_COMM_WORLD,&status);
                totalCount+=subCount;

            }
            printf("\n Total number of prime numbers is : %d \n ",totalCount);

        }
        else
	{
            int sub_range;
            MPI_Recv( &sub_range,1, MPI_INT,0,0,MPI_COMM_WORLD,&status);

            int* arr =  malloc (sub_range * sizeof (int));
            MPI_Recv( arr,sub_range, MPI_INT,0,0,MPI_COMM_WORLD,&status);

            int subCount=0;
            int i,z,check;

            //Calculate Count primes in its sub range
            for (i = 0;i<sub_range;i++)
              {
                  if(arr[i]==0||arr[i]==1)
                    continue;
                    check=1;
                for (z = 2; z <= arr[i]/2; z++)
                {
                  if (arr[i]%z == 0)
                  {
                      check=0;
                      break;
                  }
                  else{
                    check=1;
                  }
                }
                if(check==1)
                    subCount++;

              }

              //Send this partial count to the master process
              MPI_Send( &subCount,1, MPI_INT,0,0,MPI_COMM_WORLD);
               // Print the partial count
              printf("\n  Number of prime numbers in p%d is: %d \n ",my_rank,subCount);
	}

	/* shutdown MPI */
	MPI_Finalize();
    return 0;
}
