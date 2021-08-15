#pragma once
#include <chrono>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "tilemap.h"

class application
{
	tilemap* _tilemap;

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
	uint32_t _iters_init = 0;
	int _dir = 1;
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
	void set_up_distribution();

	sf::Font _font;
	const uint32_t _font_size = 30u;
	const sf::Vector2f _offset_init = sf::Vector2f(8.f, 4.f);
	const sf::Vector2f _offset = sf::Vector2f(0.f, _font_size + _offset_init.y);
	sf::Text _caption;
	sf::RectangleShape _info;
	const uint32_t _info_width = 256u;
	void print(const std::string& string, const sf::Vector2f& start, uint32_t index);

	const std::vector<sf::Color> COLORS =
	{
		//1. waters
		{ 0x0a, 0x04, 0x3c },
		{ 0x00, 0x23, 0x66 },
		{ 0x23, 0x3e, 0x8b },
		{ 0x0f, 0x52, 0xba },

		//2. greenery
		{ 0xa3, 0xa8, 0x47 },
		{ 0x61, 0xb1, 0x5a },
		{ 0x5d, 0x82, 0x33 },
		{ 0x1f, 0x44, 0x1e },

		//3. soil
		{ 0x5a, 0x39, 0x21 },
		{ 0x85, 0x60, 0x3f },
		{ 0x9e, 0x75, 0x40 },
		{ 0xbd, 0x93, 0x54 },

		//4. desert
		{ 0xe3, 0xd1, 0x8a },
		{ 0xff, 0xc4, 0x78 },
		{ 0xe0, 0x8f, 0x62 },
		{ 0xcc, 0x73, 0x51 },

		//5. redhot
		{ 0xff, 0x4c, 0x29 },
		{ 0xff, 0x48, 0x48 },
		{ 0xbd, 0x16, 0x16 },
		{ 0x6f, 0x4c, 0x5b },

		//0. heights
		{ 0x26, 0x1c, 0x2c },
		{ 0x08, 0x20, 0x32 },
		{ 0x2c, 0x39, 0x4b },
		{ 0x71, 0x6f, 0x81 },

		{ 0xb2, 0xb1, 0xb9 },
		{ 0xee, 0xee, 0xee },
	};

	const std::vector<uint8_t> LIMITS = { (uint8_t)COLORS.size(), 4, 8, 12, 16, 20 };
	uint8_t _ilimit = 0;
	std::string name_limit();

	void update();
	void handle_events();
	void render();

public:
	application(tilemap* tilemap, const std::string& title = "", uint32_t dimension = 0u);
	~application();

	void run();
};
