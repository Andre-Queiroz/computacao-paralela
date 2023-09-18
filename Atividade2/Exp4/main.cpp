#include <iostream>
#include <mpi.h>
#include <vector>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, numtasks;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

    const int vectorSize = 24;
    const int workers = numtasks - 1;

    if (rank == 0)
    {
        int vetor[vectorSize];

        int elementsPerWorker = vectorSize / workers;
        for (int i = 1; i < numtasks; ++i)
        {
            MPI_Send(&vetor[(i - 1) * elementsPerWorker], elementsPerWorker, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        for (int i = 0; i < elementsPerWorker; ++i)
        {
            vetor[i] = i + 1;
        }

        for (int i = 1; i < numtasks; ++i)
        {
            MPI_Recv(&vetor[(i - 1) * elementsPerWorker], elementsPerWorker, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        std::cout << "Vetor completo:" << std::endl;
        for (int i = 0; i < vectorSize; ++i)
        {
            std::cout << vetor[i] << " ";
        }
        std::cout << std::endl;
    }
    else
    {
        int elements_per_worker = vectorSize / workers;
        std::vector<int> worker_part(elements_per_worker);

        MPI_Recv(&worker_part[0], elements_per_worker, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for (int i = 0; i < elements_per_worker; ++i)
        {
            worker_part[i] = (rank - 1) * elements_per_worker + i + 1;
        }

        MPI_Send(&worker_part[0], elements_per_worker, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}
