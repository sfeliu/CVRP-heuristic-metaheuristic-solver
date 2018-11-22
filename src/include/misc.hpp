#ifndef _MISC_HPP_
#define _MISC_HPP_

#include "grafo.hpp"

TspFile readTsp(char *filepath);
vector<string> split_line(string &line, char delim);
void save_test(Grafo g, string full_filepath, string test, vector<Camion> camiones);
void print_result(Grafo g, vector<Camion> camiones);

#endif