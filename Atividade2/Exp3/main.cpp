#include "mpi.h"
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
    int rank, size, i;
    int tag = 1010;
    int somaGeral = 0;

    int localValue;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    localValue = rank * 100;

    MPI_Reduce(&localValue, &somaGeral, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        cout << "Resultado: " << somaGeral << endl;
    }

    MPI_Finalize();
    return 0;
}