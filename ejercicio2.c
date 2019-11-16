#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

int main(int argc, char* argv[]) {
    int n = 4;
    int matriz1[n][n];
    int matriz2[n][n];
    int id_proceso, total_procesos;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &total_procesos);
    MPI_Comm_rank(MPI_COMM_WORLD, &id_proceso);

    int matriz_resultados[total_procesos];

    if(id_proceso == 0) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                matriz1[i][j] = 1;
                matriz2[i][j] = 1;
            }
        }
    }

    MPI_Bcast(matriz1, n*n, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(matriz2, n*n, MPI_INT, 0, MPI_COMM_WORLD);

    int suma = 0;
    
    for(int i = (n/total_procesos * id_proceso); i < (n/total_procesos * (id_proceso + 1)); i++) {
        for(int x = 0; x < n; x++) {
            suma += matriz1[i][x] + matriz2[x][i];
        }
    }
    MPI_Send(&suma, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

    if(id_proceso == 0) {
        int sumaTemporal = 0;
        int sumaTotal = suma;
        MPI_Status status;

        for(int i = 1; i < total_procesos; i++) {
            MPI_Recv(&sumaTemporal, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
        }

        printf("%d", sumaTotal);
    }
    
}