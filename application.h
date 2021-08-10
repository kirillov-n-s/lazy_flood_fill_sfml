#pragma once
#include <chrono>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "floodfiller.h"

class application
{
	floodfiller* _floodfiller;

	std::string _title;
	sf::RenderWindow* _window;

	uint32_t _tile_dim;
	sf::RectangleShape _tile;
	void draw(uint32_t x, uint32_t y);

	const double _decay_base = 0.9995;
	double _decay = _decay_base;
	double _delta = 0.00005;

	bool _animate = false;
	void animate(double delta = 0.00001);

	void handle_events();
	void render();

public:
	application(floodfiller* floodfiller, const std::string& title = "", uint32_t dimension = 10u);
	~application();

	void run();
};
