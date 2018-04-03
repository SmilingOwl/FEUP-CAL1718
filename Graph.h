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

using namespace std;



#define INF std::numeric_limits<double>::max()

class Graph;
class Node;
class Aresta;

class Graph {
	vector<Node *> vertexSet;    // vertex set
	GraphViewer *gv;

public:
	Node *findVertex(int id) const;
	Node *getRandomVertex();
	bool addVertex(int id, int lat, int lon);
	bool addVertex(Node* n);
	bool addEdge(int id,int sourc, int dest, double w);

	int getNumVertex() const;
	vector<Node *> getVertexSet() const;
	vector<int> edgeC;
	vector<string> nameC;
	vector<bool> twoWay;

	// Fp05 - single source
	void dijkstraShortestPath(int s);
	void dijkstraShortestPathOld(int s);
	void unweightedShortestPath(int s);
	void bellmanFordShortestPath(int s);
	vector<int> getPath(int origin,int dest);

	void printView();

	void extractDataNodes();
	void extractDataArestas();
	void extractDataStreets();

};

class Aresta {
	int id;
	Node* dest;      // destination vertex
	double weight;         // edge weight - distance in km
	string name;
public:
	Aresta(int id, Node *d, double w, string n);
	friend class Graph;
	friend class Node;
	void changeName(string n);
	Node* getDest();
};

class Node {
	int id;
	vector<Aresta> adj;  // outgoing edges
	bool visited;          // auxiliary field
	bool bus = false;
	double dist = 0;
	double lat, lon;
	Node *path = NULL;
	int queueIndex = 0; 		// required by MutablePriorityQueue

	bool processing = false;
	void addEdge(Node *dest, double w);

public:
	Node(int id,double lat, double lon);
	Node * createBusVertex();
	void isBusNow();
	bool operator<(Node & vertex) const; // required by MutablePriorityQueue
	bool operator==(Node & vertex) const;
	int getID() const;
	double getDist() const;
	Node *getPath() const;
	double getLat() const;
	double getLon() const;
	bool getBus() const;
	Node* getRandomVertexDestination();
	vector<Aresta> getAdj();

	friend class Graph;
	friend class MutablePriorityQueue<Node>;

};









int Graph::getNumVertex() const {
	return vertexSet.size();class Node {
		int id;
		vector<Aresta> adj;  // outgoing edges
		bool visited;          // auxiliary field
		bool bus = false;
		double dist = 0;
		double lat, lon;
		Node *path = NULL;
		int queueIndex = 0; 		// required by MutablePriorityQueue

		bool processing = false;
		void addEdge(Node *dest, double w);

	public:
		Node(int id,double lat, double lon);
		Node * createBusVertex();
		void isBusNow();
		bool operator<(Node & vertex) const; // required by MutablePriorityQueue
		bool operator==(Node & vertex) const;
		int getID() const;
		double getDist() const;
		Node *getPath() const;
		double getLat() const;
		double getLon() const;
		bool getBus() const;
		Node* getRandomVertexDestination();
		vector<Aresta> getAdj();


		friend class MutablePriorityQueue;
		friend class Graph;


	};
}


vector<Node *> Graph::getVertexSet() const {
	return vertexSet;
}

/*
 * Auxiliary function to find a vertex with a given content.
 */

Node* Graph::findVertex(int id) const {
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

/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */

bool Graph::addVertex(int id, int lat, int lon) {
	while ( findVertex(id) != NULL){
		id++;
	}
	vertexSet.push_back(new Node(id, lat, lon));
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

bool Graph::addEdge(int id,int sourc, int dest, double w) {
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
	v1->addEdge(v2,w);
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



vector<int> Graph::getPath(int origin,int dest){

		vector<int> res;
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



vector<Aresta> Node::getAdj(){
	return this->adj;
}


double Node::getLat() const{
	return this->lat;
}


double Node::getLon() const{
	return this->lon;
}


bool Node::getBus() const{
	return this->bus;
}



Node::Node(int id,double lat, double lon) {
	this->id = id;
	this->lat= lat;
	this->lon= lon;
	this->visited = false;
}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */


Node * Node::createBusVertex(){
	Node* newVertex = new Node((this->id)%10000, this->lat, this->lon);
	newVertex->isBusNow();


	return newVertex;
}


void Node::isBusNow(){
	this->bus = true;
}


void Node::addEdge(Node *d, double w) {
	int id = rand() % 124567;
	Aresta newAresta(id,d,w," ");
	adj.push_back(newAresta);
}


bool Node::operator<(Node & node) const {
	return this->dist < node.dist;
}


bool Node::operator==(Node & node) const {
	return this->id == node.id;
}


int Node::getID() const {
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

Aresta::Aresta(int id, Node *d, double w, string n){
	this->id = id;
	this->dest = d;
	this->weight = w;
	this->name = n;
}


void Aresta::changeName(string n){
	this->name = n;
}


Node* Aresta::getDest(){
	return this->dest;
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

			gv->addNode(id, x * 5 + 50, -(y * 5) + 600);

			if (this->getVertexSet().at(i)->getBus()) {

				gv->setVertexIcon(id, "bus.png");
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





