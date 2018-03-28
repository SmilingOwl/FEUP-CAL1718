#include "Graph.h"
#include <cmath>


using namespace std;

class AuxiliarMethods{
public:
	double getDistance(double lat1, double lon1,double lat2,double lon2);
	double getTime(double distance, double velocity);
	double deg2rad(double deg);
	template<class T>
	void extractData(string fileName, T &g);
};




//Haversine Formula (kilometers)
double AuxiliarMethods::getDistance(double lat1, double lon1,double lat2,double lon2) {
  double R = 6371; // Radius of the earth in km
  double dLat = deg2rad(lat2-lat1);
  double dLon = deg2rad(lon2-lon1);
  double a = sin(dLat/2) * sin(dLat/2) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * sin(dLon/2) * sin(dLon/2);
  double c = 2 * atan2(sqrt(a), sqrt(1-a));
  double d = R * c; // Distance in km
  return d;
}

double AuxiliarMethods::getTime(double distance, double velocity){
	return distance/velocity;
}

double AuxiliarMethods::deg2rad(double deg) {
  return deg * (M_PI/180);
}


/*

template<class T>
void AuxiliarMethods::extractData(string fileName, T &g)
{
	stringstream ssA;
	ifstream file(fileName);
	int ID;
	double latitude;
	double longitude;

	if(file.is_open())
	{
		string line;

		while(getline(file,line))
		{
			//ID
			getline(ssA,ID,';');
			//Latitude
			getline(ssA,latitude,';');
			//longitude
			getline(ssA,longitude,';');

			Vertex temV=new Vertex(ID,latitude,longitude);
			g.addVertex(tempV);

		}
	}
}
*/
