
#ifndef _GRAFO_HPP_
#define _GRAFO_HPP_


#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <math.h>
#include <queue>
#include <chrono>
#include <limits> 
#include <list>
#include <ctime>
#define INF numeric_limits<int>::infinity()
using namespace std;

struct Coordenadas
	{
		double x;
		double y;	
	};


struct Camion
    {
        int id;
        vector<int> circuito;
        int merch;
        double distancia;
    bool operator==(const Camion& a) const
    {
        return (id == a.id && circuito == a.circuito && merch == a.merch && distancia == a.distancia);
    }
    };


struct Resultado
    {
        vector<Camion> camiones;
        double costo_total;
        bool operator==(const Resultado& a) const
        {
            return (camiones == a.camiones && costo_total == a.costo_total);
        }
        bool operator<(const Resultado& a) const
        {
            return (costo_total < a.costo_total);
        }
    };

struct Saving{
    tuple<int,int> camiones;
    tuple<int,int> vertices_unidos;
    double ahorro;
    int merch;
};


struct TspFile
{
    int capacidad;
    vector<Coordenadas> coordenadas;
    vector<int> demandas;
    int deposito;
};

typedef vector< tuple <int,int,double> > listAristas;

class Comparador
{
public:
	int operator()(tuple<int,double> t ,tuple<int,double> s){
		return get<1>(t) > get<1>(s);
	}
};

class Grafo
{
	struct Node
	{
		double weight;
		int id;
	};

	

	public:
		vector<Coordenadas>& puntos();
		Grafo(TspFile tsp, string mode);
		Grafo(vector<Coordenadas> puntos); // constructor de grafo Kn con coordenadas
		//Grafo(vector<vector<double>> pesos); // constructor de grafo Kn con matriz de pesos
		Grafo(listAristas l, int cantNodos); // Contructor con listaAristas.
		bool existe(int u, int v);
		void imprimir();
		//void new_node();
		void new_node(int n);
		void borrar_edge(int u, int v);
		void add_edge(int u, int v, double weight);
		//void add_directional_edge(int u, int v, double weight);
		//void conjunction_pc(int u, int v);
		//int find_pc(int id);
		void init_kruskal_pc();
		void imprimir_pos();
		vector<Saving> calcular_savings(vector<Camion> camiones);
		vector<Camion> heuristica_savings();
        vector<Saving> merge_and_update_savings(vector<Saving> savings, vector<Camion> &camiones);
		//int find(int id);
		//void init_kruskal();
		//void conjunction(int u, int v);
		//listAristas obtener_vecinos(int u, int v, double cant_vecinos);
		//listAristas kruskal(listAristas aristas);
		//listAristas kruskal_pc(listAristas aristas);
		listAristas prim();
		//listAristas convert();

		//void cicloNegativoFW();
		//void cicloNegativoBF();

        //void logPesos();

		vector< vector< vector< int > > > vecindadTwoOptGrande(vector< vector< int > > sol);
		vector< vector< vector< int > > > vecindadTwoOpt(Resultado sol);
		vector< vector< vector< int > > > vecindadCompletaInicialTwoOpt();
		vector< vector<int> > vecindadUnaRutaTwoOpt(vector<int> ruta);
		vector< vector< vector< int > > > vecindadCompletaTwoOpt(vector< vector<int> > sol);

		vector< vector<int> > solveVSP_conTwoOpt2();
		vector< vector<int> > routear_conTwoOpt2( vector< vector<int> > clusters);
		vector< vector<int> > routear_conTwoOpt( vector< vector<int> > clusters);
		vector<int> TwoOptCompleto(vector<int> ruta);
		vector<int> TwoOptswap(vector<int> ruta, int i, int k);
		double calcularDistancia(vector<int> ruta);
		vector< vector<int> > solveVSP_conTwoOpt();

        vector< tuple<int,double> > getAngulos(); //complejidad O(V)
		void sweep_gap(vector< vector <int> >& clusters); //complejidad O(V*log(V))
		void sweep(vector< vector <int> >& clusters); //complejidad O(V*log(V))
		void DFS( vector<int>& inorderWalk, int& actual, int padre); // requiere que g sea digrafo o tengo que sacar las aristas de vuelta en un grafo
		vector<int> solveTSP();
		vector< vector<int> > solveVSP();
		vector< vector<int> > routear( vector< vector<int> > clusters);
		Resultado simulatedAnnealing(vector<Camion> res_inicial, int picking_mode, int enfriar_mode, int vecindario_mode);
		Resultado simulatedAnnealing_swp(vector<Camion> res_inicial, int enfriar_mode, int vecindario_mode, double breaking_point);
		Resultado calcular_resultado(vector<Camion> res);
		Resultado calcular_resultado(vector<vector<int>> res);
		vector<Resultado> get_vecindario(Resultado res, int mode);
        vector<Resultado> vecinos_interchange(Resultado res_inicial);
        bool resultadoFactible(Resultado res);

        double diffEuclidea(int u, int v);
		vector< vector<int> > GolosoMasCercano();
		int tomarMinDist(int nodo, list<int>& l);
		vector<vector <int> > clusterizeRadial();
		vector<int> NearestNeighbourTSP(vector<int> cluster);
        Camion generateCamion(vector<int> circuito, int id);
		vector<Camion> generateCamiones(vector< vector<int> > circuitos);
		int deposito();
        vector<vector<int> > NNRadial();

        private:
		vector<int> _padre;
		vector<int> _altura;
		vector< vector<Node> > _vertices;
		vector<int> _demandas;
		vector<Coordenadas> _puntos;
		int _capacidad;
		int _deposito;

		double& peso(int u, int v);
        void crearKn();
		void crearStar(int center);
		//int floydWarshall(vector< vector<int> > &distancias);
		//int bellmanFord(vector< vector<int> > &distancias);

};

#endif