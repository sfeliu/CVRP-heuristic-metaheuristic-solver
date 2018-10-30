#include "include/misc.hpp"

vector<string> split_line(string &line, char delim) {
    stringstream line_stream(line);
    vector<string> words;
    string word;
    while(getline(line_stream, word, delim)) {
        words.push_back(word);
    }
    return words;
}

TspFile readTsp(char *filepath){
    TspFile res = TspFile();
    string line;
    ifstream file(filepath);
    vector<string> words_in_line;
    bool in_node_coord_section = false;
    bool in_demand_section = false;
    bool in_depot_section = false;
    if(file.is_open()) {
        while (getline(file, line)) {
            if(in_node_coord_section){
                size_t found_demand_section = line.find("DEMAND_SECTION");
                if(found_demand_section != string::npos) {
                    in_demand_section = true;
                    in_node_coord_section = false;
                    continue;
                }
                words_in_line = split_line(line, ' ');
                Coordenadas cor = Coordenadas();
                cor.x = stod(words_in_line[2]);
                cor.y = stod(words_in_line[3]);
                res.coordenadas.push_back(cor);
                continue;
            }

            if(in_demand_section){
                size_t found_depot_section = line.find("DEPOT_SECTION");
                if(found_depot_section != string::npos) {
                    in_demand_section = false;
                    in_depot_section = true;
                    continue;
                }
                words_in_line = split_line(line, ' ');
                res.demandas.push_back(stoi(words_in_line[1]));
                continue;
            }

            if(in_depot_section){
                words_in_line = split_line(line, ' ');
                res.deposito = stoi(words_in_line[1]) - 1;
                break;
            }

            size_t found_node_coord_section = line.find("NODE_COORD_SECTION");
            if(found_node_coord_section != string::npos){
                in_node_coord_section = true;
                continue;
            }
            size_t found_dimension = line.find("DIMENSION");
            if(found_dimension != string::npos){
                words_in_line = split_line(line, ' ');
                int size = stoi(words_in_line[2]);
                continue;
            }

            size_t found_capacity = line.find("CAPACITY");
            if(found_capacity != string::npos){
                words_in_line = split_line(line, ' ');
                res.capacidad = stoi(words_in_line[2]);
                continue;
            }
        }
    }else{
        cout << "Couldn't open file " << filepath << endl;
    }
    file.close();
    return res;
}
