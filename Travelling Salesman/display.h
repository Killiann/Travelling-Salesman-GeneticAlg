#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <string>

struct displayValue {	
	sf::Text name;
	sf::Text value;
	sf::Vector2f position;

	displayValue(std::string n, std::string val, sf::Font *f, sf::Vector2f pos) : position(pos) {
		name.setFont(*f);
		name.setPosition(pos);
		name.setString(n);
		name.setFillColor(sf::Color::Black);
		name.setCharacterSize(14);

		value.setFont(*f);
		value.setPosition(sf::Vector2f(pos.x + name.getLocalBounds().width, pos.y));
		value.setString(val);
		value.setFillColor(sf::Color::Black);
		value.setCharacterSize(14);
	}
	
	void updateValue(std::string val) { value.setString(val); }
	void draw(sf::RenderWindow& w) { w.draw(name); w.draw(value); }
	float getHeight() { return name.getLocalBounds().height; }
	float getWidth() { return name.getLocalBounds().width; }
};

class display
{	
	std::vector<displayValue> content;
	sf::Vector2f position; 
	sf::Vector2f size;	
	sf::Font* font;
public:
	display(sf::Vector2f pos, sf::Vector2f s, sf::Font *f);
	void AddValuePair(std::string name, std::string value);
	void UpdateValue(int idx, std::string value);
	void Draw(sf::RenderWindow& w);
};

