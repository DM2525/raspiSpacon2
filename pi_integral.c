#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

double f(double x) {
    return 4.0 / (1.0 + x * x);
}

double calculate_pi(int start, int end, double step) {
    double sum = 0.0;
    for (int i = start; i < end; i++) {
        double x = (i + 0.5) * step;
        sum += f(x);
    }
    return sum * step;
}

int main(int argc, char *argv[]) {
    int num_procs, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (argc != 3) {
        if (rank == 0) printf("Usage: %s <num_intervals> <precision>\n", argv[0]);
        MPI_Finalize();
        return 1;
    }

    int num_intervals = atoi(argv[1]);
    int precision = atoi(argv[2]);

    double step = 1.0 / num_intervals;
    int intervals_per_proc = num_intervals / num_procs;

    int start = rank * intervals_per_proc;
    int end = (rank == num_procs - 1) ? num_intervals : start + intervals_per_proc;

    double local_sum = calculate_pi(start, end, step);

    double total_sum;
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Calculated π to %d decimal places: %.*f\n", precision, precision, total_sum);
    }

    MPI_Finalize();
    return 0;
}
