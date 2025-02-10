#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <mpfr.h>
#include <math.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int rank, size;

    // MPI の初期化
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    // 引数が不足している場合は使い方を表示して終了
    if (argc < 3) {
        if (rank == 0)
            fprintf(stderr, "Usage: %s <num_terms> <display_digits>\n", argv[0]);
        MPI_Finalize();
        return 1;
    }
    
    // 引数から「項数」と「表示桁数」を取得
    long long num_terms = atoll(argv[1]);
    int display_digits = atoi(argv[2]);
    int extra_digits = 10;
    
    // 必要な精度（ビット数）は、10進表示桁数に余分桁を足した値に log2(10) ≒ 3.32193 をかける
    double bits_est = (display_digits + extra_digits) * 3.32193;
    mpfr_prec_t prec = (mpfr_prec_t)ceil(bits_est);
    
//各MPFR変数は指定した精度 (prec) で初期化する
    mpfr_t local_sum1, local_sum2, term, power, denom, temp;
    mpfr_inits2(prec, local_sum1, local_sum2, term, power, denom, temp, (mpfr_ptr) 0);
    mpfr_set_zero(local_sum1, 0);
    mpfr_set_zero(local_sum2, 0);
    
    // x1 = 1/5, x2 = 1/239 の初期化
    mpfr_t x1, x2;
    mpfr_inits2(prec, x1, x2, (mpfr_ptr) 0);
    mpfr_set_ui(temp, 5, MPFR_RNDN);
    mpfr_ui_div(x1, 1, temp, MPFR_RNDN);   // x1 = 1/5
    mpfr_set_ui(temp, 239, MPFR_RNDN);
    mpfr_ui_div(x2, 1, temp, MPFR_RNDN);   // x2 = 1/239

    //各プロセスで部分和を計算する
    //各プロセスは n = rank, rank+size, rank+2*size, … の項について、(-1)^n * (x^(2n+1))/(2n+1)をMPFR で計算してlocal_sum1（x1 用）および local_sum2（x2 用）に加算する
    for (long long n = rank; n < num_terms; n += size) {
        // 指数 (2n+1)
        unsigned long exponent = 2*n + 1;
        // x1 用の項の計算
        // power = (1/5)^(2n+1)
        mpfr_pow_ui(power, x1, exponent, MPFR_RNDN);
        // denom = 2n+1
        mpfr_set_ui(denom, exponent, MPFR_RNDN);
        // term = power/denom
        mpfr_div(term, power, denom, MPFR_RNDN);
        if (n % 2 == 1) {
            // 奇数項は符号を反転
            mpfr_neg(term, term, MPFR_RNDN); 
        }
        mpfr_add(local_sum1, local_sum1, term, MPFR_RNDN);
        
        // x2 用の項の計算
    // power = (1/239)^(2n+1)
        mpfr_pow_ui(power, x2, exponent, MPFR_RNDN);
        // term = power/denom
        mpfr_div(term, power, denom, MPFR_RNDN);
        if (n % 2 == 1) {
            mpfr_neg(term, term, MPFR_RNDN);
        }
        mpfr_add(local_sum2, local_sum2, term, MPFR_RNDN);
    }
    
    // 各プロセスは自分の部分和を文字列に変換。
// 表示桁数 + 余分桁 (extra_digits) で内部結果を文字列に変換
     // mpfr_asprintf() は必要なメモリを自動確保して返す。
    char *local_str_sum1 = NULL;
    char *local_str_sum2 = NULL;
    mpfr_asprintf(&local_str_sum1, "%.*Rf", display_digits + extra_digits, local_sum1);
    mpfr_asprintf(&local_str_sum2, "%.*Rf", display_digits + extra_digits, local_sum2);
    
    // MPI を用いた部分和の集約
    // 各プロセスは自分の部分和を文字列としてrank 0 に送信し、rank 0 で MPFR に再構成して加算する
    if (rank != 0) {
        int len1 = (int)(strlen(local_str_sum1) + 1);
        int len2 = (int)(strlen(local_str_sum2) + 1);
        // タグ 0,1 は x1 用（arctan(1/5)）
        // タグ 2,3 は x2 用（arctan(1/239)）
        MPI_Send(&len1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(local_str_sum1, len1, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
        MPI_Send(&len2, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Send(local_str_sum2, len2, MPI_CHAR, 0, 3, MPI_COMM_WORLD);
    }
    
    // rank 0 で全プロセスの部分和を受信して加算
    mpfr_t global_sum1, global_sum2;
    mpfr_inits2(prec, global_sum1, global_sum2, (mpfr_ptr) 0);
    if (rank == 0) {
        mpfr_set(global_sum1, local_sum1, MPFR_RNDN);
        mpfr_set(global_sum2, local_sum2, MPFR_RNDN);
        
        MPI_Status status;
        for (int i = 1; i < size; i++) {
            int recv_len1, recv_len2;
            // x1 用の部分和を受信
            MPI_Recv(&recv_len1, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            char *recv_str_sum1 = (char *) malloc(recv_len1 * sizeof(char));
            MPI_Recv(recv_str_sum1, recv_len1, MPI_CHAR, i, 1, MPI_COMM_WORLD, &status);
            {
                mpfr_t temp_val;
                mpfr_init2(temp_val, prec);
                mpfr_set_str(temp_val, recv_str_sum1, 10, MPFR_RNDN);
                mpfr_add(global_sum1, global_sum1, temp_val, MPFR_RNDN);
                mpfr_clear(temp_val);
            }
            free(recv_str_sum1);
            
            // x2 用の部分和を受信
            MPI_Recv(&recv_len2, 1, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
            char *recv_str_sum2 = (char *) malloc(recv_len2 * sizeof(char));
            MPI_Recv(recv_str_sum2, recv_len2, MPI_CHAR, i, 3, MPI_COMM_WORLD, &status);
            {
                mpfr_t temp_val;
                mpfr_init2(temp_val, prec);
                mpfr_set_str(temp_val, recv_str_sum2, 10, MPFR_RNDN);
                mpfr_add(global_sum2, global_sum2, temp_val, MPFR_RNDN);
                mpfr_clear(temp_val);
            }
            free(recv_str_sum2);
        }
        
        // Machinの公式により円周率を計算
        mpfr_t pi;
        mpfr_init2(pi, prec);
        mpfr_mul_ui(pi, global_sum1, 16, MPFR_RNDN);
        mpfr_mul_ui(temp, global_sum2, 4, MPFR_RNDN);
        mpfr_sub(pi, pi, temp, MPFR_RNDN);
        
        // 指定された桁数 (display_digits) で結果を表示
        mpfr_printf("Computed pi = %.*Rf\n", display_digits, pi);
        
        mpfr_clear(pi);
    }
    
    // 後始末
    //  MPFR 変数の解放と、動的確保した文字列の解放
    mpfr_clears(local_sum1, local_sum2, term, power, denom, temp, x1, x2, global_sum1, global_sum2, (mpfr_ptr) 0);
    free(local_str_sum1);
    free(local_str_sum2);
    
    MPI_Finalize();
    return 0;
}
