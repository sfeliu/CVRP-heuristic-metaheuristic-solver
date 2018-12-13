#include "include/grafo.hpp"
#include "include/misc.hpp"
#include <string>

int main(int argc, char** argv){
    string filepath(argv[1]);
    TspFile tsp = readTsp(argv[1]);
    int mode = atoi(argv[2]);
    int simAnnealing = atoi(argv[3]);

    if(mode==0) {
        Grafo g = Grafo(tsp, "kn");
        vector<Camion> resultado = g.heuristica_savings();
        // save_test(g, filepath, "savings", resultado);
        print_result(g,resultado);
        if (simAnnealing == 1){
            int vecindario = atoi(argv[4]);
            int enfriar = atoi(argv[5]);
            int cant_iteraciones = atoi(argv[6]);
            Resultado res = g.simulatedAnnealing(resultado, enfriar, vecindario, cant_iteraciones);
            // save_test(g, filepath, "savings_sa", res.camiones);
            print_result(g, res.camiones);
        }
    }
    if(mode==1){
        Grafo g = Grafo(tsp, "kn");
        vector< vector<int> > clusters = g.solveVSP(); 
        vector <Camion> resultado = g.generateCamiones(clusters);
        // save_test(g,filepath,"sweep",resultado);
        print_result(g,resultado);

        if (simAnnealing == 1){
            int vecindario = atoi(argv[4]);
            int enfriar = atoi(argv[5]);
            int cant_iteraciones = atoi(argv[6]);
            Resultado res = g.simulatedAnnealing(resultado, enfriar, vecindario, cant_iteraciones);
            save_test(g, filepath, "sweep_sa", res.camiones);
            // print_result(g, res.camiones);
        }
    }
    if(mode==2) {
        Grafo g = Grafo(tsp, "kn");
        vector<vector<int> > clusters = g.NNRadial();
        vector<Camion> resultado = g.generateCamiones(clusters);
        // save_test(g, filepath, "radial", resultado);
        print_result(g,resultado);

        if (simAnnealing == 1){
            int vecindario = atoi(argv[4]);
            int enfriar = atoi(argv[5]);
            int cant_iteraciones = atoi(argv[6]);
            Resultado res = g.simulatedAnnealing(resultado, enfriar, vecindario, cant_iteraciones);
            // save_test(g, filepath, "radial_sa", res.camiones);
            print_result(g, res.camiones);
        }
    }
    if(mode==3) {
        Grafo g = Grafo(tsp, "kn");
        vector<vector<int> > clusters = g.GolosoMasCercano();
        vector<Camion> resultado = g.generateCamiones(clusters);
        // save_test(g, filepath, "goloso", resultado);
        print_result(g,resultado);

        if (simAnnealing == 1){
            int vecindario = atoi(argv[4]);
            int enfriar = atoi(argv[5]);
            int cant_iteraciones = atoi(argv[6]);
            Resultado res = g.simulatedAnnealing(resultado, enfriar, vecindario, cant_iteraciones);
            // save_test(g, filepath, "goloso_sa", res.camiones);
            print_result(g, res.camiones);
        }
    }

    return 0;
}


