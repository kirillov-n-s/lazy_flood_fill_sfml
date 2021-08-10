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

	/*bool _paused = true;*/
	uint64_t _time;
	void update(int count);
	void handle_events();
	void render();

public:
	application(floodfiller* floodfiller, const std::string& title = "", uint32_t dimension = 10u);
	~application();

	void run();
};
