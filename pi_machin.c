#include <stdio.h>
#include <gmp.h>

// アークタンジェントをテイラー展開で計算
void arctan(mpf_t result, unsigned int reciprocal, unsigned int terms) {
    mpf_t term, x, x_power, reciprocal_float;
    mpf_init(term);
    mpf_init(x);
    mpf_init(x_power);
    mpf_init(reciprocal_float);

    // x = 1 / reciprocal
    mpf_set_ui(reciprocal_float, reciprocal);
    mpf_ui_div(x, 1, reciprocal_float);

    // 初期化
    mpf_set(x_power, x); // x^1
    mpf_set_ui(result, 0);

    for (unsigned int k = 0; k < terms; k++) {
        mpf_div_ui(term, x_power, 2 * k + 1); // x^(2k+1) / (2k+1)
        if (k % 2 == 1) {
            mpf_neg(term, term); // 符号を切り替え
        }
        mpf_add(result, result, term);

        // x_power *= x^2
        mpf_mul(x_power, x_power, x);
        mpf_mul(x_power, x_power, x);
    }

    // メモリ解放
    mpf_clear(term);
    mpf_clear(x);
    mpf_clear(x_power);
    mpf_clear(reciprocal_float);
}

// マチンの公式で円周率を計算
void calculate_pi(mpf_t pi, unsigned int terms) {
    mpf_t arctan1, arctan2;

    mpf_init(arctan1);
    mpf_init(arctan2);

    // arctan(1/5)
    arctan(arctan1, 5, terms);
    // arctan(1/239)
    arctan(arctan2, 239, terms);

    // π = 16 * arctan(1/5) - 4 * arctan(1/239)
    mpf_mul_ui(arctan1, arctan1, 16);
    mpf_mul_ui(arctan2, arctan2, 4);
    mpf_sub(pi, arctan1, arctan2);

    // メモリ解放
    mpf_clear(arctan1);
    mpf_clear(arctan2);
}

int main() {
    unsigned int terms;
    printf("Enter the number of terms: ");
    scanf("%u", &terms);

    if (terms == 0) {
        printf("Error: The number of terms must be greater than 0.\n");
        return 1;
    }

    mpf_set_default_prec(1000); // 計算精度を設定
    mpf_t pi;
    mpf_init(pi);

    calculate_pi(pi, terms);

    gmp_printf("Using %u terms, the approximate value of pi is: %.50Ff\n", terms, pi);

    mpf_clear(pi);
    return 0;
}
