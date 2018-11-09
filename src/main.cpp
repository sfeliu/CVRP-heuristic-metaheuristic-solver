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
    }
    if(mode==1){
        Grafo g = Grafo(tsp, "kn");
	    vector< vector<int> > clusters = g.solveVSP();
	    vector <Camion> resultado = g.generateCamiones(clusters);
	    save_test(g,filepath,"sweep",resultado);
    }

    return 0;
}
