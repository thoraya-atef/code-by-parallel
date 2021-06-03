#include <omp.h>
#include <stdio.h>
#include <stdlib.h>



int main(int argc, char *argv[])
{

    char** matrix;
    int i,k,N,M, chunk,x,y;
    printf("Enter Size of Matrix: ");
    scanf("%d %d", &N,&M);
     //for Dynamic allocation of Matrix
     matrix =(char**) malloc (N * sizeof(char *));
     printf("\nEnter Element  of Matrix: \n");
     for(x=0;x<N;x++)
     {
          matrix[x] =(char*) malloc (M * sizeof(char ));
           for(y=0;y<M;y++)
              {
                  scanf(" %c", &matrix[x][y]);
              }
     }

    #pragma omp parallel shared(matrix) private(i,k)
    {

        #pragma omp for schedule(static)
        for (i=0; i < N; i++)
        {
            for(k=1;k<M-1;k++)
          {

             if((matrix[i][k]=='o')&&(matrix[i][k-1]=='x')&&(matrix[i][k+1]=='x'))
             {
                 matrix[i][k]='x';
             }

          }

        }
    }   /* end of parallel region */

     printf("\noutput is:-\n");
    for (x=0; x< N; x++)
        {
            for(y=0;y<M;y++)
          {
               printf("%c ",matrix[x][y]);

          }
           printf("\n");

        }

}
