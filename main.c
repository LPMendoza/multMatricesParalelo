//Cargamos las librerias necesarias para el programa
#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {

//Vairables para guardar el total de procesos y el proceso actual

    int total_procs, id_proc;

//Se guarda el total de procesadores y el procesador en proceso

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id_proc);
    MPI_Comm_size(MPI_COMM_WORLD, &total_procs);

/*Variables para guardar el tamaño de n y m, opc sera la variable
que guarde la opción del menú que eligio el usuario*/

    int n, m, opc;

/*Si el proceso es el 0 Se le despliega un menú al usuario para que elija entre
ingresar el los datos por teclado o generarlos aleatorios*/

    if(id_proc == 0) {
        printf("1 = Elementos por teclado.\n");
        printf("2 = Elementos aleatorios.\n");
        printf("Ingresa una opción: ");
        scanf("%d", &opc);
    }

/*MPI_Barrier bloque la ejecución hasta que todos los procesos la llamen,
esto es para que todos los procesos esten bloqueados hasta que el proceso 0
haya recibido la opción que eligio el usuario*/

    MPI_Barrier(MPI_COMM_WORLD);

//Aquí si el proceso es el 0, se le indica al usaurio que ingrese m y n

    if(id_proc == 0) {
        printf("Ingresa m: ");
        scanf("%d", &m);

        printf("\nIngresa n: ");
        scanf("%d", &n);
    }

/*El proceso 0 realiza un mensaje de broadcast con m y n
y se bloquea la ejecución hasta que todos completen esa tarea*/

    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);


/*P es la cantidad de renglones, se iguala a m ya que para poder
realizar la multiplicación de matrices debe cumplir la condición de
m = p */

    int p = m;  

/*Se crean las matrices A y B, que seran las que se multiplicaran
A es de orden m x n, y B es de orden n * p */
    
    int A[m][n];
    int B[p][n];

/* Se crea la variable bloque que define el tamaño del bloque de 
datos que se le pasará a cada proceso*/

    int bloque = n / total_procs;

/*Se crea Ap que tendra el bloque de datos de cada proceso, C es
el array resultante de la multiplicación y Cp es la variable que tiene
el renglon resultante de cada proceso*/

    int Ap[n];
    int C[m][p];
    int Cp[m];

/*Si es el proceso 0, se compara la opción que eligio el usuario, si 
es 0 entonces mediante un ciclo se llenan por teclado las matrices*/
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

/*Si no es 1 la opción entonces las matrices se llenan con números
random entre el 1 y 10 para evitar multiplicar por 0 */

        else {
            for(int i = 0; i < m; i++) {
                for(int j = 0; j < p; j++) {
                    A[i][j] = 1 + rand()%(11-1);
                    B[i][j] = 1 + rand()%(11-1);
                }       
            }

        }
    }

/*Se bloquean todos los procesos hasta que el proceso 0 realice lo anterior
ya que si no es asi, entonces los demas procesos harian calculos con matrices
vacias y esto causaria un error */

    MPI_Barrier(MPI_COMM_WORLD);

/* El proceso 0 con un mensaje de broadcast comparte la matriz B a los demas procesos
y con MPI_Scatter distribuye el bloque que corresponde a cada proceso de la matriz A */

    MPI_Bcast(B, (m*n), MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(A, (bloque * total_procs), MPI_INT, Ap, (bloque * total_procs), MPI_INT, 0, MPI_COMM_WORLD);

/* Con un ciclo for el array Cp se iguala a 0, ya que en el se sumaran los productos
entre cada elemento de A y B */

    for(int i = 0; i < m; i++) {
        Cp[i] = 0;
    }

/* Mediante este ciclo se realiza la multiplicación del bloque de A de cada procesos
con el renglon que le corresponde de B a cada proceso */

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < p; j++) {
            Cp[i] += (Ap[i] * B[j][id_proc]);
        }
    }

/* MPI_Gather obtiene Cp de cada proceso y lo guarda en la matriz C*/

    MPI_Gather(Cp, (bloque * total_procs), MPI_INT, C, (bloque * total_procs), MPI_INT, 0, MPI_COMM_WORLD);

/*Si es el proceso 0, mediante un ciclo se imprime la matriz C */

    if(id_proc == 0) {
        for(int i = 0; i < m; i++) {
            for(int j = 0; j < n; j++) {
                printf("%d,", C[i][j]);
            }
            printf("\n");
        }
    }

/* Se finaliza el entorno MPI */

    MPI_Finalize();
}