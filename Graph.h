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
#include "MutablePriorityQueue.h"
#include "graphviewer.h"
#include "AuxiliarMethods.h"

using namespace std;



#define INF std::numeric_limits<double>::max()

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
	static unsigned long long idEdges = 1;
	static unsigned long long idNodes = 1;
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
	bool addEdge(unsigned long long id,int sourc, int dest, double w, int vehicle);

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
	double getPathDistance(int origin, int dest);

	bool generateBusLines(int numberOfNodes);
	bool generateMetroLine(int numberOfNodes);

	void printView();

	void extractDataNodes();
	void extractDataArestas();
	void extractDataStreets();


	vector<Node*>pathNodes;
	vector<Aresta>pathArestas;


	//algorithms
	void updateWeights(int parameter); //0 - distance, 1 - time, 2 - price

	bool getShortestPath(unsigned long long origin, unsigned long long destination);
	bool getFastestPath(unsigned long long origin, unsigned long long destination);
	bool getCheapestPath(unsigned long long origin, unsigned long long destination);

	void generatePathArestas();

	void printStreetPath();
	double pricePath();
	void printPrice();


};

class Aresta {
	unsigned long long id;
	Node* dest;      // destination vertex
	double weight;         // edge weight - distance in km
	string name;
	double distance;
	int vehicle; // a la pata = 0, bus = 1, metro = 2;

public:
	Aresta(unsigned long long id, Node *d, double w, string n, int vehicle);
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
	void addEdge(Node *dest, double w, int vehicle);

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

bool Graph::addEdge(unsigned long long id,int sourc, int dest, double w, int vehicle) {
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
	v1->addEdge(v2,w,vehicle);
	return true;
}

bool Graph::generateBusLines(int numberOfNodes){
	IDs ids = new IDs();

	if (numberOfNodes <= 1 || numberOfNodes>= this->getVertexSet().size()) return false;

	Node* initialVertex = this->getRandomVertex();
	Node* initialBusVertex = initialVertex->createBusVertex();
	Node* nextVertex;
	Node* nextBusVertex;


	this->addVertex(initialBusVertex);
	this->addEdge(ids.idEdges,initialBusVertex->getID(),initialVertex->getID(),INTERCHANGE,0);
	ids.idEdges++;
	this->addEdge(ids.idEdges,initialVertex->getID(),initialBusVertex->getID(),INTERCHANGE,0);
	ids.idEdges++;
	for (unsigned int i = 1; i < numberOfNodes -1; i++){




		nextVertex = initialVertex->getRandomVertexDestination();
		nextBusVertex =  nextVertex->createBusVertex();

		this->vertexSetBus.push_back(initialBusVertex);


		this->addVertex(nextBusVertex);
		this->addEdge(ids.idEdges,nextVertex->getID(), nextBusVertex->getID(),INTERCHANGE,0);
		ids.idEdges++;
		this->addEdge(ids.idEdges,nextBusVertex->getID(), nextVertex->getID(),INTERCHANGE,0);
		ids.idEdges++;

		this->addEdge(ids.idEdges,initialBusVertex->getID(),nextBusVertex->getID(),getDistanceVertex(initialBusVertex,nextBusVertex),1);
		ids.idEdges++;
		this->addEdge(ids.idEdges,nextBusVertex->getID(),initialBusVertex->getID(),getDistanceVertex(initialBusVertex,nextBusVertex),1);
		ids.idEdges++;

		initialVertex = nextVertex;
		initialBusVertex = nextBusVertex;


	}

	this->vertexSetBus.push_back(nextBusVertex);


}

bool Graph::generateMetroLine(int numberOfNodes){
	IDs ids = new IDs();
	if (numberOfNodes <= 1 || numberOfNodes>= this->getVertexSetBus().size()) return false;

	Node* initialBusVertex = this->getRandomBusVertex();
	Node* initialMetroVertex = initialBusVertex->createMetroVertex();
	Node* nextBusVertex;
	Node* nextMetroVertex;


	this->addVertex(initialMetroVertex);
	this->addEdge(ids.idEdges,initialMetroVertex->getID(),initialBusVertex->getID(),INTERCHANGE,0);
	ids.idEdges++;
	this->addEdge(ids.idEdges,initialBusVertex->getID(),initialMetroVertex->getID(),INTERCHANGE,0);
	ids.idEdges++;
	for (unsigned int i = 1; i < numberOfNodes -1; i++){

		this->vertexSetMetro.push_back(initialMetroVertex);


		nextBusVertex = initialBusVertex->getRandomVertexDestination();
		nextMetroVertex =  nextBusVertex->createMetroVertex();




		this->addVertex(nextMetroVertex);
		this->addEdge(ids.idEdges,nextBusVertex->getID(), nextMetroVertex->getID(),INTERCHANGE,0);
		ids.idEdges++;
		this->addEdge(ids.idEdges,nextMetroVertex->getID(), nextBusVertex->getID(),INTERCHANGE,0);
		ids.idEdges++;

		this->addEdge(ids.idEdges,initialMetroVertex->getID(),nextMetroVertex->getID(),getDistanceVertex(initialBusVertex,nextBusVertex),1);
		ids.idEdges++;
		this->addEdge(ids.idEdges,nextMetroVertex->getID(),initialMetroVertex->getID(),getDistanceVertex(initialBusVertex,nextBusVertex),1);
		ids.idEdges++;

		initialBusVertex = nextBusVertex;
		initialMetroVertex = nextMetroVertex;


	}

	this->vertexSetMetro.push_back(nextMetroVertex);


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

double Graph::getPathDistance(int origin, int dest){

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
	IDs ids = new IDs();

	Node* newVertex = new Node(ids.idNodes, this->lat, this->lon,1);
	ids.idNodes++;
	newVertex->isBusNow();


	return newVertex;
}

Node * Node::createMetroVertex(){
	IDs ids = new IDs();
	Node* newVertex = new Node(ids.idNodes, this->lat, this->lon,1);
	ids.idNodes++;
	newVertex->isMetroNow();

	return newVertex;
}


void Node::isBusNow(){
	this->vehicle = 1;
}

void Node::isMetroNow(){

	this->vehicle = 2;

}


void Node::addEdge(Node *d, double w, int vehicle) {
	int id = rand() % 124567;
	Aresta newAresta(id,d,w," ",vehicle);
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

	return this->adj.at(randomIndex).getDest();
}

void Node::updateWeights(int parameter){
	for (unsigned int i = 0; i < this->adj.size(); i++){
		this->adj.at(i).setWeight(parameter);
	}
}

Aresta::Aresta(unsigned long long id, Node *d, double w, string n,int vehicle){
	this->id = id;
	this->dest = d;
	this->distance = w;
	this->name = n;
	this->vehicle = vehicle;
	this->weight = 0;
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
 * parameter = 3 -> bus é o transporte preferido  //TODO
 * parameter = 4 -> metro é o transporte preferido  // TODO
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
			this->weight = 0;
		} else if (this->vehicle == 1){
			this->weight = distance * BUSPRICE;
		} else if (this->vehicle == 2){
			this->weight = distance * METROPRICE;
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

//TODO Atualizar pesos!!!

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

}

bool Graph::getCheapestPath(unsigned long long origin, unsigned long long destination){

	this->updateWeights(2);
	if (findVertex(origin) && findVertex(destination)){
		pathNodes = getPath(origin, destination);
		return 1;
	} else {
		return 0;
	}

}

//run this after generated pathNodes
void Graph::generatePathArestas(){
	for (unsigned int i = 0; i < this->pathNodes - 1;i++){
		Node* currentVertex = findVertex(this->pathNodes.at(i)->getID());
		Node* nextVertex = findVertex(this->pathNodes.at(i+1)->getID());
		for (unsigned int n = 0; n < currentVertex->getAdj().size(); n++){
			if (currentVertex->getAdj().at(i).dest == nextVertex){
				this->pathArestas.push_back(currentVertex->getAdj().at(i));
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
		printf("%d: %s \n",it,streets.at(it));
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

double Graph::printPrice(){
	double totalPrice = this->pricePath();

	printf("%d €",totalPrice);
}






