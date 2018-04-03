#pragma once

#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Graph.h"
using namespace std;

string NodesFileName;
string EdgesFileName;
string StreetsFileName;

const double INTERCHANGE = 2;

class AuxiliarMethods {
public:
	double getDistance(double lat1, double lon1, double lat2, double lon2);
	double getTime(double distance, double velocity);
	double deg2rad(double deg);
	double getDistanceVertex(Node* n1, Node* n2);

	bool openFile(ifstream &file, const string fileName);

	void extractDataNodes(Graph &g);

	void extractDataArestas(Graph &g);

	void extractDataStreets(Graph &g);

	bool generateBusLines(Graph &g, int numberOfNodes);
};

//Haversine Formula (kilometers)
double AuxiliarMethods::getDistance(double lat1, double lon1, double lat2,
		double lon2) {
	double R = 6371; // Radius of the earth in km
	double dLat = deg2rad(lat2 - lat1);
	double dLon = deg2rad(lon2 - lon1);
	double a = sin(dLat / 2) * sin(dLat / 2)
			+ cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * sin(dLon / 2)
					* sin(dLon / 2);
	double c = 2 * atan2(sqrt(a), sqrt(1 - a));
	double d = R * c; // Distance in km
	return d;
}

double AuxiliarMethods::getDistanceVertex(Node* n1, Node* n2){
	return getDistance(n1->getLat(),n1->getLon(),n2->getLat(),n2->getLon());
}

double AuxiliarMethods::getTime(double distance, double velocity) {
	return distance / velocity;
}

double AuxiliarMethods::deg2rad(double deg) {
	return deg * (M_PI / 180);
}

bool AuxiliarMethods::openFile(ifstream &file, const string fileName) {
	file.open(fileName);

	if (!file)
		cerr << "Error" << endl;
	else
		return true;
}


void AuxiliarMethods::extractDataNodes(Graph &g) {

	ifstream file;
	openFile(file, NodesFileName);

	string line;

	int ID = 0;
	double latitude;
	double longitude;
	string string;

	while (getline(file, line)) {
		stringstream lineSs(line);

		//ID
		lineSs >> ID;
		getline(lineSs, string, ';');
		//Latitude
		lineSs >> latitude;
		getline(lineSs, string, ';');
		//longitude
		lineSs >> longitude;

		g.addVertex(ID, latitude, longitude);
	}
	file.close();
}


void AuxiliarMethods::extractDataArestas(Graph &g) {
	ifstream file;
	openFile(file, EdgesFileName);

	string line;

	int idEdge = 0;
	int idOrigin = 0;
	int idFinal = 0;

	while (getline(file, line)) {
		stringstream lineSs(line);
		string string;
		//ID
		lineSs >> idEdge;
		getline(lineSs, string, ';');
		//Origem
		lineSs >> idOrigin;
		getline(lineSs, string, ';');
		//Destino
		lineSs >> idFinal;

		for(int i = 0; i< g.edgeC.size(); i++){
			if (g.edgeC.at(i) == idEdge){
				if(g.twoWay.at(i)){
					g.addEdge(idEdge,idOrigin, idFinal, 0);
					g.addEdge(idEdge*123,idFinal, idOrigin, 0);
				} else {
					g.addEdge(idEdge,idOrigin, idFinal, 0);
				}



				break;
			}
		}


	}
	file.close();
}


void AuxiliarMethods::extractDataStreets(Graph &g) {
	ifstream file;
	openFile(file, StreetsFileName);
	string line;

	int idEdge = 0;
	string nameOfStreet;
	bool twoWays = false;

	while(getline(file,line))
	{
		stringstream lineSs(line);
		string string;
		//ID
		lineSs >> idEdge;
		getline(lineSs, string, ';');
		//nome da rua
		lineSs >> nameOfStreet;
		getline(lineSs, string, ';');
		//direção
		lineSs >> twoWays;

		g.edgeC.push_back(idEdge);
		g.nameC.push_back(nameOfStreet);
		g.twoWay.push_back(twoWays);



	}

}


bool AuxiliarMethods::generateBusLines(Graph &g, int numberOfNodes){
	if (numberOfNodes <= 1 || numberOfNodes>= g.getVertexSet().size()) return false;

	Node* initialVertex = g.getRandomVertex();
	Node* initialBusVertex = initialVertex->createBusVertex();
	Node* nextVertex;
	Node* nextBusVertex;


	g.addVertex(initialBusVertex);
	g.addEdge(rand()%10000000,initialBusVertex->getID(),initialVertex->getID(),INTERCHANGE);
	g.addEdge(rand()%10000000,initialVertex->getID(),initialBusVertex->getID(),INTERCHANGE);
	for (unsigned int i = 1; i < numberOfNodes; i++){

		nextVertex = initialVertex->getRandomVertexDestination();
		nextBusVertex =  nextVertex->createBusVertex();

		g.addVertex(nextBusVertex);
		g.addEdge(rand()%10000000,nextVertex->getID(), nextBusVertex->getID(),INTERCHANGE);
		g.addEdge(rand()%10000000,nextBusVertex->getID(), nextVertex->getID(),INTERCHANGE);

		g.addEdge(rand()%10000000,initialBusVertex->getID(),nextVertex->getID(),getDistanceVertex(initialBusVertex,nextVertex));
		g.addEdge(rand()%10000000+1,initialVertex->getID(),nextBusVertex->getID(),getDistanceVertex(initialBusVertex,nextVertex));

		initialVertex = nextVertex;
		initialBusVertex = nextBusVertex;


	}


}

