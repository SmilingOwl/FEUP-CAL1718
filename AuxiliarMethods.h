#pragma once

#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

string NodesFileName;
string EdgesFileName;
string StreetsFileName;


//DEClARATION
	double getDistance(double lat1, double lon1, double lat2, double lon2);
	double deg2rad(double deg);
	bool openFile(ifstream &file, const string fileName);

//METHODS


/**
*Haversine Formula (kilometers)
*This function returns the distance between the two coordinates given as parameters
*/
double getDistance(double lat1, double lon1, double lat2,
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

/**
* this function converts degrees to radians
*/
double deg2rad(double deg) {
	return deg * (M_PI / 180);
}

/**
*this function checks if it is possible to open a file, returning true if possible and false otherwise
*/
bool openFile(string fileName) {
	ifstream file;
	file.open(fileName);

	if (!file){

		return false;
	}
	else {
		file.close();
		return true;
	}
}



