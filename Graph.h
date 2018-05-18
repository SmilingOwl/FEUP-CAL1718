/*
 * Graph.h
 */

#pragma once

#include <vector>
#include <algorithm>
#include <queue>
#include <list>
#include <limits>
#include <cmath>
#include <time.h>
#include <algorithm>
#include "MutablePriorityQueue.h"
#include "graphviewer.h"
#include "AuxiliarMethods.h"

using namespace std;

/*
 * setWeight (int parameter)
 * parameter = 0 -> o peso é a distancia
 * parameter = 1 -> o peso é o tempo
 * parameter = 2 -> o peso é o custo (NOT WORKING)
 * parameter = 3 -> bus é o transporte preferido
 * parameter = 4 -> metro é o transporte preferido
 *
 *
 *
 */



#define INF std::numeric_limits<double>::max()

string FILE_A = "aEspinho.txt";
string FILE_B = "bEspinho.txt";
string FILE_C = "cEspinho.txt";
string FILE_NAMES = "paragens.txt";
const int CHANGEVEHICLE = 12345; //STREET NAME

const double INTERCHANGE = 1; // km
const int WALKVELOCITY = 5; //km per hour
const int BUSVELOCITY = 18;
const int METROVELOCITY= 28;
const int BUSPRICE = 10; //cents per kilometer
const int METROPRICE = 40;


class Graph;
class Node;
class Aresta;
class IDs;

class IDs{
public:
	static unsigned long long idEdges;
	static unsigned long long idNodes;
	static unsigned long long busNumber;
	static unsigned long long metroNumber;
};



class Graph {
	vector<Node *> vertexSet;    // vertex set
	vector<Node *> vertexSetBus;
	vector<Node *> vertexSetMetro;
	GraphViewer *gv;



public:
	Node *findVertex(unsigned long long id) const;
	Node *getRandomVertex();
	bool addVertex(unsigned long long id, double lat, double lon, int vehicle);
	bool addVertex(Node* n);
	bool addEdge(unsigned long long id,unsigned long long sourc, unsigned long long dest, double w, int vehicle,string name);

	int getNumVertex() const;
	vector<Node *> getVertexSet() const;
	vector<Node *> getVertexSetBus() const;
	double getDistanceVertex(Node* n1, Node* n2);
	vector<int> edgeC;
	vector<string> nameC;
	vector<bool> twoWay;
	vector<string> vertexNames;



	void dijkstraShortestPath(int s);

	vector<unsigned long long> getPath(unsigned long long origin,unsigned long long dest);
	double getPathDistance(unsigned long long origin, unsigned long long dest);

	bool generateBusLines(vector<int> numberOfNodes,vector<pair<int,unsigned long long>> &nodes);
	bool generateMetroLines(vector<int> numberOfNodes,vector<pair<int,unsigned long long>> &nodes);

	void printView();

	void extractDataNodes();
	void extractDataArestas();
	void extractDataStreets();

	Node* getRandomVertexInDistance(Node* origin, double distance);

	vector<unsigned long long>pathNodes;
	vector<Aresta>pathArestas;


	//algorithms
	void updateWeights(int parameter); //0 - distance, 1 - time, 2 - price

	bool getShortestPath(unsigned long long origin, unsigned long long destination);
	bool getFastestPath(unsigned long long origin, unsigned long long destination);
	bool getCheapestPath(unsigned long long origin, unsigned long long destination);
	bool getBusPath(unsigned long long origin, unsigned long long destination);
	bool getMetroPath(unsigned long long origin, unsigned long long destination);

	void generatePathArestas();

	void printStreetPath();

	double pricePath();
	double distancePath();

	void printPrice();


	void printShortest(unsigned long long origin, unsigned long long destination);
	void printFastest(unsigned long long origin, unsigned long long destination);
	void printCheapest(unsigned long long origin, unsigned long long destination);
	void printBus(unsigned long long origin, unsigned long long destination);
	void printMetro(unsigned long long origin, unsigned long long destination);


	void writeNodeBus(Node* initialBusVertex);

	void writeNodeMetro(Node* initialMetroVertex);

	void writeEdge(int idNewStreet, Node* initialVertex, Node* finalVertex, int vehicle);
	void writeEdge(int idNewStreet, Node* initialVertex, Node* finalVertex, int vehicle,vector<pair<int,unsigned long long>> &nodes);
	void writeStreet(int id, string name);


	//parte 2

	vector<unsigned long long> pesquisaExata(string txt);
	vector<unsigned long long> pesquisaAproximada(string txt);
	void giveNameToAllNodes();
	void printAllNodesNames();

	vector<int> KPM_Table(string txt);
	bool KPM_Matcher(string name, string txt);
	int levenshteinDistance(const string source, const string target);
	int numberOfWords(string txt);
	vector<string> separatedWords(string txt);


};

class Aresta {
	unsigned long long id;
	Node* dest;      // destination vertex
	double weight;         // edge weight - distance in km
	string name;
	double distance;
	int vehicle; // a la pata = 0, bus = 1, metro = 2;

public:
	Aresta(unsigned long long id, Node *d, double w, string n, int vehicle, string name);
	friend class Graph;
	friend class Node;
	string getName();
	void changeName(string n);
	Node* getDest();
	void setVehicle(int n);
	int getVehicle();
	double getDistance();
	void setWeight(int parameter); //0 - distance, 1 - time, 2 - price
};

class Node {
	unsigned long long id;
	vector<Aresta> adj;  // outgoing edges
	bool visited;          // auxiliary field
	int vehicle;
	double dist = 0;
	double lat, lon;
	Node *path = NULL;
	int queueIndex = 0; 		// required by MutablePriorityQueue
	string name;


	bool processing = false;
	void addEdge(Node *dest, double w, int vehicle,string name);

public:
	Node(unsigned long long id,double lat, double lon, int vehicle);
	Node * createBusVertex();
	Node * createMetroVertex();
	void isBusNow();
	void isMetroNow();
	bool operator<(Node & vertex) const; // required by MutablePriorityQueue
	bool operator==(Node & vertex) const;
	unsigned long long getID() const;
	double getDist() const;
	Node *getPath() const;
	double getLat() const;
	double getLon() const;
	int getVehicle() const;
	void setName(string name);
	void giveName();
	string getName() const;
	Node* getRandomVertexDestination();
	vector<Aresta> getAdj();
	void updateWeights(int parameter); //0 - distance, 1 - time, 2 - price


	friend class Graph;
	friend class MutablePriorityQueue<Node>;

};





unsigned long long IDs::idEdges = 1100;
unsigned long long IDs::idNodes = 1100;
unsigned long long IDs::busNumber = 600;
unsigned long long IDs::metroNumber = 0;



int Graph::getNumVertex() const {
	return vertexSet.size();
}

double Graph::getDistanceVertex(Node* n1, Node* n2){

	return  getDistance(n1->getLat(),n1->getLon(),n2->getLat(),n2->getLon());
}

vector<Node *> Graph::getVertexSet() const {
	return vertexSet;
}

vector<Node *> Graph::getVertexSetBus() const{
	return vertexSetBus;
}

/*
 * Auxiliary function to find a vertex with a given content.
 */

Node* Graph::findVertex(unsigned long long id) const {
	for (auto v : vertexSet)
		if (v->id == id)
			return v;
	return NULL;
}


Node* Graph::getRandomVertex(){

	int r = rand() % vertexSet.size();
	return vertexSet.at(r);
}



/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */

bool Graph::addVertex(unsigned long long id, double lat, double lon, int vehicle) {
	/*
	while ( findVertex(id) != NULL){
		id++;
	}

	*/
	vertexSet.push_back(new Node(id, lat, lon, vehicle));
	return true;
}

bool Graph::addVertex(Node* n){
	if (findVertex(n->getID()) != NULL){
		return false;
	}
	vertexSet.push_back(n);
	return true;

}

/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */

bool Graph::addEdge(unsigned long long id,unsigned long long sourc, unsigned long long dest, double w, int vehicle,string name) {
	auto v1 = findVertex(sourc);
	auto v2 = findVertex(dest);
	while (v1 == NULL){
		sourc++;
		v1 = findVertex(sourc);
	}

	while (v2 == NULL){
		dest++;
		v2 = findVertex(dest);
	}
	v1->addEdge(v2,w,vehicle,name);
	return true;
}

/**
*Generates random Bus Lines, returning true if successful, false otherwise.
*/
bool Graph::generateBusLines(vector<int> numberOfNodes,vector<pair<int,unsigned long long>> &nodes){
	IDs * ids = new IDs();
	vector<int> pastNodes;


	for (unsigned int iterator = 0; iterator < numberOfNodes.size(); iterator++){

		if (numberOfNodes.at(iterator) <= 1 || numberOfNodes.at(iterator)>= this->getVertexSet().size()) return false;

		pastNodes.clear();

		Node* initialVertex = this->getRandomVertex();
		Node* initialBusVertex = initialVertex->createBusVertex();
		Node* nextVertex;
		Node* nextBusVertex;


		pastNodes.push_back(initialVertex->id);
		bool flag = false;


		//srand(time(NULL));
		int busNumber = ids->busNumber;
		string lineName = "Autocarro " + to_string(busNumber);
		this->writeStreet(busNumber,lineName);

		ids->busNumber++;

		this->writeNodeBus(initialBusVertex);

		this->writeEdge(CHANGEVEHICLE, initialBusVertex, initialVertex, 0,nodes);
		ids->idEdges++;

		//this->writeEdge(CHANGEVEHICLE, initialVertex, initialBusVertex, 0,nodes);
		ids->idEdges++;

		int fails = 25;

		for (unsigned int i = 1; i < (numberOfNodes.at(iterator) -1); i++){
			if ((initialVertex->getAdj().size() >= 1) && (initialVertex->getAdj().at(0).getVehicle() == 0)){

				do{
					flag = false;
					nextVertex = initialVertex->getRandomVertexDestination();
					printf("Size: %d", initialVertex->getAdj().size());


					for (int n = 0; n < pastNodes.size(); n++){
						if (pastNodes.at(n)==nextVertex->id){
							flag = true;

						}
					}
					fails--;

					if (fails == 1){
						break;
					}
				} while (flag);

				if( fails == 1){
					break;
				}





				pastNodes.push_back(nextVertex->id);


				nextBusVertex = nextVertex->createBusVertex();

				this->vertexSetBus.push_back(initialBusVertex);

				this->writeNodeBus(nextBusVertex);

				this->writeEdge(CHANGEVEHICLE, nextVertex, nextBusVertex, 0,nodes);
				ids->idEdges++;

				//this->writeEdge(CHANGEVEHICLE, nextBusVertex, nextVertex, 0,nodes);
				ids->idEdges++;


				this->writeEdge(busNumber, initialBusVertex, nextBusVertex, 1,nodes);
				ids->idEdges++;

				//this->writeEdge(busNumber, nextBusVertex, initialBusVertex, 1,nodes);
				ids->idEdges++;

				initialVertex = nextVertex;
				initialBusVertex = nextBusVertex;
			}
		}

			//this->vertexSetBus.push_back(nextBusVertex);

	}

	return true;

}

/**
*Used to generate random Metro lines, returns a random node within a circle of radius distance and center the origin vertex.
*/
Node* Graph::getRandomVertexInDistance(Node* origin, double distance){
	vector<Node*> nodesInDistance;

	for (unsigned int i = 0; i < vertexSet.size(); i++){
		if(getDistanceVertex(origin,vertexSet.at(i))<=distance && origin->getID() != vertexSet.at(i)->getID()){
			nodesInDistance.push_back(vertexSet.at(i));
		}
	}

	int randomIndex = rand() % nodesInDistance.size();

	return nodesInDistance.at(randomIndex);



}

/**
*Generates random Metro Lines, returning true if successful, false otherwise.
*/
bool Graph::generateMetroLines(vector<int> numberOfNodes,vector<pair<int,unsigned long long>> &nodes){
	IDs * ids = new IDs();
	string metroName[] = {"A","B","C","D","E","F","G","H","I"};
	vector<int> pastNodes;

	for(int iterator = 0; iterator < numberOfNodes.size(); iterator++){

	if (numberOfNodes.at(0) <= 1 || numberOfNodes.at(0)>= this->getVertexSet().size()) return false;

	pastNodes.clear();

	Node* initialVertex = this->getRandomVertex();
	Node* initialMetroVertex = initialVertex->createMetroVertex();
	Node* nextVertex;
	Node* nextMetroVertex;

	bool flag = false;
	pastNodes.push_back(initialVertex->id);


	int metroLine = ids->metroNumber;
	string lineName = "Metro Linha " + metroName[metroLine];
	this->writeStreet(metroLine,lineName);
	ids->metroNumber++;


	this->writeNodeMetro(initialMetroVertex);

	this->writeEdge(CHANGEVEHICLE, initialMetroVertex, initialVertex, 0,nodes);
	ids->idEdges++;

	ids->idEdges++;
	for (unsigned int i = 1; i < numberOfNodes.at(iterator) -1; i++){
		printf("node");

		do{
			flag = false;
			nextVertex = this->getRandomVertexInDistance(initialVertex, 0.1);
			for (int n = 0; n < pastNodes.size(); n++){
				if (pastNodes.at(n)==nextVertex->id){
					flag = true;
				}
			}
		} while (flag);

		pastNodes.push_back(nextVertex->id);


		this->vertexSetMetro.push_back(initialMetroVertex);


		nextMetroVertex =  nextVertex->createMetroVertex();





		this->writeNodeMetro(nextMetroVertex);


		this->writeEdge(CHANGEVEHICLE, nextVertex, nextMetroVertex, 0,nodes);
		ids->idEdges++;

		ids->idEdges++;


		this->writeEdge(metroLine, initialMetroVertex, nextMetroVertex, 2,nodes);
		ids->idEdges++;

		ids->idEdges++;

		initialVertex = nextVertex;
		initialMetroVertex = nextMetroVertex;


	}

	this->vertexSetMetro.push_back(nextMetroVertex);

	}

	return true;


}


/**
*Dijkstra Shortest Path Algorithm
*/
void Graph::dijkstraShortestPath(int origin) {


	Node* vertexOrigin = NULL;
	for (Node* v : vertexSet) {
		if (v->id == origin) {
			v->dist = 0;
			vertexOrigin = v;
		} else {
			v->dist = INF;
		}
		v->path = NULL;
	}
	MutablePriorityQueue<Node> q;
	q.insert(vertexOrigin);






	while (!q.empty()) {
		Node* v = q.extractMin();
		for (auto edge : v->adj) {
			Node* w = edge.dest;
			if (w->dist > (v->dist + edge.weight)) {
				w->dist = (v->dist + edge.weight);
				w->path = v;
				if (q.hasElement(w)) {
					q.decreaseKey(w);
				} else {
					q.insert(w);
				}
			}
		}
	}

}


/**
*Returns a vector with the ID of all nodes that belong to a path between origin and dest nodes, acording to the algorithm.
*/
vector<unsigned long long> Graph::getPath(unsigned long long origin,unsigned long long dest){

		vector<unsigned long long> res;
		vector <unsigned long long> erro;

		erro.push_back(0);
		clock_t start = clock();
		this->dijkstraShortestPath(origin);
		clock_t finish = clock();
		double timeDijkstra = double(finish-start) /CLOCKS_PER_SEC;
		Node* currentVertex = this->findVertex(dest);

		printf("Dijktra: %.6f seconds\n", timeDijkstra);

		res.push_back(currentVertex->id);
		while(currentVertex->id != origin)
		{
			if (currentVertex->path != NULL){
			currentVertex = currentVertex->path;
			res.push_back(currentVertex->id);
			} else {
				return erro;
			}
		}

		reverse(res.begin(),res.end());
		return res;
}

/**
*Returns the distance of a path.
*/
double Graph::getPathDistance(unsigned long long origin, unsigned long long dest){

	double distance = 0;
	this->dijkstraShortestPath(origin);
	Node* currentVertex = this->findVertex(dest);


	while(currentVertex->id != origin)
	{
		distance = distance + getDistanceVertex(currentVertex,currentVertex->path);
		currentVertex = currentVertex->path;

	}

	return distance;

}

void Graph::updateWeights(int parameter){
	for(unsigned int i = 0; i < this->getVertexSet().size(); i++){
		this->getVertexSet().at(i)->updateWeights(parameter);
	}
}



vector<Aresta> Node::getAdj(){
	return this->adj;
}


double Node::getLat() const{
	return this->lat;
}


double Node::getLon() const{
	return this->lon;
}


int Node::getVehicle() const{
	return this->vehicle;
}

void Node::setName(string name) {
	this->name = name;
}
string Node::getName() const {
	return this->name;
}

void Node::giveName(){

	bool flag = false;
	while(!flag){

		int r = rand() % this->adj.size();
		if (adj.at(r).name != "MUDANCA DE TRANSPORTE"){
			this->name = adj.at(r).name;
			flag = true;
		}
	}

}




Node::Node(unsigned long long id,double lat, double lon, int vehicle) {
	this->id = id;
	this->lat= lat;
	this->lon= lon;
	this->visited = false;
	this->vehicle = vehicle;
}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */


Node * Node::createBusVertex(){
	IDs *ids = new IDs();

	Node* newVertex = new Node(ids->idNodes, this->lat, this->lon,1);
	ids->idNodes++;
	newVertex->isBusNow();


	return newVertex;
}

Node * Node::createMetroVertex(){
	IDs * ids = new IDs();
	Node* newVertex = new Node(ids->idNodes, this->lat, this->lon,1);
	ids->idNodes++;
	newVertex->isMetroNow();

	return newVertex;
}


void Node::isBusNow(){
	this->vehicle = 1;
}

void Node::isMetroNow(){

	this->vehicle = 2;

}


void Node::addEdge(Node *d, double w, int vehicle, string name) {
	int id = rand() % 124567;
	Aresta newAresta(id,d,w," ",vehicle, name);
	adj.push_back(newAresta);
}


bool Node::operator<(Node & node) const {
	return this->dist < node.dist;
}


bool Node::operator==(Node & node) const {
	return this->id == node.id;
}


unsigned long long Node::getID() const {
	return this->id;
}


double Node::getDist() const {
	return this->dist;
}


Node* Node::getPath() const {
	return this->path;
}

/**
*Returns a random adjacent vertex from the original.
*/
Node* Node::getRandomVertexDestination(){



	int randomIndex = rand() % this->adj.size();




	return this->adj.at(randomIndex).getDest();
}

void Node::updateWeights(int parameter){
	for (unsigned int i = 0; i < this->adj.size(); i++){
		this->adj.at(i).setWeight(parameter);
	}
}

Aresta::Aresta(unsigned long long id, Node *d, double w, string n,int vehicle,string name){
	this->id = id;
	this->dest = d;
	this->distance = w;
	this->name = n;
	this->vehicle = vehicle;
	this->weight = 0;
	this->name = name;
}

string Aresta::getName(){
	return this->name;
}


void Aresta::changeName(string n){
	this->name = n;
}


Node* Aresta::getDest(){
	return this->dest;
}

/**
*Sets the vehicle of the edge.
*/
void Aresta::setVehicle(int n){
	this->vehicle = n;
}

/**
*Gets the vehicle of the edge.
*/
int Aresta::getVehicle(){
	return this->vehicle;
}

/**
*Gets the distance of the edge.
*/
double Aresta::getDistance(){
	return this->distance;
}



/**
 *This function sets the weight of each Edge/Aresta acording to the chosen optimization.
 * parameter = 0 -> distance
 * parameter = 1 -> time
 * parameter = 2 -> cost
 * parameter = 3 -> bus preferably
 * parameter = 4 -> metro preferably
*/
void Aresta::setWeight(int parameter){
	if (parameter == 0){
		this->weight = distance;
	} else if (parameter == 1){
		if(this->vehicle == 0){
			this->weight = distance / WALKVELOCITY;
		} else if (this->vehicle == 1){
			this->weight = distance / BUSVELOCITY;
		} else if (this->vehicle == 2){
			this->weight = distance / METROVELOCITY;
		}
	} else if (parameter == 2){
		if(this->vehicle == 0){
			this->weight = 100 * distance;
		} else if (this->vehicle == 1){
			this->weight = distance * BUSPRICE;
		} else if (this->vehicle == 2){
			this->weight = distance * METROPRICE;
		}
	} else if (parameter == 3){
		if(this->vehicle == 0){
			this->weight = 20 * distance;
		} else if (this->vehicle == 1){
			this->weight = 1 * distance;
		} else if (this->vehicle == 2){
			this->weight = 10 * distance;
		}
	} else if (parameter == 4){
		if(this->vehicle == 0){
			this->weight = 20 * distance;
		} else if (this->vehicle == 1){
			this->weight = 10 * distance;
		} else if (this->vehicle == 2){
			this->weight = 1 * distance;
		}
	}
}

/**
*This function does communication with the graphviewer application.
*/
void Graph::printView(){
		gv = new GraphViewer(1000,1000,false);
		gv->createWindow(800, 800);

		gv->defineEdgeCurved(false);

		gv->defineEdgeColor("black");
		gv->defineVertexIcon("emptyIcon.png");

		for (unsigned int i = 0; i < this->getVertexSet().size(); i++) {

			int id = this->getVertexSet().at(i)->getID();
			int x = this->getVertexSet().at(i)->getLat();
			int y = this->getVertexSet().at(i)->getLon();

			gv->addNode(id, x , y);

			if (this->getVertexSet().at(i)->getVehicle() == 1) {
				gv->setVertexIcon(id, "bus.png");
			} else if (this->getVertexSet().at(i)->getVehicle() == 2){
				gv->setVertexIcon(id,"metro.png");
			}

		}

		for (unsigned int i = 0; i < this->getVertexSet().size(); i++) {

			int idOrigem = this->getVertexSet().at(i)->getID();

			vector<Aresta> adj = this->getVertexSet().at(i)->getAdj();

			for (unsigned int j = 0; j < adj.size(); j++) {

				int idDestino = adj.at(j).getDest()->getID();

				int idAresta = 1000 * idOrigem + idDestino;



				gv->addEdge(idAresta, idOrigem, idDestino, EdgeType::DIRECTED);

			}

		}

		gv->rearrange();
}


/**
*This function generates the path when the optimization is Shortest Path. Returns 1 if possible, else returns 0
*/
bool Graph::getShortestPath(unsigned long long origin, unsigned long long destination){

	this->updateWeights(0);

	pathNodes.clear();

	if (findVertex(origin) && findVertex(destination)){
		pathNodes = getPath(origin, destination);
		if (pathNodes.at(0) != 0){
			return 1;
		}else {
			return 0;
		}
	} else {
		return 0;
	}

}

/**
*This function generates the path when the optimization is Fastest Path. Returns 1 if possible, else returns 0
*/
bool Graph::getFastestPath(unsigned long long origin, unsigned long long destination){

	this->updateWeights(1);

	pathNodes.clear();

	if (findVertex(origin) && findVertex(destination)){
		pathNodes = getPath(origin, destination);
		if (pathNodes.at(0) != 0){
			return 1;
		}else {
			return 0;
		}

	} else {
		return 0;
	}

}

/**
*This function generates the path when the optimization is Cheapest Path. Returns 1 if possible, else returns 0
*/
bool Graph::getCheapestPath(unsigned long long origin, unsigned long long destination){

	this->updateWeights(2);
	pathNodes.clear();
	if (findVertex(origin) && findVertex(destination)){
		pathNodes = getPath(origin, destination);
		if (pathNodes.at(0) != 0){
			return 1;
		}else {
			return 0;
		}
	} else {
		return 0;
	}

}

/**
*This function generates the path when the optimization is Bus Preferably. Returns 1 if possible, else returns 0
*/
bool Graph::getBusPath(unsigned long long origin, unsigned long long destination){
	this->updateWeights(3);
	pathNodes.clear();
	if (findVertex(origin) && findVertex(destination)){
		pathNodes = getPath(origin, destination);
		if (pathNodes.at(0) != 0){
			return 1;
		}else {
			return 0;
		}
	} else {
		return 0;
	}
}

/**
*This function generates the path when the optimization is Metro Preferably. Returns 1 if possible, else returns 0
*/
bool Graph::getMetroPath(unsigned long long origin, unsigned long long destination){
	this->updateWeights(4);
	pathNodes.clear();
	if (findVertex(origin) && findVertex(destination)){
		pathNodes = getPath(origin, destination);
		if (pathNodes.at(0) != 0){
			return 1;
		}else {
			return 0;
		}
	} else {
		return 0;
	}
}



/**
*This function generates a vector of Edges from the vector of the nodes of a path
*/
void Graph::generatePathArestas(){
	pathArestas.clear();
	for (unsigned int i = 0; i < this->pathNodes.size() - 1;i++){
		Node* currentVertex = findVertex(this->pathNodes.at(i));
		Node* nextVertex = findVertex(this->pathNodes.at(i+1));
		for (unsigned int n = 0; n < currentVertex->getAdj().size(); n++){
			if (currentVertex->getAdj().at(n).dest == nextVertex){
				this->pathArestas.push_back(currentVertex->getAdj().at(n));
				break;
			}
		}

	}

}

/**
*This function prints the Streets of a path
*/
void Graph::printStreetPath(){
	vector<string> streets;
	int number = 2;
	for (unsigned int i = 0; i < this->pathArestas.size();i++){
		streets.push_back(pathArestas.at(i).getName());
	}

	cout << "1: "<< streets.at(0) << endl;

	for (int it = 1; it < streets.size(); it++){
		string nome = streets.at(it);
		if (nome != streets.at(it-1) && nome != ""){
			cout << number << ": " << nome << endl;
			number++;
		}


	}

}

/**
*This function returns the total price of a path
*/
double Graph::pricePath(){
	double totalPrice = 0;
	for (unsigned int i = 0; i < this->pathArestas.size(); i++){
		if(pathArestas.at(i).getVehicle() == 1){
			totalPrice += pathArestas.at(i).getDistance() * BUSPRICE;
		} else if(pathArestas.at(i).getVehicle() == 2){
			totalPrice += pathArestas.at(i).getDistance() * METROPRICE;
		}
	}

	return totalPrice;

}

/**
*This function returns the total distance of a path
*/
double Graph::distancePath(){
	double totalDistance = 0;
	for (unsigned int i = 0; i <this->pathArestas.size(); i++){
		totalDistance += pathArestas.at(i).getDistance();
	}

	return totalDistance;
}

/**
*This function prints the result when the optimization is: Shortest Path
*/
void Graph::printShortest(unsigned long long origin, unsigned long long destination){
	clock_t begin = clock();
	int able = 0;
	able = this->getShortestPath(origin, destination);
	clock_t end = clock();
	this->generatePathArestas();

	Node* original = findVertex(origin);
	Node* final = findVertex(destination);

	if (able == 0){
		cout << "Requested path not found." << endl;
	}else {


	this->printStreetPath();

	double elapsed_time = double(end-begin) /CLOCKS_PER_SEC;

	//printf("\nElapsed Time: %.6f segundos",elapsed_time);
	printf("\nDistancia: %.3f Km",this->distancePath());
	printf("\nPreço: %.2f €",this->pricePath());

	}
}

/**
*This function prints the result when the optimization is: Fastest Path
*/
void Graph::printFastest(unsigned long long origin, unsigned long long destination){
	clock_t begin = clock();
	int able = 0;
	able = this->getFastestPath(origin, destination);
	clock_t end = clock();
	this->generatePathArestas();


	if (able == 0){
		cout << "Requested path not found." << endl;
	}else {


	this->printStreetPath();

	double elapsed_time = double(end-begin) /CLOCKS_PER_SEC;

	//printf("\nElapsed Time: %.6f segundos",elapsed_time);
	printf("\nDistancia: %.3f Km",this->distancePath());
	printf("\nPreço: %.2f €",this->pricePath());

	}
}

/**
*This function prints the result when the optimization is: Cheapest Path
*/
void Graph::printCheapest(unsigned long long origin, unsigned long long destination){
	clock_t begin = clock();
	int able = 0;
	able = this->getCheapestPath(origin, destination);
	clock_t end = clock();
	this->generatePathArestas();

	if (able == 0){
		cout << "Requested path not found." << endl;
	}else {


	this->printStreetPath();

	double elapsed_time = double(end-begin) /CLOCKS_PER_SEC;

	//printf("\nElapsed Time: %.6f segundos",elapsed_time);
	printf("\nDistancia: %.3f Km",this->distancePath());
	printf("\nPreço: %.2f €",this->pricePath());

	}
}

/**
*This function prints the result when the optimization is: Bus Preferably
*/
void Graph::printBus(unsigned long long origin, unsigned long long destination){
	clock_t begin = clock();
	int able = 0;
	able = this->getBusPath(origin, destination);
	clock_t end = clock();
	this->generatePathArestas();

	if (able == 0){
		cout << "Requested path not found." << endl;
	}else {


	this->printStreetPath();

	double elapsed_time = double(end-begin) /CLOCKS_PER_SEC;

	//printf("\nElapsed Time: %.6f segundos",elapsed_time);
	printf("\nDistancia: %.3f Km",this->distancePath());
	printf("\nPreço: %.2f €",this->pricePath());

}

}

/**
*This function prints the result when the optimization is: Metro Preferably
*/
void Graph::printMetro(unsigned long long origin, unsigned long long destination){
	clock_t begin = clock();
	int able = 0;
	able = this->getMetroPath(origin, destination);
	clock_t end = clock();
	this->generatePathArestas();

	if (able == 0){
		cout << "Requested path not found." << endl;
	}else {


	this->printStreetPath();

	double elapsed_time = double(end-begin) /CLOCKS_PER_SEC;

	//printf("\nElapsed Time: %.6f segundos",elapsed_time);
	printf("\nDistancia: %.3f Km",this->distancePath());
	printf("\nPreço: %.2f €",this->pricePath());

	}
}

/**
*This function writes a node of Bus to the file A
*/
void Graph::writeNodeBus(Node* initialBusVertex) {

	string lll;
		lll= FILE_A;

		ofstream myfile;
			myfile.open(lll,ios::app);// escrever no fim do ficheiro.
			if (myfile.is_open())
			{
				myfile << "\n"<< initialBusVertex->getID() << ";"<< initialBusVertex->getLat()<< ";"<<initialBusVertex->getLon()<< ";"<< initialBusVertex->getVehicle() ;

				myfile.close();
			}

			printf("write node bus \n");
}


/**
*This function writes a node of Metro to the file A
*/
void Graph::writeNodeMetro(Node* initialMetroVertex ) {

	string lll;
		lll= FILE_A;

		ofstream myfile;
			myfile.open(lll,ios::app);// escrever no fim do ficheiro.
			if (myfile.is_open())
			{
				myfile << "\n"<< initialMetroVertex->getID() << ";"<< initialMetroVertex->getLat()<< ";"<<initialMetroVertex->getLon()<< ";"<< initialMetroVertex->getVehicle() ;

				myfile.close();
			}
}

/**
*This function writes Edge related info to the file C
*/
void Graph::writeEdge(int idNewStreet, Node* initialVertex, Node* finalVertex, int vehicle,vector<pair<int,unsigned long long>> &nodes) {

	unsigned long long iV,fV;
	iV=initialVertex->getID();
	fV=finalVertex->getID();
	for(unsigned int i=0; i < nodes.size();i++)
		{
			if( nodes.at(i).first==initialVertex->getID())
			{
				iV= nodes.at(i).second;
			}
		}
	for(unsigned int i=0; i < nodes.size();i++)
			{
				if( nodes.at(i).first==finalVertex->getID())
				{
					fV= nodes.at(i).second;
				}
			}
	string lll;
			lll= FILE_C;

			ofstream myfile;
				myfile.open(lll,ios::app);// escrever no fim do ficheiro.
				if (myfile.is_open())
				{
					myfile << "\n"<< idNewStreet << ";"<< iV<< ";"<<fV << ";" << vehicle;

					myfile.close();
				}

				printf("write edge \n");


}

/**
*This function writes Edge related info to the file C
*/
void Graph::writeEdge(int idNewStreet, Node* initialVertex, Node* finalVertex, int vehicle) {

	string lll;
			lll= FILE_C;

			ofstream myfile;
				myfile.open(lll,ios::app);// escrever no fim do ficheiro.
				if (myfile.is_open())
				{
					myfile << "\n"<< idNewStreet << ";"<< initialVertex->getID()<< ";"<<finalVertex->getID() << ";" << vehicle;

					myfile.close();
				}

				printf("write edge \n");


}

/**
*This function writes Street related info to the file B
*/
void Graph::writeStreet(int id, string name){
    string ll;
	ll= FILE_B;

	ofstream myfile;
		myfile.open(ll,ios::app);// escrever no fim do ficheiro.
		if (myfile.is_open())
		{
			myfile << "\n"<< id << ";"<< name<< ";"<< "False" ;
			myfile.close();
		}

		printf("write street \n");
}


//parte 2


int Graph::levenshteinDistance(const string source, const string target){
	const std::size_t len1 = source.size(), len2 = target.size();
	std::vector<std::vector<unsigned int>> d(len1 + 1, std::vector<unsigned int>(len2 + 1));

	d[0][0] = 0;
	for(unsigned int i = 1; i <= len1; ++i) d[i][0] = i;
	for(unsigned int i = 1; i <= len2; ++i) d[0][i] = i;

	for(unsigned int i = 1; i <= len1; ++i)
		for(unsigned int j = 1; j <= len2; ++j)
                      d[i][j] = std::min({ d[i - 1][j] + 1, d[i][j - 1] + 1, d[i - 1][j - 1] + (source[i - 1] == target[j - 1] ? 0 : 1) });
	return d[len1][len2];
}

vector<unsigned long long> Graph::pesquisaAproximada(string txt){

	vector <unsigned long long> resultado;

	int numPalavrasNode = 0;
	int numPalavrasTXT = numberOfWords(txt);

	for (unsigned int i = 0; i < this->vertexSet.size(); i++){

		numPalavrasNode = numberOfWords(this->vertexSet.at(i)->getName());

		if (numPalavrasNode < 2 || numPalavrasTXT > 1){

			if (levenshteinDistance(this->vertexSet.at(i)->name,txt) < 3){
				resultado.push_back(this->vertexSet.at(i)->id);
			}
		} else {
			vector<string> separatedVector = separatedWords(this->vertexSet.at(i)->name);

			for (unsigned int n = 0; n < separatedVector.size(); n++){
				if (levenshteinDistance(separatedVector.at(n),txt) < 2){
					resultado.push_back(this->vertexSet.at(i)->id);
					break;
				}
			}
		}

	}
	return resultado;

}

vector<int> Graph::KPM_Table(string txt){
	//W
	vector<int> T;
	int pos = 1;
	int cnd = 0;

	T.push_back(-1);

	while (pos < txt.length()){
		if (txt[pos] == txt[cnd]){
			T.at(pos) = T.at(cnd);
			pos++;
			cnd++;
		} else {
			T.at(pos) = cnd;

			cnd = T.at(cnd);

			while(cnd >= 0 && txt[pos] != txt[cnd]){
				cnd = T.at(cnd);
			}

			pos++;
			cnd++;
		}
	}

	T.at(pos) = cnd;



	return T;
}

bool Graph::KPM_Matcher(string name, string txt){
	//S, W

	int j = 0;
	int k = 0;
	int T[100];

	// table

	int pos = 1;
	int cnd = 0;

	T[0]= -1;

	while (pos < txt.length()){
		if (txt[pos] == txt[cnd]){
			T[pos] = T[cnd];
			pos++;
			cnd++;
		} else {
			T[pos] = cnd;

			cnd = T[cnd];

			while(cnd >= 0 && txt[pos] != txt[cnd]){
				cnd = T[cnd];
			}

			pos++;
			cnd++;
		}
	}

	T[pos] = cnd;



	while (j < name.length()){
		if (txt[k] == name[j]){
			j++;
			k++;
			if (k == txt.length()){
				return true;
			}

		} else {
			k = T[k];
			if (k < 0){
				j++;
				k++;
			}
		}
	}


	return false;
}

vector<unsigned long long> Graph::pesquisaExata(string txt){
	vector <unsigned long long> resultado;

	for (unsigned int i = 0; i < this->vertexSet.size(); i++){
		if (KPM_Matcher(this->vertexSet.at(i)->name,txt)){
			resultado.push_back(this->vertexSet.at(i)->id);
		}
	}
	return resultado;
}

void Graph::giveNameToAllNodes(){
	for (unsigned int i = 0; i < this->vertexSet.size(); i++){
		vertexSet.at(i)->setName(vertexNames.at(i));
	}

}

void Graph::printAllNodesNames(){
	for(unsigned int i = 0; i < vertexSet.size(); i++){
		cout << i << ": " << vertexSet.at(i)->name << endl;
	}
}

int Graph::numberOfWords(string txt){

	    istringstream iss(txt);
	    int i = 0;

	    do
	    {
	        string subs;
	        iss >> subs;
	        i++;

	    } while (iss);
	    i--;
	    return i;
}

vector<string> Graph::separatedWords(string txt){
	    istringstream iss(txt);
	    vector<string> result;

	    do
	    {
	        string subs;
	        iss >> subs;
	        result.push_back(subs);

	    } while (iss);

	    return result;
}




