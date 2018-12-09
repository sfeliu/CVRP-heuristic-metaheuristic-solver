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
	list<int>::iterator it = l.begin();
	int nodo_min_dist = *it;
  int min = peso(*it,nodo);
	while(it != l.end()){
    double p = peso(*it,nodo);
		if (p < min) {
			min = p;
			nodo_min_dist = *it;
		}
		it++;
	}
	return nodo_min_dist;
}

vector< vector<int> > Grafo::GolosoMasCercano(){
	int cant_dep_a_visitar = _puntos.size();
	std::list<int> ids;
	for(int i = 0; i < cant_dep_a_visitar; i++)
	{
		ids.push_back(i);
	}
  ids.remove(_deposito);
	int m;
	vector< vector<int> > ciclos;
	while(!ids.empty()){
		vector<int> ciclo = {};
		m = tomarMinDist(_deposito,ids);
		int distancia = peso(_deposito,m);
		int demanda = 0;
		while(_demandas[m] + demanda  <= _capacidad && !ids.empty()){
			ciclo.push_back(m);
			ids.remove(m);
			demanda = demanda + _demandas[m];
      		if(!ids.empty()){
        		m = tomarMinDist(m,ids);
      		}
		}
		ciclos.push_back(ciclo);
	}
	return ciclos;
}


bool porPeso(tuple<int,double> a, tuple<int,double> b){
	return (get<1>(a) < get<1>(b));
}


vector< tuple<int,double> > Grafo::getAngulos(){ //complejidad O(V)
	vector< tuple<int,double> > angulos(_vertices.size());
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

void Grafo::sweep_gap(vector< vector <int> >& clusters){ //complejidad O(V*log(V))
	vector< tuple<int,double> > angulos = getAngulos();
	sort(angulos.begin(), angulos.end(), porPeso);
	vector<double> v;
	for(int i = 0; i < angulos.size(); i++){
		if(i == 0){
				v.push_back( 360 - (get<1>(angulos[angulos.size()-1]) + get<1>(angulos[0])) );
		}else{
			v.push_back(get<1>(angulos[i]) - get<1>(angulos[i-1]));
		}
	}
	int c = distance(v.begin(), max_element(v.begin(), v.end()) );
	int contenido = 0;
	int clusterNum = 0;
	int i = c;
	while(i - c < angulos.size()){
		contenido = 0;
		vector<int> temp;
		clusters.push_back(temp);
		while(contenido + _demandas[get<0>(angulos[i%v.size()])] < _capacidad && i-c<angulos.size()){
			clusters[clusterNum].push_back(get<0>(angulos[i%v.size()]));
			contenido = contenido + _demandas[get<0>(angulos[i%v.size()])];
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

vector< vector< vector< int > > > Grafo::vecindadTwoOptGrande(vector< vector< int > > sol){
	vector< vector< vector< int > > > vecindad_completa = vecindadCompletaTwoOpt(sol);
	vector< vector< vector<int> > > vecindad;
	for(int i = 0; i < vecindad_completa.size(); i++){

		vector< vector< vector< int > > > sol_temp1;
		vector< vector< int > > primero;
		primero.push_back(vecindad_completa[i][0]);
		sol_temp1.push_back(primero);
		for(int j = 0; j < vecindad_completa.size(); j++){
			vector< vector< vector< int > > > sol_temp2;
			if(j != i){
				for(int t = 0; t < sol_temp1.size(); t++){
					for(int k = 0; k < vecindad_completa[j].size(); k++){
						vector< vector< int > > sol_temp3 = sol_temp1[t];
						sol_temp3.push_back(vecindad_completa[j][k]);
						sol_temp2.push_back(sol_temp3);
					}					
				}

				sol_temp1 = sol_temp2;
			}
		}
		for(int n = 0; n < sol_temp1.size(); n++){
			vecindad.push_back(sol_temp1[n]);		
		}
	}
	return vecindad;
}

vector< vector< vector< int > > > Grafo::vecindadTwoOpt(Resultado res){
	vector<vector<int>> sol;
	for(Camion camion : res.camiones){
		sol.push_back(camion.circuito);
	}
	vector< vector< vector< int > > > vecindad_completa = vecindadCompletaTwoOpt(sol);
	vector< vector< vector<int> > > vecindad;
	for(int i = 0; i < vecindad_completa.size(); i++){
		for(int j = 1; j < vecindad_completa[i].size(); j++){
			vector< vector< int > > sol_temp;
			sol_temp.push_back(vecindad_completa[i][j]);
			for(int k = 0; k < vecindad_completa.size(); k++){
				if(k != i){
					sol_temp.push_back(vecindad_completa[k][0]);
				}
			}
			vecindad.push_back(sol_temp);
		}
	}
	return vecindad;
}

vector< vector<int> > Grafo::vecindadUnaRutaTwoOpt(vector<int> ruta){
	vector< vector <int> > rutas;
	if(ruta.size() < 4){
		rutas.push_back(ruta);
		return rutas;
	}

	for(int i = 1; i < ((int)(ruta.size())) -2; i++){
		for(int j = i+1; j < ((int)ruta.size()); j++){
			vector<int> nueva_ruta = TwoOptswap(ruta, i, j);
			rutas.push_back(nueva_ruta);
		}
	}
	return rutas;
}

vector< vector< vector< int > > > Grafo::vecindadCompletaTwoOpt(vector< vector< int > > clusters){
	vector< vector< vector< int > > > vecindad_completa;
	for(int i = 0; i < clusters.size(); i++){
		clusters[i].push_back(_deposito);
		clusters[i].insert(clusters[i].begin(),_deposito);
		vector< vector <int> > rutas = vecindadUnaRutaTwoOpt(clusters[i]);
		for(int k = 0; k < rutas.size(); k++){
			rutas[k].erase(rutas[k].begin());
			rutas[k].erase(rutas[k].begin()+rutas[k].size()-1);			
		}
		clusters[i].erase(clusters[i].begin());
		clusters[i].erase(clusters[i].begin()+clusters[i].size()-1);
		vecindad_completa.push_back(rutas);
	}
	return vecindad_completa;
}

vector<int> Grafo::TwoOptswap(vector<int> ruta, int i, int k){
	vector<int> ruta_nueva(ruta.begin(), ruta.begin() + i);
	vector<int> final(ruta.begin()+k,ruta.end());
	for(int j = k-1; j >= i; j--){
		ruta_nueva.push_back(ruta[j]);
	}
	ruta_nueva.insert(ruta_nueva.end(), final.begin(),final.end());
	return ruta_nueva;
}

double Grafo::calcularDistancia(vector<int> ruta){
	double suma = 0;
	for(int i = 0; i < ruta.size()-1; i++){
		for(int k = 0; k < _vertices[ruta[i]].size(); k++){
			suma = suma + diffEuclidea(ruta[i],ruta[i+1]);
		}
	}
	return suma;
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
	sweep_gap(v);
	return routear(v);
}

vector< vector<int> > Grafo::routear( vector< vector<int> > clusters) {
	int cantClusters = clusters.size();
	vector< vector<int> > rutas;
	for(int i = 0; i < cantClusters; i++){
		vector<Coordenadas> coordenadas_cluster;
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
            }
        }
    }
    _puntos = puntos;
}

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


bool porPeso_resultados(Resultado a, Resultado b){
	return (a.costo_total < b.costo_total);
}


void Grafo::init_kruskal_pc(){
	for(int i = 0; i<(_vertices).size(); i++){
		_padre.push_back(i);
		_altura.push_back(1);
	}
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
	std::vector<int> resultado;

	int clienteActual = cluster[cluster.size()-1];
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

Camion Grafo::generateCamion(vector<int> circuito, int id){
    Camion camion = Camion();
    camion.distancia = peso(_deposito, circuito[0]);
    camion.merch = 0;
    camion.id = id;
    for(int j=0; j<circuito.size() - 1; j++){
        camion.circuito.push_back(circuito[j]);
        camion.distancia += peso(circuito[j], circuito[j+1]);
        camion.merch += _demandas[circuito[j]];
    }
    camion.circuito.push_back(circuito[circuito.size()-1]);
    camion.distancia += peso(circuito[circuito.size()-1], _deposito);
    camion.merch += _demandas[circuito[circuito.size()-1]];
    return camion;
}


vector<Camion> Grafo::generateCamiones(vector<vector<int>> circuitos) {
    vector<Camion> camiones;
    for(int i=0; i<circuitos.size(); i++){
        camiones.push_back(generateCamion(circuitos[i], i));
    }
    return camiones;
}


bool Grafo::resultadoFactible(Resultado res){
    for(auto camion : res.camiones){
        if(camion.merch > _capacidad){
            return false;
        }
    }
    return true;
}


vector<Resultado> Grafo::vecinos_interchange(Resultado res_inicial){
    vector<Resultado> vecinos_factibles;

    vector<int> posiciones_iniciales;
    vector<unsigned long> tamanos_ciclos;
    tamanos_ciclos.push_back(0);
    for(auto camion : res_inicial.camiones){
        posiciones_iniciales.insert(posiciones_iniciales.end(), camion.circuito.begin(), camion.circuito.end());
        tamanos_ciclos.push_back(posiciones_iniciales.size());
    }

    Camion camion_temp;
    vector<int> circuito_temp;
    vector<int> posiciones_temporal;
    vector<int> ciclos_modificados;
    Resultado res_temporal;
    // itero posicion a pocision, generando nuevos vecinos factibles
    for(int i=0; i<posiciones_iniciales.size(); i++){
        for(int j=i+1; j<posiciones_iniciales.size(); j++){

            // Creo el nuevo resultado.
            res_temporal = res_inicial;
            posiciones_temporal = posiciones_iniciales;
            iter_swap(posiciones_temporal.begin() + i, posiciones_temporal.begin() + j);

            // Obtengo los ciclos que fueron modificados
            ciclos_modificados.clear();
            for(int k=0; k<tamanos_ciclos.size()-1; k++){
                if(tamanos_ciclos[k] <= i){
                	if(i < tamanos_ciclos[k+1]) {
						ciclos_modificados.push_back(k);
					}
                }else if(tamanos_ciclos[k] <= j) {
					if (j < tamanos_ciclos[k+1]) {
						ciclos_modificados.push_back(k);
					}
				}
            }

            // Creo los camiones modificados sin modificar los que no fueron modicicados
            for(int k=0; k<res_inicial.camiones.size(); k++){
                if(find(ciclos_modificados.begin(), ciclos_modificados.end(), k) != ciclos_modificados.end()) {
                    circuito_temp.clear();
                    auto inicio_circuito = posiciones_temporal.begin() + tamanos_ciclos[k];
                    auto fin_circuito = posiciones_temporal.begin() + tamanos_ciclos[k + 1];
                    circuito_temp.insert(circuito_temp.end(), inicio_circuito, fin_circuito);
                    camion_temp = generateCamion(circuito_temp, res_inicial.camiones[k].id);
                    res_temporal.camiones[k] = camion_temp;
                    res_temporal.costo_total -= res_inicial.camiones[k].distancia;
                    res_temporal.costo_total += camion_temp.distancia;
                }
            }

            // Chequeo si el resultado es factible
            if(resultadoFactible(res_temporal)){
                vecinos_factibles.push_back(res_temporal);
            }
        }
    }
    return vecinos_factibles;
}


Resultado Grafo::calcular_resultado(vector<Camion> res){
	Resultado resultado;
	resultado.camiones = res;
	resultado.costo_total = 0;
	for(auto const &camion : res){
		resultado.costo_total += camion.distancia;
	}
	return resultado;
}


Resultado Grafo::calcular_resultado(vector<vector<int>> res){
	vector<Camion> camiones = generateCamiones(res);
	return calcular_resultado(camiones);
}


vector<Resultado> Grafo::get_vecindario(Resultado res, int mode){
    if(mode == 0){
        return vecinos_interchange(res);
    }
    if(mode == 1){
        vector<Resultado> resultados;
        vector<vector<vector<int>>> vecindad = vecindadTwoOpt(res);
        for(int i = 0; i<vecindad.size(); i++){
            resultados.push_back(calcular_resultado(vecindad[i]));
        }
        return resultados;
    }
}

double enfriar(double temp, int mode, double tempMin, double tempIncial){
	if (mode == 0){
		if (fabs(temp - tempMin) < 0.000001)
			temp = tempMin - 1;
		else
			temp = (temp + tempMin) / 3;

	}
	if(mode == 1){
		if (temp < 0.000003*tempMin)
			temp = tempMin - 1;

		temp = (temp - (fabs(tempMin)/37));
	}
	if (mode == 2){
		temp = temp - fabs(tempMin - fabs(tempIncial))/30;
	}

	return temp;
}


double get_random(int max_value){
    srand(time(NULL));
    double rand_number = (rand() % (10000*max_value)) / 10000.0;
    return rand_number;
}

double get_temp(int iteracion, int cant_iteraciones, double max_temp, double min_temp, int mode){
    if(mode==4){
        return max_temp - ((double)iteracion/(double)cant_iteraciones)*(max_temp-min_temp);
    }else if(mode == 8) {
        return 1 / (((double) iteracion / (double) cant_iteraciones) + (1 / max_temp)) + min_temp;
    }else if(mode == 9) {
        return 0.002;
    }else if(mode == 1) {
        return min_temp + max_temp - pow(((double)iteracion / ((double)cant_iteraciones / (pow(max_temp, 1 / 4)))), 4);
    }else if(mode == 0){
        return min_temp + max_temp - pow(((double)iteracion/((double)cant_iteraciones/(pow(max_temp,1/8)))),8);
    }else if(mode == 2){
        double res = cos((double)iteracion/((2.0*cant_iteraciones)/M_PI))*max_temp;
        if(res < min_temp){return min_temp;}
        return res;
    }else if(mode == 5){
        double res = cos((double)iteracion/((2.0*cant_iteraciones)/M_PI) + M_PI/2.0)*max_temp + max_temp;
        if(res < min_temp){return min_temp;}
        return res;
    }else if(mode == 3){
        double res = cos((double)iteracion/((double)cant_iteraciones/M_PI))*(max_temp/2) + max_temp/2;
        if(res < min_temp){return min_temp;}
        return res;
    }else if(mode == 6){
        double res = pow((iteracion/(cant_iteraciones/pow(max_temp,1/4))) - pow(max_temp, 1/4),4);
        if(res < min_temp){return min_temp;}
        return res;
    }else if(mode == 7){
        double res = pow((iteracion/(cant_iteraciones/pow(max_temp,1/8))) - pow(max_temp, 1/8),8);
        if(res < min_temp){return min_temp;}
        return res;
    }
    return 0;
}


Resultado take_res(Resultado res_actual, vector<Resultado> vecindario, vector<Resultado> vecinos_ya_vistos, int mode, bool vecinos_change){
	if(mode == 0) {
        sort(vecindario.begin(), vecindario.end(), porPeso_resultados);
        for (auto vecino : vecindario) {
            if (find(vecinos_ya_vistos.begin(), vecinos_ya_vistos.end(), vecino) == vecinos_ya_vistos.end()) {
                return vecino;
            }
        }
    }
	if(mode == 1) {
	    vector<Resultado> diferencia;
	    if(vecinos_change) {
            sort(vecindario.begin(), vecindario.end(), porPeso_resultados);
            sort(vecinos_ya_vistos.begin(), vecinos_ya_vistos.end(), porPeso_resultados);
        }
	    set_difference(vecindario.begin(), vecindario.end(), vecinos_ya_vistos.begin(), vecinos_ya_vistos.end(),
                       inserter(diferencia, diferencia.begin()));
        if(diferencia.empty() or (diferencia.size() == 1 and diferencia[0] == res_actual)){
            Resultado nulo;
            nulo.costo_total = 0;
            return nulo;
        }
        int random_index = static_cast<int>(get_random(static_cast<int>(diferencia.size())));
        return diferencia[random_index];
	}
	return vecindario[0];
}


Resultado Grafo::simulatedAnnealing(vector<Camion> res_inicial, int enfriar_mode, int vecindario_mode, int cant_iteraciones) {
	Resultado best_res = calcular_resultado(res_inicial);
	Resultado res_actual = best_res;
	vector<Resultado> vecindario = get_vecindario(best_res, vecindario_mode);
	sort(vecindario.begin(),vecindario.end(), porPeso_resultados);
	double min_temp = vecindario[0].costo_total - res_actual.costo_total;
	double max_temp = vecindario[vecindario.size()-1].costo_total - res_actual.costo_total;
	if(min_temp < 0){
		max_temp = max_temp - min_temp;
		min_temp = 0.0002;
	}else if(min_temp > 0){
	    max_temp = max_temp + min_temp;
	    min_temp = 0.0002;
	}else{
	    max_temp += 0.0002;
	    min_temp = 0.0002;
	}
	double temperature = max_temp;
	vector<Resultado> vecinos_ya_vistos;
    bool vecinos_change = true;
    res_actual = take_res(res_actual, vecindario, vecinos_ya_vistos, 1, vecinos_change);
    Resultado res_temporal;
    double diferencia;
    double tempIncial = temperature;
    int iteracion = 0;

	while(iteracion < cant_iteraciones){
		res_temporal = take_res(res_actual, vecindario, vecinos_ya_vistos, 1, vecinos_change);
		if(res_temporal.costo_total == 0){
		    // Me atore en un maximo local
		    cout << "Me atoré en un máximo local" << endl;
		    break;
		}
        diferencia = res_temporal.costo_total - res_actual.costo_total;
		temperature = get_temp(iteracion, cant_iteraciones, max_temp, min_temp, enfriar_mode);
		//cout << "temperatura: " << temperature << "; iteracion: " << iteracion << endl;
        if(diferencia <= 0 || exp((-diferencia)/temperature) > get_random(1)){
            vecinos_ya_vistos.push_back(res_actual);
			res_actual = res_temporal;
			vecindario = get_vecindario(res_actual, vecindario_mode);
            vecinos_change = true;
			if(res_actual.costo_total < best_res.costo_total){
				best_res = res_actual;
				cout << "******************** ENCONTRE MAXIMO NUEVO VALOR " << best_res.costo_total << " EN ITERACIÓN " << iteracion << endl;
			}
		}else{
            vecinos_change = false;
			vecinos_ya_vistos.push_back(res_temporal);
		}
		// temperature = enfriar(temperature, enfriar_mode, min_temp, temperature);
        iteracion++;
	}
    return best_res;
}

/*
Resultado Grafo::simulatedAnnealing_swp(vector<Camion> res_inicial, int vecindario_mode, int enfriar_mode, double breaking_point) {
	int picking_mode = 1;
	Resultado best_res = calcular_resultado(res_inicial);
	Resultado res_actual = best_res;
	vector<Resultado> vecindario = get_vecindario(best_res, vecindario_mode);
	sort(vecindario.begin(),vecindario.end(), porPeso_resultados);
	double min_temp = vecindario[0].costo_total - res_actual.costo_total;
	double max_temp = vecindario[vecindario.size()-1].costo_total - res_actual.costo_total;
	double temperature = max_temp;
	vector<Resultado> vecinos_ya_vistos;
    // No usar
	bool vecinos_change = false;
	res_actual = take_res(res_actual, vecindario, vecinos_ya_vistos, picking_mode, vecinos_change);
	Resultado res_temporal;
	double diferencia;

	while(650 <= best_res.costo_total){
		res_temporal = take_res(res_actual, vecindario, vecinos_ya_vistos, picking_mode, vecinos_change);
        diferencia = res_temporal.costo_total - res_actual.costo_total;
        if(diferencia <= 0 || exp((-diferencia)/temperature) > get_random(1)){
            vecinos_ya_vistos.push_back(res_actual);
			res_actual = res_temporal;
			vecindario = get_vecindario(res_actual, vecindario_mode);
			if(res_actual.costo_total <= best_res.costo_total){
				best_res = res_actual;
			}
		}else{
			vecinos_ya_vistos.push_back(res_temporal);
		}
		temperature = enfriar(temperature, enfriar_mode, min_temp, temperature);
        if(temperature < (max_temp - min_temp)*breaking_point/100){
        	picking_mode = 0;
        }
	}
    return best_res;
}
*/