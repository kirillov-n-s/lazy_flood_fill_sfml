#pragma once
#include <iostream>
#include <random>
#include <vector>
#include <queue>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

class floodfiller
{
	//const sf::Color* COLORS = new sf::Color[]
	//{
	//	{ 0xff, 0xda, 0xb9 }, //sand
	//	{ 0xb0, 0x5b, 0x3b }, //soil
	//	{ 0x5d, 0x82, 0x33 }, //greenery

	//	{ 0x9e, 0x75, 0x40 }, //rock
	//	{ 0x44, 0x5c, 0x3c }, //swamp
	//	{ 0x1f, 0x44, 0x1e }, //tree

	//	{ 0x7f, 0x8b, 0x52 }, //succulent
	//	{ 0x32, 0x52, 0x88 }, //water
	//};

	const std::vector<sf::Color> COLORS =
	{
		sf::Color::Black,
		sf::Color::Red,
		sf::Color::Green,
		sf::Color::Blue,
		sf::Color::Cyan,
		sf::Color::Magenta,
		sf::Color::Yellow,
		sf::Color::White,
	};

	struct tile
	{
		uint32_t x = 0;
		uint32_t y = 0;

		uint8_t value = 0;

		bool queued = false;
		bool filled = false;

		tile(uint32_t x, uint32_t y)
			: x(x), y(y) {}
	};

	std::vector<tile*> _grid;

	uint32_t _iterations = 0;
	double _decay = 0.;
	std::minstd_rand _engine;

	uint32_t _width;
	uint32_t _height;
	uint32_t _size;

	tile*& get(uint32_t x, uint32_t y);
	tile* get(uint32_t x, uint32_t y) const;
	std::vector<tile*> get_neighbors(uint32_t x, uint32_t y);
	void handle_neighbors(std::queue<tile*>& queue, uint32_t x, uint32_t y);
	void lazy_flood_fill(uint8_t value, float decay);

public:
	floodfiller(uint32_t width, uint32_t height);
	~floodfiller();

	void generate(uint8_t value = 1, double decay = 0.9995);
	void smooth();
	/*void demo();*/
	void clear();

	sf::Color get_color(uint32_t x, uint32_t y) const;

	uint32_t width() const;
	uint32_t height() const;
	uint32_t iterations() const;
	double decay() const;
};
