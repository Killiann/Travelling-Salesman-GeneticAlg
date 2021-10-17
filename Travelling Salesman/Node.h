#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "linearAlgebra.h"

class Node
{
	int ID;
	sf::Vector2f location;
	sf::CircleShape drawable;
	const float radius = 10.f;
	const sf::Color color = sf::Color::Green;	

public:
	Node(sf::Vector2f loc, int id) :location(loc), ID(id) {
		drawable.setFillColor(color);
		drawable.setPosition(loc);
		drawable.setOrigin(radius, radius);
		drawable.setPosition(loc);
		drawable.setRadius(radius);
	};
	Node(){}
	inline int GetID() { return ID; }
	inline sf::Vector2f GetPosition() { return location; }
	inline void Draw(sf::RenderWindow &w) { w.draw(drawable); };

	inline void operator=(Node n) { 
		ID = n.ID; 
		location = n.location;
		drawable = n.drawable;
	};

	inline bool operator==(Node n) {
		return ID == n.ID;
	}
};

class Edge {
	Node node1, node2;
	float weight = 0.f;
	float distance;

	lin::Line line;
	float lineWidth = 2.f;
	sf::Color color = sf::Color::Blue;

public:
	Edge(Node n1, Node n2) : node1(n1), node2(n2) {
		line = lin::Line(n1.GetPosition(), n2.GetPosition(), lineWidth, color);
		distance = line.GetLength();
	};	
	bool Includes(Node n) { return (node1.GetID() == n.GetID() || node2.GetID() == n.GetID()); }
	void Draw(sf::RenderWindow &w) { line.Draw(w); };
	inline bool DoesConnect(Node n1, Node n2) {
		if ((node1.GetID() == n1.GetID() && node2.GetID() == n2.GetID())
			|| (node1.GetID() == n2.GetID() && node2.GetID() == n1.GetID()))
			return true;
		return false;
	}
	inline float GetDistance() { return distance; }
	inline void SetColor(sf::Color c) { color = c; line = lin::Line(node1.GetPosition(), node2.GetPosition(), lineWidth, color); }
	inline void SetWidth(float width) { lineWidth = width; line = lin::Line(node1.GetPosition(), node2.GetPosition(), lineWidth, color); }
	inline void operator=(Edge e2){
		node1 = e2.node1;
		node2 = e2.node2;
		weight = e2.weight;
		distance = e2.distance;
		line = e2.line;
		lineWidth = e2.lineWidth;
		color = e2.color;
	}
};