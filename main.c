//Cargamos las librerias necesarias para el programa
#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {

//Vairables para guardar el total de procesos y el proceso actual
    int total_procs, id_proc;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id_proc);
    MPI_Comm_size(MPI_COMM_WORLD, &total_procs);

    int n, m, opc;

    if(id_proc == 0) {
        printf("1 = Elementos por teclado.\n");
        printf("2 = Elementos aleatorios.\n");
        printf("Ingresa una opción: ");
        scanf("%d", &opc);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    
    if(id_proc == 0) {
        printf("Ingresa m: ");
        scanf("%d", &m);

        printf("\nIngresa n: ");
        scanf("%d", &n);
    }
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    int p = m;
      
    int A[m][n];
    int B[p][n];

    int bloque = n / total_procs;
    int Ap[n];
    int C[m][p];
    int Cp[m];

    if(id_proc == 0) {
        if(opc == 1) {
            for(int i = 0; i < n; i++) {
                for(int j = 0; j < m; j++) {
                    printf("\nIngresa A[%d][%d]: ", i, j);
                    scanf("%d", &A[i][j]);
                    printf("\nIngresa B[%d][%d]: ", i, j);
                    scanf("%d", &B[i][j]);
                }
            }
        }
        else {
            for(int i = 0; i < m; i++) {
                for(int j = 0; j < p; j++) {
                    A[i][j] = 1 + rand()%(11-1);
                    B[i][j] = 1 + rand()%(11-1);
                }       
            }

        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(B, (m*n), MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(A, (bloque * total_procs), MPI_INT, Ap, (bloque * total_procs), MPI_INT, 0, MPI_COMM_WORLD);
    
    for(int i = 0; i < m; i++) {
        Cp[i] = 0;
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < p; j++) {
            Cp[i] += (Ap[i] * B[j][id_proc]);
        }
    }

    MPI_Gather(Cp, (bloque * total_procs), MPI_INT, C, (bloque * total_procs), MPI_INT, 0, MPI_COMM_WORLD);

    if(id_proc == 0) {
        for(int i = 0; i < m; i++) {
            for(int j = 0; j < n; j++) {
                printf("%d,", C[i][j]);
            }
            printf("\n");
        }
    }

}