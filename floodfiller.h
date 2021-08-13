#pragma once
#include <random>
#include <vector>
#include <queue>
#include <SFML/Graphics/Color.hpp>

class floodfiller
{
	/*const std::vector<sf::Color> COLORS =
	{
		sf::Color::Black,
		sf::Color::Red,
		sf::Color::Green,
		sf::Color::Blue,
		sf::Color::Cyan,
		sf::Color::Magenta,
		sf::Color::Yellow,
		sf::Color::White,
	};*/

	const std::vector<sf::Color> COLORS =
	{
		{ 0x0a, 0x04, 0x3c },
		{ 0x00, 0x23, 0x66 },
		{ 0x23, 0x3e, 0x8b },
		{ 0x0f, 0x52, 0xba },

		{ 0xa3, 0xa8, 0x47 },
		{ 0x61, 0xb1, 0x5a },
		{ 0x5d, 0x82, 0x33 },
		{ 0x1f, 0x44, 0x1e },

		{ 0x5a, 0x39, 0x21 },
		{ 0x85, 0x60, 0x3f },
		{ 0x9e, 0x75, 0x40 },
		{ 0xbd, 0x93, 0x54 },

		{ 0xe3, 0xd1, 0x8a },
		{ 0xff, 0xc4, 0x78 },
		{ 0xe0, 0x8f, 0x62 },
		{ 0xcc, 0x73, 0x51 },

		{ 0xff, 0x4c, 0x29 },
		{ 0xff, 0x48, 0x48 },
		{ 0xbd, 0x16, 0x16 },
		{ 0x6f, 0x4c, 0x5b },

		{ 0x26, 0x1c, 0x2c },
		{ 0x08, 0x20, 0x32 },
		{ 0x2c, 0x39, 0x4b },
		{ 0x71, 0x6f, 0x81 },

		{ 0xb2, 0xb1, 0xb9 },
		{ 0xee, 0xee, 0xee },
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
	std::minstd_rand _engine = std::minstd_rand(std::random_device()());

	uint32_t _width;
	uint32_t _height;
	uint32_t _size;

	tile*& get(uint32_t x, uint32_t y);
	tile*& get(uint32_t x, uint32_t y, std::vector<tile*>& buffer);
	tile* get(uint32_t x, uint32_t y) const;

	std::vector<tile*> get_neumann_neighbors(uint32_t x, uint32_t y);
	void handle_neighbors(std::queue<tile*>& queue, uint32_t x, uint32_t y);

	/*std::vector<tile*> get_moore_neighbors(uint32_t x, uint32_t y);
	std::vector<uint8_t> get_frequencies(const std::vector<tile*>& neighbors);
	uint8_t most_frequent_value(const std::vector<uint8_t>& freqs);*/
public:
	floodfiller(uint32_t width, uint32_t height);
	~floodfiller();

	void lazy_flood_fill(uint32_t x, uint32_t y, char bias = 1, double decay = 0.9995);
	/*void smooth();*/
	void clear();

	sf::Color get_color(uint32_t x, uint32_t y) const;

	uint32_t width() const;
	uint32_t height() const;
	uint32_t iterations() const;
};
