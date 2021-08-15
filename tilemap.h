#pragma once
#include <random>
#include <vector>
#include <queue>

class tilemap
{
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
	tilemap(uint32_t width, uint32_t height);
	~tilemap();

	void lazy_flood_fill(uint32_t x, uint32_t y, char bias, double decay, uint8_t limit);
	/*void smooth();*/
	void clear();

	uint8_t get_value(uint32_t x, uint32_t y) const;

	uint32_t width() const;
	uint32_t height() const;
	uint32_t iterations() const;
};
