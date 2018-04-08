

#include <iostream>
#include "Graph.h"
#include "AuxiliarMethods.h"
#include <cstdio>
#include "graphviewer.h"
#include <fstream>
#include <iostream>
#include "Visualizer.h"
#include <sstream>

const float MAX_LAT = 41.186;
const float MIN_LAT = 41.13921;
const float MAX_LON = -8.57601;
const float MIN_LON = -8.65271;
const int IMAGE_X = 2000;
const int IMAGE_Y = 2464;

void teste();


Graph* graph = new Graph();

GraphViewer *gv;


struct aresta {
	unsigned long long id;
	string nome;
	bool is2Way;
	aresta(unsigned long long i, string n, bool is2)
	{
		id = i;
		nome = n;
		is2Way = is2;
	}
};


void loadNodes(vector<pair<int,unsigned long long>> &nodes) {
	string line;

	ifstream file("aSmall.txt");

	int i = 1;

	if (file.is_open()) {
		while (!file.eof()) {


			getline(file,line);

			std::stringstream linestream(line);
			string data;

			unsigned long long id;

			double lat;
			double lon;
			string name;
			int vehicle;

			linestream >> id;

			std::getline(linestream, data, ';');
			linestream >> lat;
			std::getline(linestream, data, ';');
			linestream >> lon;
			std::getline(linestream, data, ';');
			linestream >> vehicle;

			nodes.push_back(make_pair(i,id));

			graph->addVertex(i,lat,lon,vehicle);
			double x = ( (lon - MIN_LON) * (IMAGE_Y) ) / (MAX_LON - MIN_LON);
			double y = ((lat - MIN_LAT) * (IMAGE_X)) / (MAX_LAT - MIN_LAT);
			gv->addNode(i , x,-y);
			if (vehicle == 0){
				gv->setVertexIcon(i, "emptyIcon.png");
			} else if (vehicle == 1){
				gv->setVertexIcon(i, "bus.png");
			} else if (vehicle == 2){
				gv->setVertexIcon(i, "metro.png");
			}
			i++;
		}

		file.close();

		return;
	} else {
		cerr << "\n File not found!\n";
	}
}


void loadEdges(const vector<pair<int,unsigned long long>> &nodes, const vector<aresta> &edges){
	ifstream file("cSmall.txt");

	string line;

	unsigned long long idEdge = 0;
	unsigned long long idOrigin = 0;
	unsigned long long idFinal = 0;
	int id = 1;
	int vehicle = 0;
	if(file.fail())
	{
		cerr << "\nFile not found!\n";
	}
	while (!file.eof())
	{

		getline(file, line);

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
		getline(lineSs, string, ';');
		//vehicle
		lineSs >> vehicle;

		for(unsigned int i = 0; i < edges.size(); i++)
		{

			if(edges.at(i).id == idEdge)
			{
				unsigned long long idIntOri = 0;
				unsigned long long idIntDest = 0;

				for(auto n : nodes)
				{
					if(n.second == idOrigin)
					{
						idIntOri = n.first;
						if(idIntDest != 0)
							break;
					}
					if(n.second == idFinal)
					{
						idIntDest = n.first;
						if(idIntOri != 0)
							break;
					}
				}


				if(idIntOri != 0 && idIntDest != 0)
				{
					graph->addEdge(id,idIntOri, idIntDest,0,0);
					gv->addEdge(id,idIntOri,idIntDest,EdgeType::DIRECTED);
					if (vehicle == 0){
						gv->setEdgeColor(id,RED);
					} else if (vehicle ==  1){
						gv->setEdgeColor(id,BLUE);
					} else if (vehicle == 2){
						gv->setEdgeColor(id,GREEN);
					}
					if (!(edges.at(i).nome == "MUDANCA DE TRANSPORTE")){
						gv->setEdgeLabel(id,edges.at(i).nome);
					} else {
						gv->setEdgeLabel(id,"");
					}


					if(edges.at(i).is2Way){
						id++;
						graph->addEdge(id,idIntDest, idIntOri,0,0);
						gv->addEdge(id,idIntDest,idIntOri,EdgeType::DIRECTED);
						if (vehicle == 0){
							gv->setEdgeColor(id,RED);
						} else if (vehicle ==  1){
							gv->setEdgeColor(id,BLUE);
						} else if (vehicle == 2){
							gv->setEdgeColor(id,GREEN);
						}
						if (!(edges.at(i).nome == "MUDANCA DE TRANSPORTE")){
							gv->setEdgeLabel(id,edges.at(i).nome);
						} else {
							gv->setEdgeLabel(id,"");
						}
					}
				}
				break;
			}
		}
		id++;


	}
	file.close();

	return;

}

void loadStreets(vector<aresta> &edges){
	ifstream file("bSmall.txt");
	string line;

	unsigned long long idEdge = 0;
	string nameOfStreet;
	string twoWaysString;
	bool twoWays = false;

	if(file.fail())
	{
		cerr << "\nFile not Found!\n";
	}
	while(!file.eof())
	{
		string idd;
		getline(file,idd,';');

		if(idd != "/r/n" && idd != ""){
			idEdge = stoul(idd);

			getline(file,nameOfStreet,';');
			getline(file,twoWaysString);
			if(twoWaysString == "True"){
				twoWays = true;
			} else {
				twoWays = false;
			}

			edges.push_back(aresta(idEdge,nameOfStreet,twoWays));
		}
	}
	file.close();

	return;

}

void teste(){
	gv = new GraphViewer(500,500,false);
	gv->createWindow(800, 800);

	gv->defineEdgeCurved(false);
	gv->defineEdgeColor("black");
	//gv->defineVertexIcon("emptyIcon.png");

	vector<pair<int,unsigned long long>> nodes;
	vector<aresta> edges;

	loadNodes(nodes);
	printf("nodes loaded!");
	loadStreets(edges);
	printf("streets done!");

	loadEdges(nodes,edges);
	printf("edges done!");
	/*
	graph->generateBusLines(14);
	graph->generateBusLines(10);
	graph->generateMetroLine(5);
	*/



















}

int main() {
	//exercicio1();
	//exercicio2();
	//exercicio3();
	teste();
	getchar();
	return 0;
}
