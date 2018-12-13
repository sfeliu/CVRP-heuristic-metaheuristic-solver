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

using namespace std;

void GenerarGrafos(int cantidad_tests_por_nodo, int cant_min_N, int cant_max_N, int cantidad_ciclos, int granularidad){
	
	for(int cant_actual = cant_min_N; cant_actual < cant_max_N; cant_actual++)
	{
		for(int caso_i = 0; caso_i < cantidad_tests_por_nodo; caso_i++){
			//abrir File
			ofstream test("tests_cases_tiempo/Random-n"+to_string(cant_actual)+"-ciclo-"+to_string(cantidad_ciclos)+"-caso-"+to_string(caso_i)+".txt");
			//Descripciones
			test << "NAME : Random-n"+to_string(cant_actual) << " "<< endl;
			test << "COMMENT : (ras et al, No of trucks: 9, Optimal Value : 999)"<<endl;
			test << "TYPE : CVRP"<<endl;
			test << "DIMENSION : "+to_string(cant_actual)<<endl;
			test << "EDGE_WEIGHT_TYPE : EUC_2D"<<endl;
			int capacidad = rand() % 5000;
			test << "CAPACITY : "+to_string(capacidad)<<endl;
			test << "NODE_COORD_SECTION"<< endl;
			for(int i = 1; i < cant_actual+1; i++)
			{
				test <<" "<< i << " "<< rand() % 1000 << " "<< rand() % 1000<< endl;
				//poner coordenadas (index, x, y)
			}
			test << "DEMAND_SECTION"<< endl;
			for(int i = 1; i < cant_actual+1; i++)
			{
				int max_actual = capacidad;
				cout << "capacidad: "<< capacidad << " cantidad de ciclos: "<< cantidad_ciclos<< endl;
				for(size_t j = 0; j < cantidad_ciclos; j++)
				{
					cout << "cota: " << capacidad/cantidad_ciclos << endl;
					cout << "j: " << j << endl;
					cout << "max_actual: " << max_actual << endl;
					test << i << " ";
					if (j == (capacidad/cantidad_ciclos)-1) {
						test << to_string(capacidad-max_actual) << endl;
					}else{
						if(max_actual <= 0){
							j = cantidad_ciclos;
							cout << "Rompe"<< endl;}
						else{
							int resto = rand() % max_actual;
							max_actual = max_actual-resto;
							test << to_string(resto) << endl;
						}
					}
					i++;
				}
				i--;
				//poner (index, demanda % capacidad) 
			}
			//Depot Section
			test << "DEPOT_SECTION"<< endl;
			test <<" "<< 1 << endl;
			test <<" "<< -1 << endl;
			test << "EOF" << endl;
			test.close();
			}
	}
	
}

void GenerarGrafos2(int cantidad_tests_por_nodo, int cant_min_N, int cant_max_N, int granularidad){
	
	for(int cant_actual = cant_min_N; cant_actual < cant_max_N; cant_actual = cant_actual + granularidad)
	{
		for(int caso_i = 0; caso_i < cantidad_tests_por_nodo; caso_i++){
			//abrir File
			ofstream test("tests_cases_tiempo/Random-n"+to_string(cant_actual)+"-caso-"+to_string(caso_i)+".txt");
			//Descripciones
			test << "NAME : Random-n"+to_string(cant_actual) << " "<< endl;
			test << "COMMENT : (ras et al, No of trucks: 9, Optimal Value : 999)"<<endl;
			test << "TYPE : CVRP"<<endl;
			test << "DIMENSION : "+to_string(cant_actual)<<endl;
			test << "EDGE_WEIGHT_TYPE : EUC_2D"<<endl;
			int capacidad = (rand() % 2000)+1;
			test << "CAPACITY : "+to_string(capacidad)<<endl;
			test << "NODE_COORD_SECTION"<< endl;
			for(int i = 1; i < cant_actual+1; i++)
			{
				test <<" "<< i << " "<< rand() % 1000 << " "<< rand() % 1000<< endl;
				//poner coordenadas (index, x, y)
			}
			test << "DEMAND_SECTION"<< endl;
			for(int i = 1; i < cant_actual+1; i++)
			{
				test << i << " ";
				int resto = rand() % capacidad;
				test << to_string(resto) << endl;
			}
			//Depot Section
			test << "DEPOT_SECTION"<< endl;
			test <<" "<< 1 << endl;
			test <<" "<< -1 << endl;
			test << "EOF" << endl;
			test.close();
			}
	}
	
}

int main(int argc, char** argv)
{
    int cantidad_tests_por_nodo = atoi(argv[1]);
    int cant_min_N = atoi(argv[2]);
    int cant_max_N = atoi(argv[3]);
	int granularidad = atoi(argv[4]);
	//int cantidad_ciclos = atoi(argv[4]);
    //GenerarGrafos(cantidad_tests_por_nodo, cant_min_N, cant_max_N, cantidad_ciclos, granularidad);
	GenerarGrafos2(cantidad_tests_por_nodo, cant_min_N, cant_max_N, granularidad);
    return 0;
}
