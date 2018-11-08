#ifndef _MISC_HPP_
#define _MISC_HPP_

#include "grafo.hpp"

/*
void imprimir_agm(listAristas l);
listAristas remover_inconsistentes(listAristas l, Grafo g, double ds, double f, double diametro, int mod);
void descubrirConexoAux(int u, listAristas res, int& contador, vector<bool> &visitado, int& i, vector<int> &comp_conex);
vector<int> descubrirConexo(listAristas l, int n);
listAristas diff(listAristas& l1, listAristas& l2);
*/
TspFile readTsp(char *filepath);
vector<string> split_line(string &line, char delim);
void save_test(Grafo g, string full_filepath, string test, vector<Camion> camiones);

#endif