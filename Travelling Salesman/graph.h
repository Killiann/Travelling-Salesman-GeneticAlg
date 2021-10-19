#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

struct point {
	sf::Vector2f pos;
	float val;
	sf::CircleShape shape;
	point(sf::Vector2f pos, float val) : pos(pos), val(val) {}
	inline void SetPosition(sf::Vector2f p) { pos = p; }
};

class label {
	sf::Vector2f position;
	std::string lbl;
	sf::RectangleShape drawable;
	sf::Font *f;
	sf::Text text;
	sf::Color c = sf::Color(90, 90, 90);
	bool vertical = false;

public:
	label(sf::Vector2f pos, bool vertical, std::string l, sf::Font *font) : position(pos), lbl(l), f(font), vertical(vertical){
		text.setString(l);
		text.setFont(*f);
		text.setCharacterSize(14);
		text.setFillColor(c);
		drawable.setFillColor(c);
		
		if (vertical) {			
			text.setPosition(sf::Vector2f(position.x - text.getLocalBounds().width - 4, position.y - 8));						
			drawable.setPosition(pos);
			drawable.setSize(sf::Vector2f(5, 2));			
		}
		else {
			text.setPosition(sf::Vector2f(position.x - (text.getLocalBounds().width / 2), position.y + 2));
			drawable.setPosition(sf::Vector2f(position.x, position.y - 5));
			drawable.setSize(sf::Vector2f(2, 5));
		}
	}
	void setPosition(sf::Vector2f newPos) { position = newPos; }
	void setText(std::string t) { 
		text.setString(t);
		if (vertical) text.setPosition(sf::Vector2f(position.x - text.getLocalBounds().width - 4, position.y - 8));
		else text.setPosition(sf::Vector2f(position.x - (text.getLocalBounds().width / 2), position.y + 2));
	}
	void draw(sf::RenderWindow& window) { window.draw(drawable); window.draw(text); }
};

class graph
{
	float minX = 0, maxX = 0, minY = 0, maxY = 0;
	int width = 400, height = 400;
	int padding = 30, lineWidth = 2;	
	
	sf::Vector2f pos;

	sf::RectangleShape bottomBar;
	sf::RectangleShape leftBar;

	sf::Font *font;
	sf::Text xAxisText, yAxisText;

	std::vector<std::string> leftMarkers;
	std::vector<std::string> bottomMarkers;

	sf::VertexArray line;
	
	std::vector<point> points;
	std::vector<label> labels;

public:
	graph(sf::Vector2f position, sf::Vector2f size, std::string xAx, std::string yAx, sf::Font *f);
	void AddPoint(float val);
	void Clear();
	void Draw(sf::RenderWindow& window);
};

