#include "Graph.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

string NodesFileName;

class AuxiliarMethods {
public:
	double getDistance(double lat1, double lon1, double lat2, double lon2);
	double getTime(double distance, double velocity);
	double deg2rad(double deg);
	bool openFile(ifstream &file, const string fileName);
	template <class T>
	void extractDataNodes(T &g);
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

template <class T>
void AuxiliarMethods::extractDataNodes(T &g) {

	ifstream file;
	openFile(file, NodesFileName);

	string line;

	int ID;
	double latitude;
	double longitude;


		while (getline(file, line)) {
			//ID
			getline(ssA, ID, ';');
			//Latitude
			getline(ssA, latitude, ';');
			//longitude
			getline(ssA, longitude, ';');

			Vertex temV = new Vertex(ID, latitude, longitude);
			g.addVertex(tempV);
		}

	}

}
*/
