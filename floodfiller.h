#pragma once
#include <iostream>
#include <random>
#include <vector>
#include <queue>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

class floodfiller
{
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

	uint8_t _max_value = 0;
	uint32_t _iterations = 0;
	std::minstd_rand _engine;

	uint32_t _width;
	uint32_t _height;
	uint32_t _size;

	tile*& get(uint32_t x, uint32_t y);
	tile*& get(uint32_t x, uint32_t y, std::vector<tile*>& buffer);
	tile* get(uint32_t x, uint32_t y) const;

	std::vector<tile*> get_neumann_neighbors(uint32_t x, uint32_t y);
	void handle_neighbors(std::queue<tile*>& queue, uint32_t x, uint32_t y);

	std::vector<tile*> get_moore_neighbors(uint32_t x, uint32_t y);
	std::vector<uint8_t> get_frequencies(const std::vector<tile*>& neighbors);
	uint8_t most_frequent_value(const std::vector<uint8_t>& freqs);
public:
	floodfiller(uint32_t width, uint32_t height);
	~floodfiller();

	void lazy_flood_fill(uint8_t value = 1, double decay = 0.9995);
	void smooth();
	void clear();

	sf::Color get_color(uint32_t x, uint32_t y) const;

	uint32_t width() const;
	uint32_t height() const;
	uint32_t iterations() const;
};
