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
	double _delta = 0.000025;

	bool _animate = false;
	uint32_t _iters = 0;
	uint32_t _min = 200;
	uint32_t _max = 300;
	std::minstd_rand _engine = std::minstd_rand(std::random_device()());

	void handle_events();
	void render();

public:
	application(floodfiller* floodfiller, const std::string& title = "", uint32_t dimension = 10u);
	~application();

	void run();
};
