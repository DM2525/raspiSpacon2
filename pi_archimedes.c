#include <stdio.h>
#include <math.h>

void calculate_sequence(int iterations) {
    double a_n = 2 * sqrt(3); // 初期値 a_0
    double b_n = 3;           // 初期値 b_0

    printf("n\ta_n\tb_n\n");
    printf("0\t%.20f\t%.20f\n", a_n, b_n);

    for (int n = 1; n <= iterations; n++) {
        double a_next = (2 * a_n * b_n) / (a_n + b_n); // a_(n+1)
        double b_next = sqrt(a_next * b_n);            // b_(n+1)

        a_n = a_next;
        b_n = b_next;

        printf("%d\t%.20f\t%.20f\n", n, a_n, b_n);
    }
}

int main() {
    int iterations;

    printf("Enter the number of iterations: ");
    scanf("%d", &iterations);

    calculate_sequence(iterations);

    return 0;
}