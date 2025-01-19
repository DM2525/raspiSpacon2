# raspiSpacon2

演習用プログラム

## pi_archimedes.c
アルキメデスの漸化式を使用した円周率を求めるプログラム

## pi_machin.c
マチンの公式を使用した円周率を求めるプログラム。
使用する場合はGMPライブラリをインストールする。<br>
``` sudo apt-get install libgmp-dev ```

## pi_montecalro.c
モンテカルロ法を用いて円周率を求めるプログラム。並列演算の体験。
MPIの環境で実行してください。引数は打つ点の数<br>
``` mpicc -o pi_montecarlo pi_montecarlo.c -lgmp```<br>
``` mpirun -np 4 ./pi_montecarlo 10000 ```

## pi_integral.c
積分近似法を使って円周率を求めるプログラム。MPIの環境で実行してください<br>
第一引数は積分範囲の分割数。第二引数は表示する桁数。
``` mpicc -o pi_integral pi_integral.c```<br>
``` mpirun -np 4 ./pi_integral 1000000 100 ```

