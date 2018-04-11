

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

	ifstream file(FILE_A);

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
	ifstream file(FILE_C);

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

					Node* nodeOriginal = graph->findVertex(idIntOri);
					Node* nodeFinal = graph->findVertex(idIntDest);

					graph->addEdge(id,idIntOri, idIntDest,graph->getDistanceVertex(nodeOriginal, nodeFinal),vehicle,edges.at(i).nome);
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



						id++;

						graph->addEdge(id,idIntDest, idIntOri,graph->getDistanceVertex(nodeOriginal, nodeFinal),vehicle,edges.at(i).nome);
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
				break;
			}
		}
		id++;


	}
	file.close();

	return;

}

void loadStreets(vector<aresta> &edges){
	ifstream file(FILE_B);
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

bool checkIfNodeExists(unsigned long long id){
	for (unsigned int i = 0; i < graph->getVertexSet().size(); i++){
		if(graph->getVertexSet().at(i)->getID()==id){
			return true;
		}
	}
	return false;
}

void menu()
{

	cout<<"  _____     _       ____  _                             "<<endl;
	cout<<" |_   _| __(_)_ __ |  _ \\| | __ _ _ __  _ __   ___ _ __ "<<endl;
	cout<<"   | || '__| | '_ \\| |_) | |/ _` | '_ \\| '_ \\ / _ \\ '__|"<<endl;
	cout<<"   | || |  | | |_) |  __/| | (_| | | | | | | |  __/ |   "<<endl;
	cout<<"   |_||_|  |_| .__/|_|   |_|\\__,_|_| |_|_| |_|\\___|_|   "<<endl;
	cout<<"             |_|                                        "<<endl;


	cout<< "Welcome! We will help to find your favorite way to get your destiny!"<<endl;

	cout<<"Where are you?"<<endl;
	cout<<"Please choose an ID"<<endl;

	unsigned long long idOrigem;
	cin>>idOrigem;
	while (!checkIfNodeExists(idOrigem)){
		cout<<"Please choose a valid ID"<<endl;
		cin >>idOrigem;
	}



	cout<<"Where do you want to go?"<<endl;
	cout<<"Please choose an ID"<<endl;

	unsigned long long idDestino;
	cin>>idDestino;
	while (!checkIfNodeExists(idDestino)){
		cout<<"Please choose a valid ID"<<endl;
		cin >>idDestino;
	}



	cout<<"Choose your preferences!"<<endl;


	cout << "+------------------------------------------+" << endl;
	cout << "|   1. Fastest Path                        |" << endl;
	cout << "+------------------------------------------+" << endl;
	cout << "|   2. Shortest Path                       |" << endl;
	cout << "+------------------------------------------+" << endl;
	cout << "|   3. Bus Preferably                      |" << endl;
	cout << "+------------------------------------------+" << endl;
	cout << "|   4. Metro Preferably                    |" << endl;
	cout << "+------------------------------------------+" << endl;

	int option;
	cout<<"option: ";
	cin>>option;

	switch(option)
	{
	case 1:
		graph->printShortest(idOrigem, idDestino);
		break;

	case 2:
		//caminho mais curto em km utilizando apenas autocarros,
		//se não existir autocarros ir a pé.
		//mostar também o custo total da viagem e o tempo que demora
		graph->printFastest(idOrigem, idDestino);
		break;

	case 3:
		//caminho mais curto em km utilizando apenas metro,
		//se não existir autocarros ir a pé.
		//mostar também o custo total da viagem e o tempo que demora
		break;

	case 4:
		int option2;
		cout<<"What do you prefer?"<<endl;

		cout << "+------------------------------------------+" << endl;
		cout << "|   1. Shortest path                       |" << endl;
		cout << "+------------------------------------------+" << endl;
		cout << "|   2. Fastest path                        |" << endl;
		cout << "+------------------------------------------+" << endl;
		cout << "|   3. Cheapest path                       |" << endl;
		cout << "+------------------------------------------+" << endl;

		cout<<"option: ";
		cin>> option2;

		switch(option2)
		{
		case 1:
			//caminho mais curto combinando a pé, autocarro e metro
			//mostrar distancia, custo e tempo
			break;
		case 2:
			//caminho mais rápido combinando a pé, autocarro e metro
			//mostrar distancia, custo e tempo
			break;
		case 3:
			//caminho mais barato combinando a pé, autocarro e metro
			//mostrar distancia, custo e tempo
			break;
		}
		break;
	default:
		break;
	}
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
	//printf("nodes loaded!");
	loadStreets(edges);
	//printf("streets done!");

	loadEdges(nodes,edges);
	//printf("edges done!");

	vector<int> buses = {10, 15, 9,10,10};
	vector<int> metro = {5, 3, 4,5,5};

	//graph->generateBusLines(buses,nodes);
	//graph->generateMetroLines(metro,nodes);



















}

int main() {
	srand(time(NULL));
	//exercicio1();
	//exercicio2();
	//exercicio3();
	teste();
	menu();

	getchar();
	return 0;
}
