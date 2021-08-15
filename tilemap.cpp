#include "tilemap.h"

tilemap::tile*& tilemap::get(uint32_t x, uint32_t y)
{
	return _grid[(y * _width + x) % _size];
}

tilemap::tile*& tilemap::get(uint32_t x, uint32_t y, std::vector<tile*>& buffer)
{
	return buffer[(y * _width + x) % _size];
}

tilemap::tile* tilemap::get(uint32_t x, uint32_t y) const
{
	return _grid[(y * _width + x) % _size];
}

//lazy flood fill utility
std::vector<tilemap::tile*> tilemap::get_neumann_neighbors(uint32_t x, uint32_t y)
{
	std::vector<tile*> neighbors(4);
	neighbors[0] = get(x, y - 1);
	neighbors[1] = get(x - 1, y);
	neighbors[2] = get(x + 1, y);
	neighbors[3] = get(x, y + 1);
	return neighbors;
}

void tilemap::handle_neighbors(std::queue<tile*>& queue, uint32_t x, uint32_t y)
{
	auto neighbors = get_neumann_neighbors(x, y);
	for (auto& n : neighbors)
	{
		if (!n->queued && !n->filled)
		{
			queue.push(n);
			n->queued = true;
		}
	}
}

//smooth utility
//std::vector<tilemap::tile*> tilemap::get_moore_neighbors(uint32_t x, uint32_t y)
//{
//	std::vector<tile*> neighbors(8);
//	neighbors[0] = get(x, y - 1);
//	neighbors[1] = get(x - 1, y);
//	neighbors[2] = get(x + 1, y);
//	neighbors[3] = get(x, y + 1);
//	neighbors[4] = get(x - 1, y - 1);
//	neighbors[5] = get(x - 1, y + 1);
//	neighbors[6] = get(x + 1, y - 1);
//	neighbors[7] = get(x + 1, y + 1);
//	return neighbors;
//}
//
//std::vector<uint8_t> tilemap::get_frequencies(const std::vector<tile*>& neighbors)
//{
//	std::vector<uint8_t> frequencies(COLORS.size(), 0);
//	for (const auto& n : neighbors)
//		frequencies[n->value]++;
//	return frequencies;
//}
//
//uint8_t tilemap::most_frequent_value(const std::vector<uint8_t>& freqs)
//{
//	int imax = 0;
//	for (int i = 0; i < freqs.size(); i++)
//		if (freqs[i] > freqs[imax])
//			imax = i;
//	return imax;
//}

//public interface
tilemap::tilemap(uint32_t width, uint32_t height)
	: _width(width), _height(height), _size(width* height)
{
	_grid = std::vector<tile*>(_size);
	for (uint32_t x = 0; x < _width; x++)
		for (uint32_t y = 0; y < _width; y++)
			get(x, y) = new tile(x, y);
}

tilemap::~tilemap()
{
	for (auto& tile : _grid)
		delete tile;
}

void tilemap::lazy_flood_fill(uint32_t x, uint32_t y, char bias, double decay, uint8_t limit)
{
	std::queue<tile*> queue;
	double chance = 100.;
	std::uniform_real_distribution<double> random(0., 100.);

	queue.push(get(x, y));
	while (!queue.empty())
	{
		auto& tile = queue.front();
		queue.pop();

		tile->queued = false;
		tile->filled = true;
		int value = tile->value;
		if (value + bias < limit && value + bias > -1)
			tile->value += bias;

		if (random(_engine) <= chance)
			handle_neighbors(queue, tile->x, tile->y);
		chance *= decay;
	}

	for (auto& tile : _grid)
		tile->filled = tile->queued = false;

	_iterations++;
}

//void tilemap::smooth()
//{
//	std::vector<tile*> buffer = _grid;
//
//	for (int x = 0; x < _width; x++)
//	{
//		for (int y = 0; y < _height; y++)
//		{
//			auto neighbors = get_moore_neighbors(x, y);
//			auto frequencies = get_frequencies(neighbors);
//			auto most_freq = most_frequent_value(frequencies);
//
//			if (frequencies[most_freq] > 4)
//				get(x, y, buffer)->value = most_freq;
//		}
//	}
//
//	_grid = buffer;
//}

void tilemap::clear()
{
	_iterations = 0;
	for (uint32_t x = 0; x < _width; x++)
		for (uint32_t y = 0; y < _width; y++)
			get(x, y)->value = 0;
}

uint8_t tilemap::get_value(uint32_t x, uint32_t y) const
{
	return get(x, y)->value;
}

uint32_t tilemap::width() const
{
	return _width;
}

uint32_t tilemap::height() const
{
	return _height;
}

uint32_t tilemap::iterations() const
{
	return _iterations;
}
