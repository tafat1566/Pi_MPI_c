#include<stdio.h>
#include<stdlib.h>
#include <mpi.h>
#include <time.h>
#include <math.h>
#include<unistd.h>


/************************* Programme cree par Aghiles TAFAT***********************/




/******************* Calcul de Pi ************************/
		
double f_pi(double x)
{
	return 4.0/(1+x*x);
}


/******************* Calcul de l'integrale (Trapèze) ************************/


double Trapeze(long double p_a,long double p_b, double n)
		{

		long double x,sum = 0, h;
		h=(p_b-p_a)/n;
		for(int i=0;i<n;i++)
		 {
		   x=p_a+i*h;
		   sum=sum+h*f_pi(x);
		}
		  return sum;
}


/************************ Fonction principale *******************************/


int main(int argc, char* argv[])
	{
	
	
	int t0=0,t=1,nump,mon_id,n;
	double start_time,end_time,total_time,erreur_pi,Pi_r=3.14159265358979323846; 
	long double mon_pi,Pi, p_a,p_b;
	
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nump);
	MPI_Comm_rank(MPI_COMM_WORLD, &mon_id);


	if (mon_id == 0){ 
		if (argc == 1){ 
			printf("Entrez le nombre de points d’intégration:");
			scanf("%d",&n);
		}else {
			n = atoi(argv[1]); 
		}
    }

	p_a=t0+mon_id*((t-t0)/(float)nump);
	p_b=p_a+((t-t0)/(float)nump);

	start_time=MPI_Wtime();

	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

	mon_pi=Trapeze(p_a,p_b,n/nump);	

	sleep(mon_pi);
	end_time = MPI_Wtime();
	total_time =end_time - start_time;

	
	MPI_Reduce(&mon_pi,&Pi,1.0,MPI_LONG_DOUBLE,MPI_SUM,  0,MPI_COMM_WORLD);
		
	double TF = MPI_Wtime();
	if(mon_id==0){
		erreur_pi = fabs(Pi_r - Pi) / Pi_r;
		printf("Pi= %.16Lf\nErreur = %.2e\nLe temps de calcul=%8.4f s\n",Pi,erreur_pi,total_time);
		}	
		

	MPI_Finalize();
	return 0;

 }


