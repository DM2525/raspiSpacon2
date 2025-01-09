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
MPIの環境で実行してください。<br>
``` mpicc -o montecarlo_pi mpi_montecarlo_pi.c ```<br>
``` mpirun -np 4 ./montecarlo_pi 10000 ```
