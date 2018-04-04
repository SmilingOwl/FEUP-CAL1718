#pragma once

#include <iostream>
#include "Graph.h"
#include "AuxiliarMethods.h"
#include <cstdio>
#include "graphviewer.h"
#include <fstream>
#include <iostream>
#include "Visualizer.h"
#include <sstream>



void teste();
void loadNodes();
void loadEdges();
void loadStreets();
Graph* graph = new Graph();

void loadNodes() {
	string line;

	ifstream file("a.txt");

	int i = 0;

	if (file.is_open()) {
		while (getline(file, line)) {

			std::stringstream linestream(line);
			string data;

			int id;
			double lat;
			double lon;
			string name;

			linestream >> id;

			std::getline(linestream, data, ';');
			linestream >> lat;
			std::getline(linestream, data, ';');
			linestream >> lon;

			graph->addVertex(id,lat,lon);

			printf("Nodes %d \n",i);
			i++;
		}

		file.close();
		return;
	} else {
		cerr << "n File not found!\n";
	}

}

void loadEdges(){
	ifstream file("c.txt");

	string line;

	int idEdge = 0;
	int idOrigin = 0;
	int idFinal = 0;
	int i = 0;

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


		for(int i = 0; i< graph->edgeC.size(); i++){
			if (graph->edgeC.at(i) == idEdge){
				if(graph->twoWay.at(i)){
					graph->addEdge(idEdge,idOrigin, idFinal, 0,0);
					graph->addEdge(idEdge*123,idFinal, idOrigin, 0,0);
				} else {
					graph->addEdge(idEdge,idOrigin, idFinal, 0,0);
				}



				break;
			}
		}

		printf("Edges %d \n",i);
		i++;


	}
	file.close();

	return;

}

void loadStreets(){
	ifstream file("b.txt");
	string line;

	int idEdge = 0;
	string nameOfStreet;
	string twoWaysString;
	bool twoWays = false;
	int i = 0;

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
		lineSs >> twoWaysString;

		if(twoWaysString == "True"){
			twoWays = true;
		} else {
			twoWays = false;
		}

		graph->edgeC.push_back(idEdge);
		graph->nameC.push_back(nameOfStreet);
		graph->twoWay.push_back(twoWays);

		printf("Streets %d \n",i);
		i++;


	}
	file.close();

	return;

}









void teste(){

	loadStreets();
	printf("streets done!");
	loadNodes();
	printf("nodes done!");
	loadEdges();
	printf("edges done!");


	graph->printView();


}

int main() {
	//exercicio1();
	//exercicio2();
	//exercicio3();
	teste();
	getchar();
	return 0;
}
