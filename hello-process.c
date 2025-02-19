#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);  // MPIの初期化

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // ランクの取得

    printf("Hello from process %d\n", rank);

    MPI_Finalize();  // MPIの終了
    return 0;
}
