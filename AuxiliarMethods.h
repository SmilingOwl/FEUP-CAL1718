#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Graph.h"
using namespace std;

string NodesFileName;
string EdgesFileName;
string StreetsFileName;

class AuxiliarMethods {
public:
	double getDistance(double lat1, double lon1, double lat2, double lon2);
	double getTime(double distance, double velocity);
	double deg2rad(double deg);

	bool openFile(ifstream &file, const string fileName);
	template<class T>
	void extractDataNodes(T &g);
	template<class T>
	void extractDataArestas(T &g);
	template<class T>
	void extractDataStreets(T &g);
	template<class T>
	bool generateBusLines(T &g, int numberOfNodes);
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

template<class T>
void AuxiliarMethods::extractDataNodes(T &g) {

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

		g.addNode(ID, latitude, longitude);
	}
	file.close();
}

template<class T>
void AuxiliarMethods::extractDataArestas(T &g) {
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
					g.addEdge(idOrigin, idFinal, 0);
					g.addEdge(idFinal, idOrigin, 0);
				} else {
					g.addEdge(idOrigin, idFinal, 0);
				}

				//TODO por o nome da rua

				break;
			}
		}


	}
	file.close();
}

template<class T>
void AuxiliarMethods::extractDataStreets(T &g) {
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

template<class T>
bool AuxiliarMethods::generateBusLines(T &g, int numberOfNodes){
	if (numberOfNodes <= 1 || numberOfNodes>= g->VertexSet.size()) return false;

	Vertex<T>* initialVertex = g->getRandomVertex();
	Vertex<T>* initialBus = initialVertex->createBusVertex();

	for (unsigned int i = 1; i < numberOfNodes; i++){
		//TODO
	}


}

