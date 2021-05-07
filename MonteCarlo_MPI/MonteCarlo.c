#include<stdio.h>
#include<stdlib.h>
#include <mpi.h>
#include <time.h>
#include <math.h>
#include<unistd.h>
#define N_it 100000000



// La fonction implémente la version Monte Carlo pour calculez le nombre en cercle.
unsigned long long int Monte_carlo (unsigned long long int n){
 unsigned long long int  resultat = 0;        
  double x,y;
  unsigned int seed = (unsigned) time(NULL);
 
  for (int i = 0; i < n; i++) {

  	//Tirez 2 nombres aléatoires x et y
     x = rand_r(&seed)/(double)RAND_MAX; 
     y = rand_r(&seed)/(double)RAND_MAX; 
     // Vérifiez si (x, y) est dans le cercle
     if((x*x+y*y) <= 1.0 ) 
        {
            resultat++;
        }
    }
    return resultat;
}

int main(int argc, char* argv[])

{
	//unsigned long long int n_it = 1000000;
	unsigned long long int resultat=0;
	int myid,nbprocess,i,resultat_all;
	double x_coord, y_coord, pi, end_time,start_time,r, pi_all,Pi_r = 3.14159265358979323846;

	// Iinitialisation de MPI
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nbprocess);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	resultat=0;
	start_time = MPI_Wtime();
	// resultat = estimation de PI
	resultat = Monte_carlo(N_it);
	

	MPI_Allreduce(&resultat,&resultat_all,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
	if(myid==0)
	{
		pi_all=4.0*resultat_all/(N_it*nbprocess);
		printf(" throw=%d, pi=%lf\n",N_it*nbprocess,pi_all );
	}

	total_time=end_time - start_time;
	if(myid==0)
	{
		printf("Pi_reel= %.16f\n",Pi_r);
	}

	return 0;

}