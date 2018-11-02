#include "include/grafo.hpp"
#include "include/misc.hpp"

int main(int argc, char** argv){
    TspFile tsp = readTsp(argv[1]);
    Grafo g = Grafo(tsp, "star");
	return 0;
}
