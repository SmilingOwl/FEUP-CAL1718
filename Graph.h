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

const string FILE_A = "a1000.txt";
const string FILE_B = "b1000.txt";
const string FILE_C = "c1000.txt";
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
};



class Graph {
	vector<Node *> vertexSet;    // vertex set
	vector<Node *> vertexSetBus;
	vector<Node *> vertexSetMetro;
	GraphViewer *gv;



public:
	Node *findVertex(unsigned long long id) const;
	Node *getRandomVertex();
	Node *getRandomBusVertex();
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



	void dijkstraShortestPath(int s);
	void dijkstraShortestPathOld(int s);
	void unweightedShortestPath(int s);
	void bellmanFordShortestPath(int s);
	vector<unsigned long long> getPath(unsigned long long origin,unsigned long long dest);
	double getPathDistance(unsigned long long origin, unsigned long long dest);

	bool generateBusLines(vector<int> numberOfNodes);
	bool generateMetroLines(vector<int> numberOfNodes);

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

	void writeStreet(int id, string name);

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
	Node* getRandomVertexDestination();
	vector<Aresta> getAdj();
	void updateWeights(int parameter); //0 - distance, 1 - time, 2 - price

	friend class Graph;
	friend class MutablePriorityQueue<Node>;

};





unsigned long long IDs::idEdges = 1;
unsigned long long IDs::idNodes = 1;



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
	srand(time(NULL));
	int r = rand() % vertexSet.size();
	return vertexSet.at(r);
}

Node* Graph::getRandomBusVertex(){
	srand(time(NULL));
	int r = rand() % vertexSetBus.size();
	return vertexSetBus.at(r);
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

bool Graph::generateBusLines(vector<int> numberOfNodes){
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
		bool beco = false;

		srand(time(NULL));
		int busNumber = rand() % 899 + 100;
		string lineName = "Autocarro " + to_string(busNumber);
		this->writeStreet(busNumber,lineName);



		this->writeNodeBus(initialBusVertex);

		this->writeEdge(CHANGEVEHICLE, initialBusVertex, initialVertex, 0);
		ids->idEdges++;

		this->writeEdge(CHANGEVEHICLE, initialVertex, initialBusVertex, 0);
		ids->idEdges++;

		for (unsigned int i = 1; i < (numberOfNodes.at(iterator) -1); i++){
			if ((initialVertex->getAdj().size() >= 1) && (initialVertex->getAdj().at(0).getVehicle() == 0)){

				do{
					flag = false;
					nextVertex = initialVertex->getRandomVertexDestination();


					for (int n = 0; n < pastNodes.size(); n++){
						if (pastNodes.at(n)==nextVertex->id){
							flag = true;

						}
					}
				} while (flag);





				pastNodes.push_back(nextVertex->id);


				nextBusVertex = nextVertex->createBusVertex();

				this->vertexSetBus.push_back(initialBusVertex);

				this->writeNodeBus(nextBusVertex);

				this->writeEdge(CHANGEVEHICLE, nextVertex, nextBusVertex, 0);
				ids->idEdges++;

				this->writeEdge(CHANGEVEHICLE, nextBusVertex, nextVertex, 0);
				ids->idEdges++;


				this->writeEdge(busNumber, initialBusVertex, nextBusVertex, 1);
				ids->idEdges++;

				this->writeEdge(busNumber, nextBusVertex, initialBusVertex, 1);
				ids->idEdges++;

				initialVertex = nextVertex;
				initialBusVertex = nextBusVertex;
			}
		}

			this->vertexSetBus.push_back(nextBusVertex);

	}

	return true;

}

Node* Graph::getRandomVertexInDistance(Node* origin, double distance){
	vector<Node*> nodesInDistance;

	for (unsigned int i = 0; i < vertexSet.size(); i++){
		if(getDistanceVertex(origin,vertexSet.at(i))<=distance){
			nodesInDistance.push_back(vertexSet.at(i));
		}
	}

	int randomIndex = rand() % nodesInDistance.size();

	return nodesInDistance.at(randomIndex);



}

bool Graph::generateMetroLines(vector<int> numberOfNodes){
	IDs * ids = new IDs();
	string metroName[] = {"A","B","C","D","E","F","G","H","I"};
	vector<int> pastNodes;

	if (numberOfNodes.at(0) <= 1 || numberOfNodes.at(0)>= this->getVertexSet().size()) return false;

	pastNodes.clear();

	Node* initialVertex = this->getRandomVertex();
	Node* initialMetroVertex = initialVertex->createMetroVertex();
	Node* nextVertex;
	Node* nextMetroVertex;

	bool flag = false;
	pastNodes.push_back(initialVertex->id);


	int metroLine = rand() % 9;
	string lineName = "Metro Linha " + metroName[metroLine];
	this->writeStreet(metroLine,lineName);


	this->writeNodeMetro(initialMetroVertex);
	//this->addEdge(CHANGEVEHICLE,initialMetroVertex->getID(),initialBusVertex->getID(),INTERCHANGE,0);
	this->writeEdge(CHANGEVEHICLE, initialMetroVertex, initialVertex, 0);
	ids->idEdges++;
	//this->addEdge(CHANGEVEHICLE,initialBusVertex->getID(),initialMetroVertex->getID(),INTERCHANGE,0);
	this->writeEdge(CHANGEVEHICLE, initialVertex, initialMetroVertex, 0);
	ids->idEdges++;
	for (unsigned int i = 1; i < numberOfNodes.at(0) -1; i++){

		do{
			flag = false;
			nextVertex = this->getRandomVertexInDistance(initialVertex, 0.5);
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


		this->writeEdge(CHANGEVEHICLE, nextVertex, nextMetroVertex, 0);
		ids->idEdges++;

		this->writeEdge(CHANGEVEHICLE, nextMetroVertex, nextVertex, 0);
		ids->idEdges++;


		this->writeEdge(metroLine, initialMetroVertex, nextMetroVertex, 2);
		ids->idEdges++;

		this->writeEdge(metroLine, nextMetroVertex, initialMetroVertex, 2);
		ids->idEdges++;

		initialVertex = nextVertex;
		initialMetroVertex = nextMetroVertex;


	}

	this->vertexSetMetro.push_back(nextMetroVertex);

	return true;


}




/**************** Single Source Shortest Path algorithms ************/


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
		for (Aresta edge : v->adj) {
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



vector<unsigned long long> Graph::getPath(unsigned long long origin,unsigned long long dest){

		vector<unsigned long long> res;
		this->dijkstraShortestPath(origin);
		Node* currentVertex = this->findVertex(dest);

		res.push_back(currentVertex->id);
		while(currentVertex->id != origin)
		{
			currentVertex = currentVertex->path;
			res.push_back(currentVertex->id);
		}

		reverse(res.begin(),res.end());
		return res;
}

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


Node* Node::getRandomVertexDestination(){



	int randomIndex = rand() % this->adj.size();




	return this->adj.at(0).getDest();
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

void Aresta::setVehicle(int n){
	this->vehicle = n;
}

int Aresta::getVehicle(){
	return this->vehicle;
}

double Aresta::getDistance(){
	return this->distance;
}

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
			this->weight = 5 * distance;
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

				/*
				//string weight = std::to_string(adj.at(j).getWeight());
				string weight = doubleToString(adj.at(j).getWeight());


				if(weight.find(".") != string::npos){
					for (size_t i = weight.find(".") + 2; i < weight.size(); i++)
						weight.erase(i);
				}
				*/

				gv->addEdge(idAresta, idOrigem, idDestino, EdgeType::DIRECTED);
				//gv->setEdgeLabel(idAresta, weight);
			}

		}

		gv->rearrange();
}



bool Graph::getShortestPath(unsigned long long origin, unsigned long long destination){

	this->updateWeights(0);

	pathNodes.clear();

	if (findVertex(origin) && findVertex(destination)){
		pathNodes = getPath(origin, destination);
		return 1;
	} else {
		return 0;
	}

}

bool Graph::getFastestPath(unsigned long long origin, unsigned long long destination){

	this->updateWeights(1);

	pathNodes.clear();

	if (findVertex(origin) && findVertex(destination)){
		pathNodes = getPath(origin, destination);
		return 1;
	} else {
		return 0;
	}

}//caminho mais curto em km apenas !

bool Graph::getCheapestPath(unsigned long long origin, unsigned long long destination){

	this->updateWeights(2);
	pathNodes.clear();
	if (findVertex(origin) && findVertex(destination)){
		pathNodes = getPath(origin, destination);
		return 1;
	} else {
		return 0;
	}

}

bool Graph::getBusPath(unsigned long long origin, unsigned long long destination){
	this->updateWeights(3);
	pathNodes.clear();
	if (findVertex(origin) && findVertex(destination)){
		pathNodes = getPath(origin, destination);
		return 1;
	} else {
		return 0;
	}
}

bool Graph::getMetroPath(unsigned long long origin, unsigned long long destination){
	this->updateWeights(4);
	pathNodes.clear();
	if (findVertex(origin) && findVertex(destination)){
		pathNodes = getPath(origin, destination);
		return 1;
	} else {
		return 0;
	}
}

//run this after generated pathNodes

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

void Graph::printStreetPath(){
	vector<string> streets;
	for (unsigned int i = 0; i < this->pathArestas.size();i++){
		streets.push_back(pathArestas.at(i).getName());
	}

	for (int it = 0; it < streets.size(); it++){
		string nome = streets.at(it);
		cout << it << ": " << nome << endl;
	}

}


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

double Graph::distancePath(){
	double totalDistance = 0;
	for (unsigned int i = 0; i <this->pathArestas.size(); i++){
		totalDistance += pathArestas.at(i).getDistance();
	}

	return totalDistance;
}

void Graph::printShortest(unsigned long long origin, unsigned long long destination){
	clock_t begin = clock();
	this->getShortestPath(origin, destination);
	clock_t end = clock();
	this->generatePathArestas();

	this->printStreetPath();

	double elapsed_time = double(end-begin) /CLOCKS_PER_SEC;

	printf("\nElapsed Time: %.6f",elapsed_time);
	printf("\nDistancia: %.3f Km",this->distancePath());
	printf("\nPreço: %.2f",this->pricePath());
}
void Graph::printFastest(unsigned long long origin, unsigned long long destination){
	clock_t begin = clock();
	this->getFastestPath(origin, destination);
	clock_t end = clock();
	this->generatePathArestas();

	this->printStreetPath();

	double elapsed_time = double(end-begin) /CLOCKS_PER_SEC;

	printf("\nElapsed Time: %.6f",elapsed_time);
	printf("\nDistancia: %.3f Km",this->distancePath());
	printf("\nPreço: %.2f",this->pricePath());
}

void Graph::printCheapest(unsigned long long origin, unsigned long long destination){
	clock_t begin = clock();
	this->getCheapestPath(origin, destination);
	clock_t end = clock();
	this->generatePathArestas();

	this->printStreetPath();

	double elapsed_time = double(end-begin) /CLOCKS_PER_SEC;

	printf("\nElapsed Time: %.6f",elapsed_time);
	printf("\nDistancia: %.3f Km",this->distancePath());
	printf("\nPreço: %.2f",this->pricePath());
}
void Graph::printBus(unsigned long long origin, unsigned long long destination){
	clock_t begin = clock();
	this->getBusPath(origin, destination);
	clock_t end = clock();
	this->generatePathArestas();

	this->printStreetPath();

	double elapsed_time = double(end-begin) /CLOCKS_PER_SEC;

	printf("\nElapsed Time: %.6f",elapsed_time);
	printf("\nDistancia: %.3f Km",this->distancePath());
	printf("\nPreço: %.2f",this->pricePath());

}
void Graph::printMetro(unsigned long long origin, unsigned long long destination){
	clock_t begin = clock();
	this->getMetroPath(origin, destination);
	clock_t end = clock();
	this->generatePathArestas();

	this->printStreetPath();

	double elapsed_time = double(end-begin) /CLOCKS_PER_SEC;


	printf("\nElapsed Time: %.6f",elapsed_time);
	printf("\nDistancia: %.3f Km",this->distancePath());
	printf("\nPreço: %.2f",this->pricePath());
}


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






