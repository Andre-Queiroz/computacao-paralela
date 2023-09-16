#include "mpi.h"
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
    int rank, size, i;
    int tag = 1010;

    MPI_Status status;
    int msg = 12345;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        MPI_Bcast(&msg, 1, MPI_INT, rank, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Bcast(&msg, 1, MPI_INT, rank, MPI_COMM_WORLD);
        cout << "\nInteiro recebido: " << msg << endl;
    }
    MPI_Finalize();
    return 0;
}