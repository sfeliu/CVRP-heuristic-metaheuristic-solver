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
        save_test(g, filepath, "savings", resultado);
        // print_result(g,resultado);
        if (simAnnealing == 1){
            int vecindario = atoi(argv[4]);
            int enfriar = atoi(argv[5]);
            int cant_iteraciones = atoi(argv[6]);
            Resultado res = g.simulatedAnnealing(resultado, enfriar, vecindario, cant_iteraciones);
            save_test(g, filepath, "savings_sa", res.camiones);
            // print_result(g, res.camiones);
        }
    }
    if(mode==1){
        Grafo g = Grafo(tsp, "kn");
        vector< vector<int> > clusters = g.solveVSP(); 
        vector <Camion> resultado = g.generateCamiones(clusters);
        save_test(g,filepath,"sweep",resultado);
        // print_result(g,resultado);

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
        save_test(g, filepath, "radial", resultado);
        // print_result(g,resultado);

        if (simAnnealing == 1){
            int vecindario = atoi(argv[4]);
            int enfriar = atoi(argv[5]);
            int cant_iteraciones = atoi(argv[6]);
            Resultado res = g.simulatedAnnealing(resultado, enfriar, vecindario, cant_iteraciones);
            save_test(g, filepath, "radial_sa", res.camiones);
            // print_result(g, res.camiones);
        }
    }
    if(mode==3) {
        Grafo g = Grafo(tsp, "kn");
        vector<vector<int> > clusters = g.GolosoMasCercano();
        vector<Camion> resultado = g.generateCamiones(clusters);
        save_test(g, filepath, "goloso", resultado);
        // print_result(g,resultado);

        if (simAnnealing == 1){
            int vecindario = atoi(argv[4]);
            int enfriar = atoi(argv[5]);
            int cant_iteraciones = atoi(argv[6]);
            Resultado res = g.simulatedAnnealing(resultado, enfriar, vecindario, cant_iteraciones);
            save_test(g, filepath, "goloso_sa", res.camiones);
            // print_result(g, res.camiones);
        }
    }
    /*if(mode==4){

        Grafo g = Grafo(tsp, "kn");

        vector<int> vec1{30, 74, 21, 47, 36, 69, 71, 60, 70, 20, 37, 5, 48, 29, 45, 4};
        vector<int> vec1{46, 8, 35, 53, 14, 59, 19, 54, 13, 57, 15, 27, 52, 34, 67};
        vector<int> vec1{6, 33, 73, 1, 43, 41, 42, 64, 22, 61, 28, 62, 2, 68, 75};
        vector<int> vec1{7, 11, 66, 65, 38, 10, 31, 55, 25, 9, 39, 72, 58, 26};
        vector<int> vec1{17, 51, 16, 63, 23, 56, 49, 24, 18, 50, 32, 44, 3, 40, 12};






        vector<int> vec1 = {21, 74, 4, 36, 71, 60, 30, 69, 47, 37, 70, 29, 20, 5, 45, 48};
        vector<int> vec2 = {67, 53, 54, 52, 59, 34, 15, 14, 13, 8, 46, 57, 19, 27, 35};
        vector<int> vec3 = {1, 41, 43, 33, 28, 22, 61, 75, 68, 42, 62, 64, 6, 73, 2};
        vector<int> vec4 = {31, 65, 38, 25, 39, 7, 55, 9, 58, 10, 11, 26, 72, 66};
        vector<int> vec5 = {32, 3, 16, 44, 18, 17, 50, 12, 40, 24, 51, 56, 49, 63, 23};
        vector<vector<int> > clusters{vec1,vec2,vec3,vec4,vec5};
        vector<Camion> resultado = g.generateCamiones(clusters);
        save_test(g, filepath, "optimo", resultado);
        // print_result(g,resultado);

        if (simAnnealing == 1){
            int vecindario = atoi(argv[4]);
            int enfriar = atoi(argv[5]);
            double breaking_point = (double)atoi(argv[6]);
            Resultado res = g.simulatedAnnealing_swp(resultado, vecindario, enfriar, breaking_point);
            save_test(g, filepath, "optimo_sa", res.camiones);
            // print_result(g, res.camiones);
        }
    }*/

    return 0;
}


