#include "floodfiller.h"

floodfiller::tile*& floodfiller::get(uint32_t x, uint32_t y)
{
	return _grid[(y * _width + x) % _size];
}

floodfiller::tile*& floodfiller::get(uint32_t x, uint32_t y, std::vector<tile*>& buffer)
{
	return buffer[(y * _width + x) % _size];
}

floodfiller::tile* floodfiller::get(uint32_t x, uint32_t y) const
{
	return _grid[(y * _width + x) % _size];
}

//lazy flood fill utility
std::vector<floodfiller::tile*> floodfiller::get_neumann_neighbors(uint32_t x, uint32_t y)
{
	std::vector<tile*> neighbors(4);
	neighbors[0] = get(x, y - 1);
	neighbors[1] = get(x - 1, y);
	neighbors[2] = get(x + 1, y);
	neighbors[3] = get(x, y + 1);
	return neighbors;
}

void floodfiller::handle_neighbors(std::queue<tile*>& queue, uint32_t x, uint32_t y)
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
std::vector<floodfiller::tile*> floodfiller::get_moore_neighbors(uint32_t x, uint32_t y)
{
	std::vector<tile*> neighbors(8);
	neighbors[0] = get(x, y - 1);
	neighbors[1] = get(x - 1, y);
	neighbors[2] = get(x + 1, y);
	neighbors[3] = get(x, y + 1);
	neighbors[4] = get(x - 1, y - 1);
	neighbors[5] = get(x - 1, y + 1);
	neighbors[6] = get(x + 1, y - 1);
	neighbors[7] = get(x + 1, y + 1);
	return neighbors;
}

std::vector<uint8_t> floodfiller::get_frequencies(const std::vector<tile*>& neighbors)
{
	std::vector<uint8_t> frequencies(_max_value + 1, 0);
	for (const auto& n : neighbors)
		frequencies[n->value]++;
	return frequencies;
}

uint8_t floodfiller::most_frequent_value(const std::vector<uint8_t>& freqs)
{
	int imax = 0;
	for (int i = 0; i < freqs.size(); i++)
		if (freqs[i] > freqs[imax])
			imax = i;
	return imax;
}

//public interface
floodfiller::floodfiller(uint32_t width, uint32_t height)
	: _width(width), _height(height), _size(width* height), _engine(std::random_device()())
{
	_grid = std::vector<tile*>(_size);
	for (uint32_t x = 0; x < _width; x++)
		for (uint32_t y = 0; y < _width; y++)
			get(x, y) = new tile(x, y);
}

floodfiller::~floodfiller()
{
	for (auto& tile : _grid)
		delete tile;
}

void floodfiller::lazy_flood_fill(uint8_t value, double decay)
{
	std::queue<tile*> queue;
	double chance = 100.;
	//std::uniform_int_distribution<uint32_t> start(0, _size - 1);
	std::poisson_distribution<uint32_t> start((_size + _width) / 2);
	std::uniform_int_distribution<int> fluct(-(int)_size / 4, _size / 4);
	std::uniform_real_distribution<double> random(0., 100.);

	queue.push(_grid[start(_engine) + fluct(_engine)]);
	//queue.push(_grid[_size / 2 + _width / 2]);
	while (!queue.empty())
	{
		auto& tile = queue.front();
		queue.pop();

		tile->queued = false;
		tile->filled = true;
		tile->value = value;
		if (_max_value < value)
			_max_value = value;

		if (random(_engine) <= chance)
			handle_neighbors(queue, tile->x, tile->y);
		chance *= decay;
	}

	for (auto& tile : _grid)
		tile->filled = tile->queued = false;

	_iterations++;
}

void floodfiller::smooth()
{
	std::vector<tile*> buffer = _grid;

	for (int x = 0; x < _width; x++)
	{
		for (int y = 0; y < _height; y++)
		{
			auto frequencies = get_frequencies(get_moore_neighbors(x, y));
			auto most_freq = most_frequent_value(frequencies);
			if (frequencies[most_freq] > 4)
				get(x, y, buffer)->value = most_freq;
		}
	}

	_grid = buffer;
}

void floodfiller::clear()
{
	_iterations = 0;
	for (uint32_t x = 0; x < _width; x++)
		for (uint32_t y = 0; y < _width; y++)
			get(x, y)->value = 0;
}

sf::Color floodfiller::get_color(uint32_t x, uint32_t y) const
{
	/*auto val = get(x, y).value / 8.f * 255.f;
	return sf::Color(val, val, val);*/
	
	return COLORS[get(x, y)->value];

	/*auto val = get(x, y)->value / 8.;
	if (val == 0.)
		return sf::Color(0, 0, 0);
	return sf::Color(255 * val, 0, 0);*/
}

uint32_t floodfiller::width() const
{
	return _width;
}

uint32_t floodfiller::height() const
{
	return _height;
}

uint32_t floodfiller::iterations() const
{
	return _iterations;
}
