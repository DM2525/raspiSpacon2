# raspiSpacon2

演習用プログラム

## pi_archimedes.c
アルキメデスの漸化式を使用した円周率を求めるプログラム<br>
``` gcc pi_archimedes.c -o pi_archimedes -lm ```<br>
``` ./pi_archimedes ```<br>

## pi_machin.c
マチンの公式を使用した円周率を求めるプログラム。<br>
使用する場合はGMPライブラリをインストールする。<br>
``` sudo apt-get install libgmp-dev ```<br>
計算精度はコード内<br>
``` gcc pi_machin.c -o pi_machin -lgmp ```<br>
``` ./pi_machin ```<br>

## pi_montecalro.c
モンテカルロ法を用いて円周率を求めるプログラム。並列演算の体験。<br>
MPIの環境で実行してください。引数は打つ点の数<br>
``` mpicc pi_montecarlo.c -o pi_montecarlo -lgmp ```<br>
``` mpirun -np 4 ./pi_montecarlo 10000 ```<br>

## pi_integral.c
積分近似法を使って円周率を求めるプログラム。MPIの環境で実行してください。<br>
第一引数は積分範囲の分割数。第二引数は表示する桁数。<br>
``` mpicc pi_integral.c -o pi_integral ```<br>
``` mpirun -np 4 ./pi_integral 1000000 100 ```

## pi_machin_mpi.c
マチンの公式を使用し並列化して円周率を求めるプログラム。<br>
使用する場合はGMPライブラリをインストールする。<br>
第1引数に項数、第2引数に表示する桁数<br>
``` sudo apt-get install libmpfr-dev ```<br>
``` mpicc pi_machin_mpi.c -o pi_machin_mpi -lmpfr -lgmp -lm ```<br>
``` mpirun -np 4 ./pi_machin_mpi 10000 20 ```<br>

## hello-process.c
MPIの初歩プログラム。
``` mpicc hello-process.c -o hello-process```<br>
``` mpirun -np 4 ./hello-process ```<br>
