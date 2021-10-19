#include "graph.h"

graph::graph(sf::Vector2f position, sf::Vector2f size, std::string xAx, std::string yAx, sf::Font *f) : font(f) {
	bottomBar.setPosition(sf::Vector2f(position.x + padding, position.y + size.y - padding));
	bottomBar.setSize(sf::Vector2f(size.x - (padding * 2), lineWidth));
	bottomBar.setFillColor(sf::Color::Black);

	leftBar.setPosition(sf::Vector2f(position.x + padding, position.y + padding));
	leftBar.setSize(sf::Vector2f(lineWidth, size.y - (padding * 2)));
	leftBar.setFillColor(sf::Color::Black);	

	width = size.x;
	height = size.y;

	pos = position;	

	labels.emplace_back(sf::Vector2f(position.x + padding, position.y + padding), true, "top", font);
	labels.emplace_back(sf::Vector2f(position.x + padding, position.y + height - (padding * 2)), true, "bottom", font);
	labels.emplace_back(sf::Vector2f(position.x + padding, position.y + size.y - padding), false, "0", font);
	labels.emplace_back(sf::Vector2f(position.x + size.x - padding, position.y +size.y - padding), false, "total", font);

	xAxisText.setString(xAx);
	yAxisText.setString(yAx);

	xAxisText.setFont(*font);
	yAxisText.setFont(*font);

	xAxisText.setCharacterSize(14);
	yAxisText.setCharacterSize(14);

	xAxisText.setFillColor(sf::Color::Black);
	yAxisText.setFillColor(sf::Color::Black);

	yAxisText.rotate(90.f);

	xAxisText.setOrigin(sf::Vector2f(xAxisText.getLocalBounds().width / 2, xAxisText.getLocalBounds().height / 2));
	yAxisText.setOrigin(sf::Vector2f(yAxisText.getLocalBounds().width / 2, yAxisText.getLocalBounds().height / 2));

	xAxisText.setPosition(sf::Vector2f(position.x + (size.x / 2), position.y + size.y));
	yAxisText.setPosition(sf::Vector2f(position.x, position.y + (size.y / 2)));		
}

void graph::AddPoint(float val) {	
	if (points.size() == 0) {
		minY = maxY = val;
		minY -= 1;
		labels[0].setText(std::to_string(maxY));
		labels[1].setText(std::to_string(minY));
	}
	//reset positions	
	if (maxY < val) {
		maxY = val;
		labels[0].setText(std::to_string(maxY));
	}
	if (minY > val) {
		minY = val;
		labels[1].setText(std::to_string(minY));
	}
	point p(sf::Vector2f(0, 0), val);
	points.push_back(p);

	sf::VertexArray x(sf::LineStrip, points.size());

	for (int i = 0; i < points.size(); ++i) {
		sf::Vector2f ps;
		ps.x = pos.x + padding + ((float)i * ((float)(width - (padding * 2)) / (float)points.size()));
		ps.y = pos.y + height - (padding * 2);
		if(points[i].val != minY)
		ps.y -= ((height - (padding * 3)) / (maxY - minY)) * (points[i].val - minY);		
		points[i].SetPosition(ps);
		
		x[i].color = sf::Color::Magenta;
		x[i].position = ps;
	}

	labels[3].setText(std::to_string(points.size()));
	line = x;
}

void graph::Clear() {
	points.clear();
}

void graph::Draw(sf::RenderWindow& window) {
	window.draw(bottomBar);
	window.draw(leftBar);
	window.draw(xAxisText);
	window.draw(yAxisText);
	for (auto& l : labels) {
		l.draw(window);
	}
	window.draw(line);
}