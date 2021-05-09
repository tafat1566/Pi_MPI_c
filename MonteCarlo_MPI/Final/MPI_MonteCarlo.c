
/********************************************************************************/
/**
 * Auteur:   GUENANA Youghourta
 *
 * Fichier:  pi_client_serveur.c
 *
 * Objectif: Implémenter une version de calcul qui estime PI de type client/serveur. 
 *           Le serveur est un processus qui collecte les résultats partiels calculés par les clients.
 *
 **/ 
/********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <time.h>
#include <string.h>
#define N_it 100000000


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
 unsigned long long int Monte_carlo (unsigned long long int n, int myRank){
  unsigned long long int  resultat = 0,z=0;        
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


int main(int argc, char* argv[]){



    
    unsigned long long int n_it = 100,resultat = 0;            
    int myid; //identificateur du processus
    int nbprocess; // nombre de processus 
    int i;
    double x,y,z,pi,Err,epsilon,begin,end,start_time,end_time,total_time;  
    epsilon = 1/sqrt(n_it);                                                   
    
    MPI_Status status; // Statut de reciption 

    // Iinitialisation de MPI
    Erreur(MPI_Init(&argc, &argv));                     
    Erreur(MPI_Comm_rank(MPI_COMM_WORLD, & myid));                
    Erreur(MPI_Comm_size(MPI_COMM_WORLD, & nbprocess));

    if (myid == 0) {
        if (argc<2) {
            epsilon = 1e-2;
        } else {
        sscanf ( argv[1], " %lf", &epsilon );
    }
    }
    start_time = MPI_Wtime();

    MPI_Bcast ( &epsilon, 1, MPI_DOUBLE, 0,MPI_COMM_WORLD );
    int resultats[nbprocess];                // Contient tous les processus de comptage des points circle
    int temp;                                 // Tampon temporaire pour stocker les messages de comptage entrants sur le nœud 0

                         

    resultat=Monte_carlo(n_it,myid);

    if(myid != 0){    

                          // Tout le monde envoie le resultat au processus 0 
        MPI_Send(&resultat, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        
    } else{                                   // Sinon, le nœud 0 doit recevoir un message de tous les autres       
        resultats[0] = resultat;                    // Mettre le compte de root en place 
        
        for(i = 0; i < nbprocess-1; i++){   
                                              // On ne sait pas qui sera terminé en premier, on a donc besoin de poser un reçu MPI_ANY_SOURCE 
            MPI_Recv(&temp, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);    
            resultats[status.MPI_SOURCE] = temp; // Découvrir de qui provient ce message MPI_Recv () */

        }
    }

    end_time = MPI_Wtime();
    total_time=end_time-start_time;
    /* Ok, le root a les contributions de tout le monde, additionner-les et calculer Pi */
    if (myid == 0){      
        
        int sum = 0;        
        for(i = 0; i < nbprocess; ++i){
            sum += resultats[i];
            
        }

        long total_n_it = nbprocess * n_it;
        pi = ((double)sum / (double)total_n_it) * 4.0; 

        double done,request;
        float error = (pi- M_PI);

        if(error >=0.)
        	error=error;
        else
        	error=-1*error;
        
       
        printf("Processeurs = %2d\nItérations = %ld\nTemps = %f s\n", nbprocess, total_n_it,total_time);
       	printf("Pi_reel = %23.20lf\npi_calculé = %23.20lf\nErreur = %23.20lf\n", M_PI,pi,error);
        //printf("pi_calculé = %23.20lf\n", pi );
    }

    
    Erreur(MPI_Finalize());
    return 0;
}

