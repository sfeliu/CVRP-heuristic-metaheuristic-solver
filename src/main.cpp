#include "include/grafo.hpp"
#include "include/misc.hpp"
#include <string>

int main(int argc, char** argv){
    string filepath(argv[1]);
    TspFile tsp = readTsp(argv[1]);
    int mode = atoi(argv[2]);
    if(mode==0) {
        Grafo g = Grafo(tsp, "kn");
        vector<Camion> resultado = g.heuristica_savings();
        save_test(g, filepath, "savings", resultado);
        Resultado res = g.simulatedAnnealing(resultado, 0, 1, 0);
        save_test(g, filepath, "savings_sa", res.camiones);
    }
    if(mode==1){
        Grafo g = Grafo(tsp, "kn");
        // vector< vector<int> > clusters = g.solveVSP_conTwoOpt2(); //solveVSP no usa ninguna heuristica, solveVSP_conTwoOpt usa 2opt pero mira solo una vecindad, solveVSP_conTwoOpt2 mira todas las vecindades.
        // vector <Camion> resultado = g.generateCamiones(clusters);
        vector< vector< vector<int> > > vecindad_ini = g.vecindadCompletaInicialTwoOpt();   
        vector< vector< vector<int> > > vecindades = g.vecindadTwoOpt(vecindad_ini); 
        vector <Camion> resultado = g.generateCamiones(vecindades[661499]);
        save_test(g,filepath,"sweep",resultado);
    }
    if(mode==2) {
        Grafo g = Grafo(tsp, "kn");
        vector<vector<int> > clusters = g.NNRadial();
        vector<Camion> resultado = g.generateCamiones(clusters);
        save_test(g, filepath, "radial", resultado);
        Resultado res = g.simulatedAnnealing(resultado, 0, 0, 0);
        save_test(g, filepath, "radial_sa", res.camiones);
    }
    if(mode==3) {
        Grafo g = Grafo(tsp, "kn");
        vector<vector<int> > clusters = g.GolosoMasCercano();
        vector<Camion> resultado = g.generateCamiones(clusters);
        save_test(g, filepath,
                  "goloso", resultado);
    }

    return 0;
}
