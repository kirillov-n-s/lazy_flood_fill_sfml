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

	const uint32_t _tile_dim_base = 10u;
	uint32_t _tile_dim;
	sf::RectangleShape _tile;
	void draw(uint32_t x, uint32_t y);

	const double _decay_base = 0.9995;
	double _decay = _decay_base;
	const double _delta_base = 0.00005;
	double _delta = _delta_base;

	bool _gen = false;
	uint32_t _iters = 0;
	int _mod = 1;
	std::minstd_rand _engine = std::minstd_rand(std::random_device()());

	bool _centered = false;
	sf::Vector2u _center;
	std::uniform_int_distribution<uint32_t> _uni_x;
	std::uniform_int_distribution<uint32_t> _uni_y;
	std::poisson_distribution<uint32_t> _poisson_x;
	std::poisson_distribution<uint32_t> _poisson_y;
	std::uniform_int_distribution<int> _dx;
	std::uniform_int_distribution<int> _dy;
	const double _scatter_base = 0.35;
	double _scatter = _scatter_base;

	void update();
	void handle_events();
	void render();

public:
	application(floodfiller* floodfiller, const std::string& title = "", uint32_t dimension = 0u);
	~application();

	void run();
};
