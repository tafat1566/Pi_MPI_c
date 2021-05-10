#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <time.h>
#include <string.h>

//using namespace std;


//Géstion d'erreurs des appels de la fonction MPI
void Erreur(int erreur) {
  if(erreur != MPI_SUCCESS) {
    fprintf(stderr,"Erreur de démarrage du programme\n");
    MPI_Abort(MPI_COMM_WORLD,erreur);
    MPI_Finalize();
    exit(1);
  }
}

// La fonction implémente la version Monte Carlo pour calculez le nombre en cercle.
  long long int Monte_carlo (int n, int myRank){
   long long int  resultat = 0,z=0;        
  double x,y;
  unsigned int seed = (unsigned) time(NULL);
  
  
  srand(seed + myRank);
  for (int i = 0; i < n; i++) {
     x = rand_r(&seed)/(double)RAND_MAX; // tirage aléatoirement de x entre [0 et 1]
     y = rand_r(&seed)/(double)RAND_MAX; // tirage aléatoirement de y entre [0 et 1]
     if((x*x+y*y) <= 1.0 ) 
        {
            resultat++;

        }

    }
    
    return resultat;
}


int main(void) {
    int comm_sz,i,count_time;
    int my_rank;
    unsigned int seed ;

    double x,y,z,pi,Err,epsilon,begin,end,start_time,end_time,total_time;  

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

     MPI_Status status; // Statut de reciption 
    if (comm_sz <= 1) {
        MPI_Finalize();
        return 0;
    }
    
    long long int Cal[2] = { 0 };

    start_time = MPI_Wtime();
    if (my_rank == 0) {
        
        printf("Entre n : \n");
        scanf("%d",&count_time);

        long long int tempCal[2] = { 0 };

        long long int width = count_time / (comm_sz - 1);
        long long int reminder = count_time % (comm_sz - 1);
        long long int temp_count_time;
        for (int i = 1; i < comm_sz; ++i)
        {
            temp_count_time = width;
            if (i <= reminder) 
                temp_count_time++;
            MPI_Send(&temp_count_time, 1, MPI_LONG_LONG_INT, i, 0, MPI_COMM_WORLD);
        }
        for (int i = 1; i < comm_sz; ++i)
        {
            MPI_Recv(tempCal, 2, MPI_LONG_LONG_INT, i, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
            for (int j = 0; j < 2; ++j)
             { 
                Cal[j] += tempCal[j]; }
        }
        //pi=4.0 * (double)Cal[0]/ (double)count_time ;
        //printf("%f\n",4.0 * Cal[0] / count_time );
        /*float error = (pi- M_PI);

        if(error >=0.)
            error=error;
        else
            error=-1*error;
        printf("Processeurs = %2d\nItérations = %ld\nTemps = %f s\n", count_time, Cal[0],total_time);
        printf("Pi_reel = %23.20lf\npi_calculé = %23.20lf\nErreur = %23.20lf\n", M_PI,pi,error);
     */
     } 
    else {
        srand((unsigned)time(NULL));
        MPI_Recv(&count_time, 1, MPI_LONG_LONG_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        Cal[1] = count_time;
        Cal[0] = 0;
        double x, y, dst;
        for (int i = 0; i < count_time; ++i) {
                Cal[0] =Monte_carlo(count_time,my_rank);
        }
        MPI_Send(Cal, 2, MPI_LONG_LONG_INT, 0, 0, MPI_COMM_WORLD);
    }
    end_time = MPI_Wtime();
    total_time=end_time-start_time;
     if(my_rank==0){
     pi=4.0 * (double)Cal[0]/ (double)count_time ;
        //printf("%f\n",4.0 * Cal[0] / count_time );
               
        
        printf("pi_calculé = %23.20lf\n", pi );
       // printf("Processeurs = %2d\nItérations = %lld\nTemps = %f s\n", count_time, Cal[0],total_time);
        //printf("Pi_reel = %23.20lf\npi_calculé = %23.20lf\nErreur = %23.20lf\n", M_PI,pi,error);
    }
    float error = (pi- M_PI);
    if(error >=0.)
            error=error;
     else
            error=-1*error;
    /*printf("Processeurs = %2d\nItérations = %lld\nTemps = %f s\n", count_time, Cal[0],total_time);
    printf("Pi_reel = %23.20lf\npi_finale = %23.20lf\nErreur = %23.20lf\n", M_PI,pi,error);
    MPI_Finalize();*/
    
    return 0;
}