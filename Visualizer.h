#include <iostream>
#include "Graph.h"
#include "AuxiliarMethods.h"
#include <cstdio>
#include "graphviewer.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

class Visualizer{
	GraphViewer *gv;


public:
	Visualizer();

	void printView(Graph &g);
};



Visualizer::Visualizer(){
	gv = new GraphViewer(1000, 1000, false);
}


void Visualizer::printView(Graph &g){
	gv->createWindow(800, 800);

	gv->defineEdgeCurved(false);

	gv->defineEdgeColor("black");
	gv->defineVertexIcon("emptyIcon.png");

	for (unsigned int i = 0; i < g.getVertexSet().size(); i++) {

		int id = g.getVertexSet().at(i)->getID();
		int x = g.getVertexSet().at(i)->getLat();
		int y = g.getVertexSet().at(i)->getLon();

		gv->addNode(id, x * 5 + 50, -(y * 5) + 600);

		if (g.getVertexSet().at(i)->getBus()) {

			gv->setVertexIcon(id, "bus.png");
		}



	}

	for (unsigned int i = 0; i < g.getVertexSet().size(); i++) {

		int idOrigem = g.getVertexSet().at(i)->getID();

		vector<Aresta> adj = g.getVertexSet().at(i)->getAdj();

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



