#include "include/grafo.hpp"


vector<string> split_line(string &line, char delim) {
    stringstream line_stream(line);
    vector<string> words;
    string word;
    while(getline(line_stream, word, delim)) {
        words.push_back(word);
    }
    return words;
}


Grafo::Grafo(TspFile tsp){
    vector<Coordenadas> puntos = tsp.coordenadas;
    unsigned long n = puntos.size();
    this->new_node(n);
    for(int i = 0; i<n ; i++){
        for(int j = 0; j<n; j++){
            if(i != j ){
                double diff_x = puntos[i].x - puntos[j].x;
                double diff_y = puntos[i].y - puntos[j].y;
                double peso = sqrt(diff_x*diff_x + diff_y*diff_y);
                this->add_edge(i,j,peso);
                /*if((i == 12 || i == 16)&&(j == 12 || j == 16)){
                    cout << "i=(" << puntos[i].x << "," << puntos[i].y << ");j=(" << puntos[j].x << "," << puntos[j].y << ") |" << "diff_x="<< diff_x << ";diff_y=" << diff_y <<";peso "<< peso << endl;
                    cout << "abs(puntos[i].x)=" << abs(puntos[i].x) << "|abs(puntos[j].x)=" << abs(puntos[j].x) <<endl;
                }*/
            }
        }
    }
    _puntos = puntos;
    _demandas = tsp.demandas;
    _capacidad = tsp.capacidad;
}

/* Nunca se usa
void Grafo::new_node(){
	vector<Node>v;
	_vertices.push_back(v);
}
*/

/* Nunca se usa
Grafo::Grafo(vector<Coordenadas> puntos) {
    unsigned long n = puntos.size();
    this->new_node(n);
    for(int i = 0; i<n ; i++){
        for(int j = 0; j<n; j++){
            if(i != j ){
                double diff_x = puntos[i].x - puntos[j].x;
                double diff_y = puntos[i].y - puntos[j].y;
                double peso = sqrt(diff_x*diff_x + diff_y*diff_y);
                this->add_edge(i,j,peso);
                if((i == 12 || i == 16)&&(j == 12 || j == 16)){
                    cout << "i=(" << puntos[i].x << "," << puntos[i].y << ");j=(" << puntos[j].x << "," << puntos[j].y << ") |" << "diff_x="<< diff_x << ";diff_y=" << diff_y <<";peso "<< peso << endl;
                    cout << "abs(puntos[i].x)=" << abs(puntos[i].x) << "|abs(puntos[j].x)=" << abs(puntos[j].x) <<endl;
                }
            }
        }
    }
    _puntos = puntos;
}
*/

/* No se usa
Grafo::Grafo(vector<vector<double>> pesos) {
    unsigned long n = pesos.size();
    this->new_node(n);
    for(int i = 0; i<n ; i++){
        for(int j = 0; j<n; j++){
            this->add_directional_edge(i,j,pesos[i][j]);
        }
    }
}
 */
/* No se usa
Grafo::Grafo(listAristas l, int cantNodos) {
    this->new_node(cantNodos);
    for(int i =0; i<l.size(); i++){
        this->add_edge(get<0>(l[i]),get<1>(l[i]),get<2>(l[i]));
    }
}
*/

void Grafo::new_node(int n){
	for(int i = 0; i < n; i++){
	    vector<double> v;
		_vertices.push_back(v);
	}
}


bool Grafo::existe(int u, int v){
    if(_vertices.size() > u && _vertices[0].size() > v) {
        return _vertices[u][v] >= 0;
    }
    return false;
}

void Grafo::borrar_edge(int u, int v){
	if(0 > u > _vertices.size()-1 || 0 > v > _vertices.size()-1){
		return;
	}
	_vertices[u][v] = -1;
	_vertices[v][u] = -1;
}

/* No se usa
void Grafo::add_directional_edge(int u, int v, double w){
	if(u > _vertices.size()-1 || v > _vertices.size()-1 || existe(u,v)){
		return;
	}
	Node nuevo_1 = Node();
	nuevo_1.id = v;
	nuevo_1.weight = w;
	(_vertices[u]).push_back(nuevo_1);
}
 */

void Grafo::add_edge(int u, int v, double w){
	if(0 > u > _vertices.size()-1 || 0 > v > _vertices.size()-1 || existe(u,v)){
		return;
	}
    _vertices[u][v] = w;
    _vertices[v][u] = w;
}

void Grafo::imprimir(){
	cout<< endl << "imprimiendo grafo..."<< endl;
	for(int i = 0; i < _vertices.size();i++){
		for(int j = 0; j< _vertices[i].size(); j++){
			cout<< "(" << i << "-" << _vertices[i][j] << "->" << j << ") ";
		}
		cout << endl;
	}
}

//******************************* HASTA ACA HICE ************************************************+
bool porPeso(tuple<int,int,double> a, tuple<int,int,double> b){
	return (get<2>(a) < get<2>(b));
}


void Grafo::init_kruskal_pc(){
	for(int i = 0; i<(_vertices).size(); i++){
		_padre.push_back(i);
		_altura.push_back(1);
	}
}

int Grafo::find_pc(int id){
	if(_padre[id] != id){
		_padre[id] = find(_padre[id]);
	}
	return _padre[id];
}

void Grafo::conjunction_pc(int u, int v){
	int x = find(u);
	int y = find(v);
	if(_altura[x] < _altura[y]){
		_padre[x] = y;
	}else{
		_padre[y] = x;
	}
	if(_altura[x] == _altura[y]){
		_altura[x] = _altura[x]+1;
	}
}

listAristas Grafo::convert(){
	listAristas aristas;
	for(int i = _vertices.size()-1; i >= 0; i--){
		for(int j = _vertices[i].size()-1; j >= 0; j--){
			_vertices[i].erase(_vertices[i].begin() + j);
			aristas.push_back(tuple<int,int,double>(i,(_vertices[i][j]).id,(_vertices[i][j]).weight));
		}
	}
	return aristas;

}

listAristas Grafo::kruskal_pc(listAristas aristas){
	init_kruskal_pc();
	listAristas agm;
	sort(aristas.begin(),aristas.end(), porPeso);
	for(int j = 0; j < aristas.size(); j++){
		if( find_pc(get<0>(aristas[j])) != find_pc(get<1>(aristas[j])) ){
			agm.push_back(aristas[j]);
			conjunction_pc(get<0>(aristas[j]),get<1>(aristas[j]));
		}
	}
	return agm;
}


void Grafo::init_kruskal(){
	for(int i = 0; i<(_vertices).size(); i++){
		_padre.push_back(i);
	}
}

int Grafo::find(int id){
	if(_padre[id] != id){
		return find(_padre[id]);
	}
	return _padre[id];
}

void Grafo::conjunction(int u, int v){
	_padre[find(u)] = _padre[find(v)];
}


listAristas Grafo::kruskal(listAristas aristas){
	init_kruskal();
	listAristas agm;
	sort(aristas.begin(),aristas.end(), porPeso);
	for(int j = 0; j < aristas.size(); j++){
		if( find(get<0>(aristas[j])) != find(get<1>(aristas[j])) ){
			agm.push_back(aristas[j]);
			conjunction(get<0>(aristas[j]),get<1>(aristas[j]));
		}
	}
	return agm;
}

listAristas Grafo::prim(){
	listAristas padre;
	vector<double> distancia;
	vector<bool> visitado;

	for(int i = 0; i < _vertices.size(); i++){
		padre.push_back(tuple<int,int,double>());
		distancia.push_back(10000000);
		visitado.push_back(false);
	}

	distancia[0] = 0;
	priority_queue <tuple<int,double>, vector<tuple<int,double>>, Comparador> pq;
	pq.push(tuple<int,double>(0, distancia[0]));
	while(!pq.empty()){
		tuple <int,double> t = pq.top();
		pq.pop();
		int t_1 = get<0>(t);
		if(!visitado[t_1]){
			visitado[t_1] = true;
			for(int j = 0; j < _vertices[t_1].size(); j++){
				int t_2 = _vertices[t_1][j].id;
				if(!visitado[t_2] && (distancia[t_2] > _vertices[t_1][j].weight)){
					distancia[t_2] = _vertices[t_1][j].weight;
					padre[t_2] = tuple<int,int,double>(t_1,t_2,distancia[t_2]);
					t = tuple<int,double>(t_2, distancia[t_2]);
					pq.push(t);
				}
			}
		}
	}
	return padre;
}

void sumaCamino(listAristas l, int u, int v, double &suma, int &pasos, int &vecinos){
	if (pasos == 0){
		return;
	} else {
		for(int i = 0; i < l.size(); i++){
			if((get<0>(l[i]) == u && get<1>(l[i]) != v)){
				suma = suma + get<2>(l[i]);
				pasos--;
				vecinos++;
				sumaCamino(l, get<1>(l[i]), get<0>(l[i]), suma, pasos, vecinos);
				pasos++;
			}
			if((get<1>(l[i]) == u && get<0>(l[i]) != v)){
				suma = suma + get<2>(l[i]);
				pasos--;
				vecinos++;
				sumaCamino(l, get<0>(l[i]), get<1>(l[i]), suma, pasos, vecinos);
				pasos++;
			}
		}
	}
}


double promedio_vecinos(listAristas vecinos){
	double suma = 0;
	// pasos = diametro;
	// sumaCamino(l,v,u,suma,pasos,vecinos);
	for(int i=0; i<vecinos.size(); i++){
	    suma += get<2>(vecinos[i]);
	}
	if(!vecinos.empty()){
		suma = suma/(vecinos.size());
	}else{
		suma = 10000000;
	}
	return suma;
}

void varianza_vecinos(listAristas l, int u, int v, double &suma, int &pasos, int &vecinos, double promedio){
	if (pasos == 0){
		return;
	} else {
		for(int i = 0; i < l.size(); i++){
			if((get<0>(l[i]) == u && get<1>(l[i]) != v)){
				suma = suma + pow(get<2>(l[i])-promedio,2);
				pasos--;
				vecinos++;
				sumaCamino(l, get<1>(l[i]), get<0>(l[i]), suma, pasos, vecinos);
				pasos++;
			}
			if((get<1>(l[i]) == u && get<0>(l[i]) != v)){
				suma = suma + pow(get<2>(l[i])-promedio,2);
				pasos--;
				vecinos++;
				sumaCamino(l, get<0>(l[i]), get<1>(l[i]), suma, pasos, vecinos);
				pasos++;
			}
		}
	}					
}

double desvio_estandard(listAristas vecinos, double promedio){
    double suma = 0;
    for(int i=0; i<vecinos.size(); i++){
        suma += suma + pow(get<2>(vecinos[i])-promedio,2);
    }
    if(!vecinos.empty()){
        suma = suma/(vecinos.size());
    }else{
        suma = 10000000;
    }
    suma = sqrt(suma);
    return suma;

}


// Basado en DFS, Sabiendo que no hay ciclos.
listAristas Grafo::obtener_vecinos(int u, int v, double cant_vecinos){
    listAristas vecindad;
    for(int i=0; i<_vertices[u].size(); i++){
        if(_vertices[u][i].id != v && cant_vecinos > 0){
            vecindad.push_back(tuple<int,int,double>(u,_vertices[u][i].id,_vertices[u][i].weight));
            cant_vecinos--;
            listAristas vecinos_de_vecinos = obtener_vecinos(_vertices[u][i].id, u, cant_vecinos);
            vecindad.insert(vecindad.end(), vecinos_de_vecinos.begin(), vecinos_de_vecinos.end());
            cant_vecinos++;
        }
    }
    return vecindad;
}

listAristas remover_inconsistentes(listAristas l, Grafo g, double ds, double f, double diametro, int mod){
	listAristas res = l;
	for(int i = 0; i < res.size(); i++){ //   O(6*E*E)
		int u = get<0>(res[i]);
		int v = get<1>(res[i]);
		if(u == v){
			res.erase(res.begin()+i);
			g.borrar_edge(u,v);
			i--;
		}else{
		    listAristas vecinos_u = g.obtener_vecinos(u,v,diametro); // O(E)
            listAristas vecinos_v = g.obtener_vecinos(v, u, diametro); // O(E)
            double promedio_u = promedio_vecinos(vecinos_u); // O(E)
            double promedio_v = promedio_vecinos(vecinos_v); // O(E)
            double peso = get<2>(res[i]);
            double desvio_u = desvio_estandard(vecinos_u,promedio_u); //O(E)
            double desvio_v = desvio_estandard(vecinos_v,promedio_v); //O(E)

            /*if(u == 8 || v == 8) {
		        cout << "Viendo caso u=" << u << " y v=" << v << " con vecindad=" << diametro <<endl;
                cout << "u -->" << endl;
                for (int x = 0; x < vecinos_u.size(); x++) {
                    cout << get<0>(vecinos_u[x]) << " --> " << get<1>(vecinos_u[x]) << " peso: "
                              << get<2>(vecinos_u[x]) << endl;
                }
                cout << "v -->" << endl;
                for (int x = 0; x < vecinos_v.size(); x++) {
                    cout << get<0>(vecinos_v[x]) << " --> " << get<1>(vecinos_v[x]) << " peso: "
                              << get<2>(vecinos_v[x]) << endl;
                }
                cout << "Promedio obtenido u: " << promedio_u << " | promedio obtenido v: " << promedio_v << endl;
                cout << "desvio obtenido u: " << desvio_u << " | desvio obtenido v: " << desvio_v << endl;
            }*/

			bool pesoMayorPromedioU = peso > f*promedio_u;
			bool pesoMayorPromedioV = peso > f*promedio_v;
			bool pesoPromedioMayorDesvioU = (peso - promedio_u) > desvio_u*ds;
			bool pesoPromedioMayorDesvioV = (peso - promedio_v) > desvio_v*ds;
			if(mod == 1){
				if(pesoMayorPromedioU && pesoMayorPromedioV){
				    res.erase(res.begin()+i);
                    cout << "f = " << f << " --> ds=" << ds << " --> vecindad=" << diametro << " --> modo=promedio" << endl;
                    cout << "Borrado eje " << u << " --> " << v << endl << endl;
                    g.borrar_edge(u,v);
					i--;
				}
			}
			if(mod == 2){
				if(pesoPromedioMayorDesvioU && pesoPromedioMayorDesvioV){
					res.erase(res.begin()+i);
					cout << "f = " << f << " --> ds=" << ds << " --> vecindad=" << diametro << " --> modo=desvio" << endl;
					cout << "Borrado eje " << u << " --> " << v << endl << endl;
                    g.borrar_edge(u,v);
					i--;
				}
			}
			if(mod == 3){
				if((pesoMayorPromedioU && pesoMayorPromedioV) || (pesoPromedioMayorDesvioU && pesoPromedioMayorDesvioV)){
					res.erase(res.begin()+i);
                    g.borrar_edge(u,v);
					i--;
				}
			}
			if(mod == 4){
				if(pesoMayorPromedioU && pesoMayorPromedioV && pesoPromedioMayorDesvioU && pesoPromedioMayorDesvioV){
					res.erase(res.begin()+i);
                    g.borrar_edge(u,v);
					i--;
				}
			}			
		}
	}
	return res;
}


void imprimir_agm(listAristas l){

	cout<< endl << "imprimiendo arbol generador minimo..."<< endl;
	for(int j = 0; j< l.size(); j++){
		cout<< "(" << get<0>(l[j]) << "-->" << get<1>(l[j]) << " ;peso: " << get<2>(l[j]) << ") " << endl;
	}
	cout << endl;
}

void Grafo::imprimir_pos(){
	cout<< "imprimiendo cordenadas..."<<endl;
	cout<< "{ ";
	for(int i = 0; i< puntos().size(); i++){
		cout<<"("<< puntos()[i].x << ", "<< puntos()[i].y << ") ";
	}
	cout << "}" <<endl;
}

vector<Coordenadas>& Grafo::puntos(){
	return _puntos;
}

double& Grafo::peso(int u, int v){
	return _vertices[u][v].weight;
}

void Grafo::logPesos(){
	int n = _vertices.size();
	for (int u = 0; u < n; u++){
		for (int v = 0; v < n; v++){
			peso(u,v) = - log10(peso(u,v));
		}
	}
}

void Grafo::cicloNegativoFW(){
    auto start = chrono::steady_clock::now();
	int n = _vertices.size();
	vector <int> filaSiguiente(n,-1);
	vector <vector<int> > siguiente (n,filaSiguiente);

	int hayCicloNegativo = floydWarshall(siguiente);

	if (hayCicloNegativo != -1){
		cout << "SI ";
		vector<int> recorrido;
		recorrido.push_back(hayCicloNegativo);
		int v = hayCicloNegativo;
		int u = siguiente[v][v];
			while (v != u){
				recorrido.push_back(u);
				u = siguiente[u][v];
			}
			recorrido.push_back(hayCicloNegativo);
			for (int i = 0; i < recorrido.size(); i++){
				cout << recorrido[i] << " ";
			}
	} else {
		cout << "NO";
	}
    auto end = chrono::steady_clock::now();
    auto diff = end - start;
    cerr << chrono::duration<double, milli>(diff).count();

}

int Grafo::floydWarshall(vector< vector<int> > &siguiente){
    int n = _vertices.size();
	vector <double> filaDistancias(n,INF);

	vector <vector<double> > distancias (n,filaDistancias);

	for (int u = 0; u < n; u++){
		for(int v = 0; v < n; v++){
			distancias[u][v] = peso(u,v);
			siguiente[u][v] = v;
		}
	}

	for (int k = 0; k < n; k++){
		for (int i = 0; i < n; i++){
			for (int j = 0; j < n; j++){
				if (distancias[i][j] > distancias[i][k] + distancias[k][j]){
					distancias[i][j] = distancias[i][k] + distancias[k][j];
					siguiente[i][j] = siguiente[i][k];
				}
			}
		}
	}

    for (int i = 0; i < n; i++) 
        if (distancias[i][i] < 0) 
            return i; 
    return -1;  
}

void Grafo::cicloNegativoBF(){
    int n = _vertices.size();
    vector <int> pred(n,-1);
    vector <double> distancias(n,INF);
    vector <double> copia;
    distancias[0] = 0;

	int i = 0;
	bool cambio = true;
	while(i < n && cambio){	
		copia = distancias;
		cambio = false;
		for (int u = 0; u < n; ++u){
			for (int v = 0; v < n; ++v){
				if(copia[u] != INF) {
                    if ( copia[v] == INF || copia[v] > copia[u] + peso(u, v)){
                        distancias[v] = copia[u] + peso(u, v);
                        pred[v] = u;
                        cambio = true;
                    }
                }
			}
		}
		i++;
	}

    bool hayCiclo = false;

    for (int v = 0; v < n; v++) {
        if (copia[v] != distancias[v]){
            hayCiclo = true;
            cout << "SI ";
            int h = pred[v];
            vector<int> recorrido;
            recorrido.push_back(v);

            while (h != v) {
                recorrido.push_back(h);
                h = pred[h];
            }
            for (int j = recorrido.size()-1; j >= 0; j--) {
                cout << recorrido[j] << " ";
            }
            cout << recorrido[recorrido.size()-1] << endl;
            break;
        }
    }
    if(!hayCiclo){
        cout<< "NO";
    }
}

void descubrirConexoAux(int u, listAristas res, int& contador, vector<bool> &visitado, int& i, vector<int> &comp_conex){
	comp_conex[u] = contador;
	comp_conex[get<1>(res[i])] = contador;
	visitado[u] = true;
	for(int j = 0; j<res.size(); j++){
		if(i != j ){
			if(get<0>(res[j]) == u){
				descubrirConexoAux(get<1>(res[j]), res, contador, visitado, j, comp_conex);
			}
			if( get<1>(res[j]) == u){
				descubrirConexoAux(get<0>(res[j]), res, contador, visitado, j, comp_conex);
			}
		}else{
			if(!visitado[get<1>(res[j])]){
				descubrirConexoAux(get<1>(res[j]), res, contador, visitado, j, comp_conex);
			}
		}
	}
}


vector<int> descubrirConexo(listAristas l, int n){
	vector<int> comp_conex;
	int contador = 0;
	vector<bool> visitado;
	for(int i = 0; i < n; i++){
		comp_conex.push_back(0);
		visitado.push_back(false);	
	}
	for(int i = 0; i < l.size(); i++){
		if(!visitado[get<0>(l[i])]){
			descubrirConexoAux(get<0>(l[i]), l, contador, visitado,i,comp_conex);
			contador++;
		}
	}
	return comp_conex;
}

listAristas diff(listAristas& l1, listAristas& l2){
	listAristas res;
	for(int i = 0; i < l1.size(); i++){
		bool pertenece = false;
		for(int j = 0; j< l2.size(); j++){
			bool igualesInv = (get<0>(l1[i]) == get<1>(l2[j])) && (get<1>(l1[i]) == get<0>(l2[j]));
			bool iguales = (get<0>(l1[i]) == get<0>(l2[j])) && (get<1>(l1[i]) == get<1>(l2[j]));
			if(iguales || (igualesInv)){
				pertenece = true;
				res.push_back(l1[i]);
				l2.erase(l2.begin()+j);
				l1.erase(l1.begin()+i);
				i--;
				break;
			}
		}
	}
	return res;
}
