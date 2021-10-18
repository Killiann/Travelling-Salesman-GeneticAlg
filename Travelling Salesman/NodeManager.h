#pragma once
#include <vector>
#include "Node.h"

class Route {
	std::vector<Node> nodes;
	std::vector<Edge> drawable;

	float distance = 0.f;

public:
	Route() {};
	Route(std::vector<Node> ns) :nodes(ns) { BuildRoute(); };

	void BuildRoute();
	void AddNode(Node n);
	void Draw(sf::RenderWindow& window) { for (auto& e : drawable) e.Draw(window); }

	void operator=(Route r2);
	inline float GetDistance() { return distance; }

	inline std::vector<Node> GetNodes() {
	
		std::vector<Node> n = nodes;
		return n; 
	}
	inline void SetNodes(std::vector<Node> n) { nodes = n; }
};

class NodeManager
{
	
};


