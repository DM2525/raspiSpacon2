#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

int main(int argc, char **argv) {
    int rank, size;
    int local_count = 0;
    int global_count = 0;
    double x, y;

    // MPIの初期化
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // プロセスのランクを取得
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // 総プロセス数を取得

    // コマンドライン引数からNUM_POINTSを取得
    if (argc != 2) {
        if (rank == 0) {
            printf("Usage: %s <number_of_points>\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    int num_points = atoi(argv[1]);  // 入力された点の数を取得

    // 乱数の初期化（各プロセスで異なるシードを使用）
    srand(time(NULL) + rank);

    // 各プロセスがモンテカルロ法を実行
    for (int i = 0; i < num_points; i++) {
        x = (double)rand() / RAND_MAX;
        y = (double)rand() / RAND_MAX;
        if (x * x + y * y <= 1.0) {
            local_count++;
        }
    }
  
   // 各プロセスの結果を集計
    MPI_Reduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WO>

    // マスター（ランク0）のプロセスで円周率を計算
    if (rank == 0) {
        double pi = 4.0 * global_count / (num_points * size);
        printf("Estimated value of Pi: %f\n", pi);
    }

    // MPIの終了
    MPI_Finalize();
    return 0;
}
