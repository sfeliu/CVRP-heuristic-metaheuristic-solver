#include "include/grafo.hpp"


Grafo::Grafo(TspFile tsp, string mode){
    vector<Coordenadas> puntos = tsp.coordenadas;
    unsigned long n = puntos.size();
    this->new_node(n);
    _puntos = puntos;
    _demandas = tsp.demandas;
    _capacidad = tsp.capacidad;
    _deposito = tsp.deposito;
    if(mode == "kn"){
        crearKn();
    }
    else if(mode == "star"){
        crearStar(_deposito);
    }
}

double Grafo::diffEuclidea(int u, int v){
    double diff_x = _puntos[u].x - _puntos[v].x;
    double diff_y = _puntos[u].y - _puntos[v].y;
    double peso = sqrt(diff_x*diff_x + diff_y*diff_y);
    return peso;
}

void print_vec(vector<int> v){
	cout<< "[ ";
	for(int i = 0 ; i < v.size()-1; i++){
		cout << v[i] << ", ";
	}
	cout<< v[v.size()-1] << " ]" << endl;
}


void Grafo::crearKn() {
    unsigned long n = _puntos.size();
    for(int i = 0; i<n ; i++){
        for(int j = 0; j<n; j++){
            if(i != j ){
                double peso = diffEuclidea(i, j);
                this->add_edge(i,j,peso);
                /*if((i == 12 || i == 16)&&(j == 12 || j == 16)){
                    cout << "i=(" << puntos[i].x << "," << puntos[i].y << ");j=(" << puntos[j].x << "," << puntos[j].y << ") |" << "diff_x="<< diff_x << ";diff_y=" << diff_y <<";peso "<< peso << endl;
                    cout << "abs(puntos[i].x)=" << abs(puntos[i].x) << "|abs(puntos[j].x)=" << abs(puntos[j].x) <<endl;
                }*/
            }
        }
    }
}

void Grafo::crearStar(int center){
    unsigned long n = _puntos.size();
    for(int i = 0; i<n; i++){
        if(center != i){
            double peso = diffEuclidea(center, i);
            this->add_edge(center, i, peso);
        }
    }
}

int Grafo::tomarMinDist(int nodo, list<int>& l){
	if (l.empty()) {
		cout << "tomarMinDist rompe en vacio"<< endl;
	}
	list<int>::iterator it = l.begin();
	int nodo_min_dist = *it;
  int min = peso(*it,nodo);
	while(it != l.end()){
    double p = peso(*it,nodo);
		//cout <<"peso: "<< p<< " iterador: "<< *it<< endl;
		if (p < min) {
			min = p;
			nodo_min_dist = *it;
		}
		it++;
	}
	return nodo_min_dist;
}

vector< vector<int> > Grafo::GolosoMasCercano(){
	cout << "Inicio GolosoMasCercano"<< endl;
	int cant_dep_a_visitar = _puntos.size();
	std::list<int> ids;
	for(int i = 2; i < cant_dep_a_visitar; i++)
	{
		ids.push_back(i);
	}
  ids.remove(_deposito);
	int m;
	vector< vector<int> > ciclos;
  //cout << cant_dep_a_visitar << endl;
  int j = 0;
	while(!ids.empty()){
		j++;
		vector<int> ciclo = {_deposito};

		m = tomarMinDist(_deposito,ids);
		cout <<"vecino mas cercano "<< j << " es: "<< m<<" con peso: " << peso(m,_deposito)<<endl;

		int distancia = peso(_deposito,m);

		int demanda = 0;
    int k = 0;
		while(_demandas[m] + demanda  <= _capacidad && !ids.empty()){
			cout << "cicloInterno : " << k << endl;
      k++;
			ciclo.push_back(m);
			ids.remove(m);
			demanda = demanda + _demandas[m];
			cout <<"demanda: "<< demanda-_demandas[m]<< endl;
      if(!ids.empty()){
        m = tomarMinDist(m,ids);
      }
			cout <<"viejo: "<< m<< endl;
			cout <<"nueva_demanda: "<< demanda<< endl;
			cout << endl;
		}
		ciclo.push_back(_deposito);
		ciclos.push_back(ciclo);
	}
	cout << ciclos.size()<< endl;
	int costo_de_rutas = 0;
	for(int i = 0; i < ciclos.size(); i++)
	{
		int costo_ruta = 0;
		for(int j = 0; j < ciclos[i].size()-1; j++)
		{
			costo_ruta += peso(ciclos[i][j+1],ciclos[i][j]);
			cout << ciclos[i][j]+1 << " ";
		}
		cout << ciclos[0][0]+1 << " ";
		costo_de_rutas += costo_ruta;
		cout << endl;
	}
	cout << costo_de_rutas << endl;
	return ciclos;
}

/* Nunca se usa
void Grafo::new_node(){
	vector<Node>v;
	_vertices.push_back(v);
}
*/

bool porPeso(tuple<int,double> a, tuple<int,double> b){
	return (get<1>(a) < get<1>(b));
}


vector< tuple<int,double> > Grafo::getAngulos(){ //complejidad O(V)
	vector< tuple<int,double> > angulos(_vertices.size());
	// cout<< "ID: "<< _deposito << " | "<< "x: "<< _puntos[_deposito].x << " | "<< "y: " << _puntos[_deposito].y << " | peso: "<< diffEuclidea(_deposito,_deposito)<< endl;
	for(int i = 0; i < _vertices.size(); i++){
		if(i == _deposito){i++;}
		double y1 = _puntos[i].y;
		double y2 = _puntos[_deposito].y;
		double x1 = _puntos[i].x;
		double x2 = _puntos[_deposito].x;
		double peso = diffEuclidea(_deposito,i);
		double arcsin = asin((y1 - y2)/peso);
		double arccos = acos((x1 - x2)/peso);
		if(y1-y2 < 0){
			angulos[i] = tuple<int,double>(i,360.0-(arccos * 180.0/3.14159265));	
		}else{
			angulos[i] = tuple<int,double>(i,(arccos * 180.0)/3.14159265);	
		}		
	}
    angulos.erase(angulos.begin() + _deposito);
	return angulos;
}

void Grafo::sweep(vector< vector <int> >& clusters){ //complejidad O(V*log(V))
	vector< tuple<int,double> > angulos = getAngulos();
	sort(angulos.begin(), angulos.end(), porPeso);
	for(int i = 0; i < angulos.size(); i++){
		// cout << "ID: " << get<0>(angulos[i]) << " | Degree: " << get<1>(angulos[i]) << endl;
	}
	int contenido = 0;
	int clusterNum = 0;
	int i = 0;
	while(i < angulos.size()){
		contenido = 0;
		vector<int> v;
		clusters.push_back(v);
		while(contenido + _demandas[get<0>(angulos[i])] < _capacidad && i<angulos.size()){
			clusters[clusterNum].push_back(get<0>(angulos[i]));
			contenido = contenido + _demandas[get<0>(angulos[i])];
			i++;
		}
		clusterNum++;
	}
	return;
}


void Grafo::DFS( vector<int>& inorderWalk, int& actual, int padre) { // requiere que g sea digrafo o tengo que sacar las aristas de vuelta en un grafo
	inorderWalk.push_back(actual);
	int temp_actual = actual;
	int temp_padre = padre;
	for(int j = 0; j < _vertices[actual].size(); j++){
		if(_vertices[actual][j].id != padre){
			padre = actual;
			DFS(inorderWalk,_vertices[actual][j].id,padre);	
			actual = temp_actual;
			padre = temp_padre;
		}
	}
}



vector<int> Grafo::TwoOptswap(vector<int> ruta, int i, int k){
	vector<int> ruta_nueva(ruta.begin(), ruta.begin() + i);
	// print_vec(ruta);
	// print_vec(ruta_nueva);
	vector<int> final(ruta.begin()+k,ruta.end());
	for(int j = k-1; j >= i; j--){
		ruta_nueva.push_back(ruta[j]);
	}
	ruta_nueva.insert(ruta_nueva.end(), final.begin(),final.end());
	// print_vec(ruta_nueva);
	// cout<<endl;
	return ruta_nueva;
}

double Grafo::calcularDistancia(vector<int> ruta){
	double suma = 0;
	for(int i = 0; i < ruta.size()-1; i++){
		for(int k = 0; k < _vertices[ruta[i]].size(); k++){
			suma = suma + diffEuclidea(ruta[i],ruta[i+1]);
			// if(_vertices[ruta[i]][k].id == ruta[i+1] ){
			// 	suma = suma + _vertices[ruta[i]][k].weight;		
			// }
		}
	}
	// cout<< suma<<endl;
	return suma;
}

vector<int> Grafo::TwoOptCompleto(vector<int> ruta){
	double menor_distancia = calcularDistancia(ruta);
	double distancia_nueva;
	vector<int> mejor_ruta(ruta);
	// print_vec(mejor_ruta);
	for(int i = 1; i < ((int)(ruta.size())) -2; i++){
		for(int j = i+2; j < ((int)ruta.size()); j++){
			vector<int> nueva_ruta = TwoOptswap(mejor_ruta, i, j);
			// print_vec(nueva_ruta);
			distancia_nueva = calcularDistancia(nueva_ruta);
			if(distancia_nueva < menor_distancia){
				mejor_ruta = nueva_ruta;
				menor_distancia = distancia_nueva;
				// TwoOptCompleto(mejor_ruta);
				// break;
			}
		}
	}
	// print_vec(mejor_ruta);
	// cout<<endl;
	return mejor_ruta;
}

vector< vector<int> > Grafo::routear_conTwoOpt( vector< vector<int> > clusters) {
	int cantClusters = clusters.size();
	vector< vector<int> > rutas;
	for(int i = 0; i < cantClusters; i++){
		vector<Coordenadas> coordenadas_cluster;
		// clusters[i].push_back(_deposito);
		for(int j = 0; j < clusters[i].size(); j++){
			coordenadas_cluster.push_back(_puntos[clusters[i][j]]);
		}
		Grafo g1(coordenadas_cluster);
		vector<int> l = g1.solveTSP();
		vector<int> copy = clusters[i];
		for(int k = 0; k < l.size(); k++){
			clusters[i][k] = copy[l[k]];
		}
		double costo_anterior = calcularDistancia(clusters[i]);
		clusters[i].push_back(_deposito);
		clusters[i].insert(clusters[i].begin(),_deposito);
		// print_vec(clusters[i]);
		clusters[i] = TwoOptCompleto(clusters[i]);
		clusters[i].erase(clusters[i].begin());
		clusters[i].erase(clusters[i].begin()+clusters[i].size()-1);
		double costo_posterior = calcularDistancia(clusters[i]);
		// print_vec(clusters[i]);
	}
	return clusters;
}

vector< vector<int> > Grafo::routear_conTwoOpt2( vector< vector<int> > clusters) {
	int cantClusters = clusters.size();
	vector< vector<int> > rutas;
	for(int i = 0; i < cantClusters; i++){
		vector<Coordenadas> coordenadas_cluster;
		// clusters[i].push_back(_deposito);
		for(int j = 0; j < clusters[i].size(); j++){
			coordenadas_cluster.push_back(_puntos[clusters[i][j]]);
		}
		Grafo g1(coordenadas_cluster);
		vector<int> l = g1.solveTSP();
		vector<int> copy = clusters[i];
		for(int k = 0; k < l.size(); k++){
			clusters[i][k] = copy[l[k]];
		}
		clusters[i].push_back(_deposito);
		clusters[i].insert(clusters[i].begin(),_deposito);
		double costo_anterior = calcularDistancia(clusters[i]);
		// print_vec(clusters[i]);
		clusters[i] = TwoOptCompleto(clusters[i]);
		double costo_posterior = calcularDistancia(clusters[i]);

		clusters[i].erase(clusters[i].begin());
		clusters[i].erase(clusters[i].begin()+clusters[i].size()-1);
		vector<int> temp = clusters[i];
		while(costo_posterior < costo_anterior){
			clusters[i] = temp;
			temp.push_back(_deposito);
			temp.insert(temp.begin(),_deposito);
			costo_anterior = calcularDistancia(temp);
			// print_vec(clusters[i]);
			temp = TwoOptCompleto(temp);
			costo_posterior = calcularDistancia(temp);

			temp.erase(temp.begin());
			temp.erase(temp.begin()+temp.size()-1);
		}
		// print_vec(clusters[i]);
	}
	return clusters;
}

vector< vector<int> > Grafo::solveVSP_conTwoOpt(){
	vector< vector<int> > camiones;
	sweep(camiones);
	return routear_conTwoOpt(camiones);
}

vector< vector<int> > Grafo::solveVSP_conTwoOpt2(){
	vector< vector<int> > camiones;
	sweep(camiones);
	return routear_conTwoOpt2(camiones);
}


vector<int> Grafo::solveTSP() {
	listAristas agm = prim();
	Grafo g(agm, _vertices.size());
	g.borrar_edge(0,0);
	vector<int> inorderWalk;
	int nodo = 0;
	g.DFS(inorderWalk, nodo, nodo);
	return inorderWalk;
}

vector< vector<int> > Grafo::solveVSP(){
	vector< vector<int> > v;
	sweep(v);
	return routear(v);
}

vector< vector<int> > Grafo::routear( vector< vector<int> > clusters) {
	int cantClusters = clusters.size();
	vector< vector<int> > rutas;
	for(int i = 0; i < cantClusters; i++){
		vector<Coordenadas> coordenadas_cluster;
		// clusters[i].push_back(_deposito);
		for(int j = 0; j < clusters[i].size(); j++){
			coordenadas_cluster.push_back(_puntos[clusters[i][j]]);
		}
		Grafo g1(coordenadas_cluster);
		vector<int> l = g1.solveTSP();
		vector<int> copy = clusters[i];
		for(int k = 0; k < l.size(); k++){
			clusters[i][k] = copy[l[k]];
		}
	}
	return clusters;
}

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
                // if((i == 12 || i == 16)&&(j == 12 || j == 16)){
                //     cout << "i=(" << puntos[i].x << "," << puntos[i].y << ");j=(" << puntos[j].x << "," << puntos[j].y << ") |" << "diff_x="<< diff_x << ";diff_y=" << diff_y <<";peso "<< peso << endl;
                //     cout << "abs(puntos[i].x)=" << abs(puntos[i].x) << "|abs(puntos[j].x)=" << abs(puntos[j].x) <<endl;
                // }
            }
        }
    }
    _puntos = puntos;
}


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
Grafo::Grafo(listAristas l, int cantNodos) {
    this->new_node(cantNodos);
    for(int i =0; i<l.size(); i++){
        this->add_edge(get<0>(l[i]),get<1>(l[i]),get<2>(l[i]));
    }
}


void Grafo::new_node(int n){
	for(int i = 0; i < n; i++){
	    vector<Node> v;
		_vertices.push_back(v);
	}
}


bool Grafo::existe(int u, int v){
    for(int i=0; i <  _vertices[u].size(); i++){
        if(_vertices[u][i].id == v){
            return true;
        }
    }
    return false;
}

void Grafo::borrar_edge(int u, int v){
	if(0 > u > _vertices.size()-1 || 0 > v > _vertices.size()-1){
		return;
	}
    for(int i=0; i<_vertices[u].size(); i++) {
        if (_vertices[u][i].id == v) {
            _vertices[u].erase(_vertices[u].begin() + i);
        }
    }
    for(int i=0; i<_vertices[v].size(); i++) {
        if (_vertices[v][i].id == u) {
            _vertices[v].erase(_vertices[v].begin() + i);
        }
    }
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
    Node nuevo_1 = Node();
    nuevo_1.id = v;
    nuevo_1.weight = w;
    (_vertices[u]).push_back(nuevo_1);

    Node nuevo_2 = Node();
    nuevo_2.id = u;
    nuevo_2.weight = w;
    (_vertices[v]).push_back(nuevo_2);
}

void Grafo::imprimir(){
	cout<< endl << "imprimiendo grafo..."<< endl;
	for(int i = 0; i < _vertices.size();i++){
		for(int j = 0; j< _vertices[i].size(); j++){
			cout<< "(" << i << "-" << _vertices[i][j].weight << "->" <<  _vertices[i][j].id << ") ";
		}
		cout << endl;
	}
}


bool porPeso_list_aristas(tuple<int,int,double> a, tuple<int,int,double> b){
	return (get<2>(a) < get<2>(b));
}


bool porPeso_savings(Saving a, Saving b){
	return (a.ahorro > b.ahorro);
}


void Grafo::init_kruskal_pc(){
	for(int i = 0; i<(_vertices).size(); i++){
		_padre.push_back(i);
		_altura.push_back(1);
	}
}

/*
int Grafo::find_pc(int id){
	if(_padre[id] != id){
		_padre[id] = find(_padre[id]);
	}
	return _padre[id];
}
*/

/*
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
*/

/*
listAristas Grafo::convert(){
	listAristas aristas;
	for(int i = _vertices.size()-1; i >= 0; i--){
		for(int j = _vertices[i].size()-1; j >= 0; j--){
			_vertices[i].erase(_vertices[i].begin() + j);
			aristas.push_back(tuple<int,int,double>(i,(_vertices[i][j]).id,(_vertices[i][j]).weight));
		}
	}
	return aristas;

}*/

/*
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
*/

/*
void Grafo::init_kruskal(){
	for(int i = 0; i<(_vertices).size(); i++){
		_padre.push_back(i);
	}
}
*/

/*
int Grafo::find(int id){
	if(_padre[id] != id){
		return find(_padre[id]);
	}
	return _padre[id];
}
*/

/*
void Grafo::conjunction(int u, int v){
	_padre[find(u)] = _padre[find(v)];
}
 */

/*
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
*/


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


/*
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
*/

/*
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
*/

/*
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
*/

/*
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
*/

/*
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
*/

/*
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

            *//*if(u == 8 || v == 8) {
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
            }*//*

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
*/


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
    for(int i=0; i<_vertices[u].size(); i++) {
        if (_vertices[u][i].id == v) {
            return _vertices[u][i].weight;
        }
    }
}

/*
void Grafo::logPesos(){
	int n = _vertices.size();
	for (int u = 0; u < n; u++){
		for (int v = 0; v < n; v++){
			peso(u,v) = - log10(peso(u,v));
		}
	}
}
*/

/*
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
*/

/*
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
*/

/*
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
*/


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


vector<vector<int> > Grafo::clusterizeRadial(){
	vector<vector<int> > todosClusters;
	int n = _vertices.size();

	vector<tuple<int,double> > clientes;
	for (int i = 0; i < n; i++){
		if (i == _deposito) continue;
		clientes.push_back(tuple<int,double> (i,peso(i,_deposito)));
	}

  sort(clientes.begin(),clientes.end(),
       [](const tuple<int,double>& a,
       const tuple<int,double>& b) -> bool
       {
         return std::get<1>(a) > std::get<1>(b);
       });
	// sort(clientes.begin(),clientes.end(),porPeso);

	vector<int> cluster;
	int capacidadCamion = _capacidad;
	for (auto c : clientes){
		int demandaC = _demandas[get<0>(c)];
		if (capacidadCamion >= demandaC){
			capacidadCamion -= demandaC;
			cluster.push_back(get<0>(c));
		} else {
			todosClusters.push_back(cluster);
			cluster.clear();
			capacidadCamion = _capacidad - demandaC;
			cluster.push_back(get<0>(c));
		}
	}
	todosClusters.push_back(cluster);
	return todosClusters;
}

vector<vector<int> > Grafo::NNRadial(){
	vector<vector<int> > clusters;
	clusters = clusterizeRadial();
	int n = clusters.size();
	vector<vector<int> > result;
	for (int i = 0; i < n; i++) {
		result.push_back(NearestNeighbourTSP(clusters[i]));
	}
	return result;
}

vector<int> Grafo::NearestNeighbourTSP(vector<int> cluster)
{
	int n = cluster.size();
	// std::vector<int> auxUsados;
	std::vector<int> resultado;

	int clienteActual = cluster[0];
	resultado.push_back(clienteActual);
	
    cluster.erase(std::remove(cluster.begin(), cluster.end(), clienteActual), cluster.end());

	for (int i = 1; i < n; i++){
		auto min = std::min_element(begin(cluster), end(cluster),
									[&](const int &a, const int &b) {
										return peso(a,clienteActual) < peso(b,clienteActual);
									});
		
		resultado.push_back(*min);
		clienteActual = *min;
	    cluster.erase(std::remove(cluster.begin(), cluster.end(), *min), cluster.end());
	}
	return resultado;
}

int get_oposite_vert(Camion camion, int vertice){
    if(camion.circuito[0] == vertice){
        return camion.circuito[camion.circuito.size() - 1];
    }else{
        return camion.circuito[0];
    }

}

bool utiliza_vertice(Saving saving, int vertice){
    int vertice_a = get<0>(saving.vertices_unidos);
    int vertice_b = get<1>(saving.vertices_unidos);
    return vertice_a == vertice || vertice_b == vertice;
}

bool utiliza_vertice(Camion camion, int vertice){
    int vertice_a = camion.circuito[0];
    int vertice_b = camion.circuito[camion.circuito.size() - 1];
    return vertice_a == vertice || vertice_b == vertice;
}

bool utiliza_camion(Saving saving, Camion camion){
    int indice_camion = camion.id;
    int indice_a = get<0>(saving.camiones);
    int indice_b = get<1>(saving.camiones);
    return indice_camion == indice_a || indice_camion == indice_b;
}

void unir_circuito(Camion &camion1, Camion &camion2, tuple<int,int> unir_vertices){
    if(get<0>(unir_vertices) == camion1.circuito[0] ||
       get<1>(unir_vertices) == camion1.circuito[0]){
        reverse(camion1.circuito.begin(),camion1.circuito.end());
    }
    if(get<0>(unir_vertices) == camion2.circuito[camion2.circuito.size() - 1] ||
       get<1>(unir_vertices) == camion2.circuito[camion2.circuito.size() - 1]) {
        reverse(camion2.circuito.begin(), camion2.circuito.end());
    }
    camion1.circuito.insert(camion1.circuito.end(),camion2.circuito.begin(),camion2.circuito.end());
}

vector<Saving> Grafo::merge_and_update_savings(vector<Saving> savings, vector<Camion> &camiones){
    Saving best_saving = savings[0];
    tuple<int,int> id_camiones_merge = best_saving.camiones;
    vector<Camion> camiones_merge;
    for(int i=0; i<camiones.size(); i++){
        int id_temp = camiones[i].id;
        if(id_temp == get<0>(id_camiones_merge) || id_temp == get<1>(id_camiones_merge)){
            camiones_merge.push_back(camiones[i]);
            camiones.erase(camiones.begin()+i);
            i--;
        }
    }
    Camion camion_unido;
    camion_unido = camiones_merge[0];
    Camion camion_temp = camiones_merge[1];
    int best_vertice_a = get<0>(best_saving.vertices_unidos);
    int best_vertice_b = get<1>(best_saving.vertices_unidos);
    savings.erase(savings.begin());
    for(int i=0; i<savings.size(); i++){
        int camion_a = get<0>(savings[i].camiones);
        int camion_b = get<1>(savings[i].camiones);

        if(utiliza_camion(savings[i], camion_temp)){
            if(utiliza_camion(savings[i], camion_unido)){
                savings.erase(savings.begin() + i);
                i--;
                continue;
            }
        }
        if(utiliza_vertice(savings[i], best_vertice_a) || utiliza_vertice(savings[i], best_vertice_b)){
            int vertice_saving = utiliza_vertice(savings[i], best_vertice_a)? best_vertice_a : best_vertice_b;
            if(utiliza_vertice(camion_temp, vertice_saving)){
                if(camion_temp.circuito.size() > 1) {
                    savings.erase(savings.begin() + i);
                    i--;
                    continue;
                }else{
                    if(savings[i].merch + camion_unido.merch > _capacidad){
                        savings.erase(savings.begin() + i);
                        i--;
                        continue;
                    }
                    savings[i].merch += camion_unido.merch;
                }
            }else{
                if(camion_unido.circuito.size() > 1){
                    savings.erase(savings.begin() + i);
                    i--;
                    continue;
                }else {
                    if (savings[i].merch + camion_temp.merch > _capacidad) {
                        savings.erase(savings.begin() + i);
                        i--;
                        continue;
                    }
                    savings[i].merch += camion_temp.merch;
                }
            }
        }else{
            int oposite_vert_a = utiliza_vertice(camion_unido, best_vertice_a) ? get_oposite_vert(camion_unido, best_vertice_a) : get_oposite_vert(camion_unido, best_vertice_b);
            int oposite_vert_b = utiliza_vertice(camion_temp, best_vertice_a) ? get_oposite_vert(camion_temp, best_vertice_a) : get_oposite_vert(camion_temp, best_vertice_b);

            if(utiliza_vertice(savings[i], oposite_vert_a) || utiliza_vertice(savings[i], oposite_vert_b)){
                int oposite_vertice = utiliza_vertice(savings[i], oposite_vert_a) ? oposite_vert_a : oposite_vert_b;

                if(utiliza_vertice(camion_temp, oposite_vertice)) {
                    if (savings[i].merch + camion_unido.merch > _capacidad) {
                        savings.erase(savings.begin() + i);
                        i--;
                        continue;
                    }
                    savings[i].merch += camion_unido.merch;
                }else {
                    if (savings[i].merch + camion_temp.merch > _capacidad) {
                        savings.erase(savings.begin() + i);
                        i--;
                        continue;
                    }
                    savings[i].merch += camion_temp.merch;
                }

            }
        }
        if(utiliza_camion(savings[i], camion_temp)) {
            if (camion_temp.id == camion_a) {
                savings[i].camiones = make_tuple(camion_unido.id, camion_b);
            }
            if (camion_temp.id == camion_b) {
                savings[i].camiones = make_tuple(camion_a, camion_unido.id);
            }
        }
    }

    camion_unido.merch += camion_temp.merch;
    camion_unido.distancia += camion_temp.distancia - best_saving.ahorro;
    unir_circuito(camion_unido, camion_temp, best_saving.vertices_unidos);
    camiones.push_back(camion_unido);
    return savings;

}

vector<Saving> Grafo::calcular_savings(vector<Camion> camiones){
    vector<Saving> savings;
    int contador = 0;
    for(unsigned long i=0; i<camiones.size(); i++) {
        for (unsigned long j = i+1; j < camiones.size(); j++) {
            if (i != j) {

                Saving saving;
                double ahorro = peso(camiones[i].circuito[0], _deposito) + peso(camiones[j].circuito[0], _deposito) -
                                peso(camiones[i].circuito[0], camiones[j].circuito[0]);
                int merch = camiones[i].merch + camiones[j].merch;
                if (ahorro > 0 && merch <= _capacidad) {
                    saving.ahorro = ahorro;
                    saving.camiones = make_tuple(camiones[i].id, camiones[j].id);
                    saving.vertices_unidos = make_tuple(camiones[i].circuito[0], camiones[j].circuito[0]);
                    saving.merch = merch;
                    savings.push_back(saving);
                }
                contador++;
            }
        }
    }
    cout << contador << endl;
    return savings;
}


vector<Camion> Grafo::heuristica_savings(){
    vector<Camion> camiones;
    for(int u = 0; u<_vertices.size(); u++){
        if(_deposito != u) {
            Camion camion = Camion();
            camion.id = u;
            vector<int> circuito;
            circuito.push_back(u);
            int merch = _demandas[u];
            double distancia = peso(_deposito, u)*2;
            camion.circuito = circuito;
            camion.merch = merch;
            camion.distancia = distancia;
            camiones.push_back(camion);
        }
    }
    vector<Saving> savings = calcular_savings(camiones);
    // De mayor a menor
    sort(savings.begin(),savings.end(), porPeso_savings);
    vector<Saving> updated_savings = merge_and_update_savings(savings, camiones);
    while(savings.size() != updated_savings.size()){
        savings = updated_savings;
        updated_savings = merge_and_update_savings(savings, camiones);
        if(updated_savings.empty()){
            break;
        }
    }
    return camiones;
}

int Grafo::deposito() {
    return _deposito;
}

vector<Camion> Grafo::generateCamiones(vector<vector<int>> circuitos) {
    vector<Camion> camiones;
    for(int i=0; i<circuitos.size(); i++){
        Camion camion = Camion();
        camion.distancia = peso(_deposito, circuitos[i][0]);
        camion.merch = 0;
        camion.id = i;
        for(int j=0; j<circuitos[i].size() - 1; j++){
            camion.circuito.push_back(circuitos[i][j]);
            camion.distancia += peso(circuitos[i][j], circuitos[i][j+1]);
            camion.merch += _demandas[circuitos[i][j]];
        }
        camion.circuito.push_back(circuitos[i][circuitos[i].size()-1]);
        camion.distancia += peso(circuitos[i][circuitos[i].size()-1], _deposito);
        camion.merch += _demandas[circuitos[i][circuitos[i].size()-1]];
        camiones.push_back(camion);
    }
    return camiones;
}
