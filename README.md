# algo3TP3
Para correr el TP
- mkdir build && cd build
- cmake ..
- make
- ./tp ../tests\_cases/archivo.txt modo SA vecindario enfriar breakingPoint 

El primer parametro indica el modo:
- 0: savings
- 1: clusterize first run second (sweep)
- 2: clusterize first run second
- 3: constructiva golosa

El segundo parametro indica si es con o sin Simulated Annhealing:
- 0: Sin SA
- 1: Con SA

Si es con SA, entran en juego estos parametros:
- vecindario (0,1)
- modo de enfriar (0,1,2)
- breaking point (0.0,100.0)
