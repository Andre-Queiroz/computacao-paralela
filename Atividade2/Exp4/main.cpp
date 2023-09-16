#include <iostream>
#include <mpi.h>
#include <vector>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, numtasks;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

    const int vectorSize = 24;  // Tamanho do vetor (deve ser par e maior que numtasks-1)
    const int workers = numtasks - 1;  // Número de tarefas trabalhadoras

    if (rank == 0) {

        int vetor[vectorSize];

        // Divide o vetor em partes iguais e distribui para os trabalhadores
        int elementsPerWorker = vectorSize / workers;
        for (int i = 1; i < numtasks; ++i) {
            MPI_Send(&vetor[(i - 1) * elementsPerWorker], elementsPerWorker, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        // Processa a parte do mestre
        for (int i = 0; i < elementsPerWorker; ++i) {
            vetor[i] = i + 1;
        }

        // Coleta as partes dos trabalhadores e exibe os elementos selecionados
        for (int i = 1; i < numtasks; ++i) {
            MPI_Recv(&vetor[(i - 1) * elementsPerWorker], elementsPerWorker, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        // Exibe o vetor completo
        std::cout << "Vetor completo:" << std::endl;
        for (int i = 0; i < vectorSize; ++i) {
            std::cout << vetor[i] << " ";
        }
        std::cout << std::endl;
    } else {
        // Processo trabalhador

        int elements_per_worker = vectorSize / workers;
        std::vector<int> worker_part(elements_per_worker);

        // Recebe a parte do vetor do mestre
        MPI_Recv(&worker_part[0], elements_per_worker, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Atribui valores aos elementos da parte do vetor do trabalhador
        for (int i = 0; i < elements_per_worker; ++i) {
            worker_part[i] = (rank - 1) * elements_per_worker + i + 1;
        }

        // Envia a parte do vetor de volta para o mestre
        MPI_Send(&worker_part[0], elements_per_worker, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}

