#include "NodeManager.h"

void Route::BuildRoute() {
	float sum = 0;
	drawable.clear();
	for (int i = 0; i < nodes.size() - 1; ++i) {
		Edge e = Edge(nodes[i], nodes[i + 1]);
		e.SetColor(sf::Color::Red);
		e.SetWidth(7.f);
		drawable.push_back(e);

		sum += e.GetDistance();
	}
	distance = sum;
}

void Route::AddNode(Node n) {
	nodes.push_back(n);
	BuildRoute();
}

void Route::operator=(Route r2){
	nodes = r2.nodes;
	drawable = r2.drawable;
	distance = r2.distance;
}