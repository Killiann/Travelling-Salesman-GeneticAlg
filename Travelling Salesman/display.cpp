#include "display.h"

display::display(sf::Vector2f pos, sf::Vector2f s, sf::Font *f): position(pos), size(s), font(f){
}

void display::AddValuePair(std::string name, std::string value) {
	displayValue p(name, value, font, sf::Vector2f(position.x, position.y + content.size() * 20));
	content.push_back(p);
}

void display::UpdateValue(int idx, std::string val) {
	if (content.size() > idx) {
		content[idx].updateValue(val);
	}
}

void display::Draw(sf::RenderWindow& w) {
	for (auto& c : content) {
		c.draw(w);
	}
}