#include "floodfiller.h"

floodfiller::tile*& floodfiller::get(uint32_t x, uint32_t y)
{
	return _grid[(y * _width + x) % _size];
}

floodfiller::tile* floodfiller::get(uint32_t x, uint32_t y) const
{
	return _grid[(y * _width + x) % _size];
}

std::vector<floodfiller::tile*> floodfiller::get_neighbors(uint32_t x, uint32_t y)
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
	auto neighbors = get_neighbors(x, y);
	for (auto& n : neighbors)
	{
		if (!n->queued && !n->filled)
		{
			queue.push(n);
			n->queued = true;
		}
	}
}

void floodfiller::lazy_flood_fill(uint8_t value, float decay)
{
	std::queue<tile*> queue;
	double chance = 100.;
	std::uniform_int_distribution<uint32_t> start(0, _size - 1);
	std::uniform_real_distribution<double> random(0., 100.);

	queue.push(_grid[start(_engine)]);
	//queue.push(_grid[_size / 2 + _width / 2]);
	while (!queue.empty())
	{
		auto& tile = queue.front();
		queue.pop();

		tile->queued = false;
		tile->filled = true;
		tile->value = value;

		if (random(_engine) <= chance)
			handle_neighbors(queue, tile->x, tile->y);
		chance *= decay;
	}

	for (auto& tile : _grid)
		tile->filled = tile->queued = false;
}

void floodfiller::smooth()
{
	std::vector<tile*> buffer = _grid;
	for (int x = 0; x < _width; x++)
	{
		for (int y = 0; y < _height; y++)
		{
			auto neighbors = get_neighbors(x, y);
			neighbors.push_back(get(x - 1, y - 1));
			neighbors.push_back(get(x - 1, y + 1));
			neighbors.push_back(get(x + 1, y - 1));
			neighbors.push_back(get(x + 1, y + 1));
			neighbors.shrink_to_fit();

			std::vector<int> frequency(8, 0);
			for (auto& n : neighbors)
				frequency[n->value]++;

			int max_freq_val = 0;
			for (int i = 0; i < 8; i++)
				if (frequency[i] > frequency[max_freq_val])
					max_freq_val = i;

			if (frequency[max_freq_val] > 4)
				buffer[(y * _width + x) % _size]->value = max_freq_val;
		}
	}
	_grid = buffer;
}

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

void floodfiller::generate(uint8_t value, double decay)
{
	_iterations++;
	_decay = decay;
	lazy_flood_fill(value, decay);
}

//void floodfiller::demo()
//{
//	auto delta = 0.0000005;
//	_decay += delta;
//	generate(8, _decay);
//	generate(7, _decay - 1. * delta);
//	generate(6, _decay - 2. * delta);
//	generate(5, _decay - 3. * delta);
//	generate(4, _decay - 4. * delta);
//	generate(3, _decay - 5. * delta);
//	generate(2, _decay - 6. * delta);
//	generate(1, _decay - 7. * delta);
//}

void floodfiller::clear()
{
	_iterations = 0;
	_decay = 0.f;
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
		return sf::Color(0, 0, 64);
	return sf::Color(240 * val, 240 * val, 255);*/
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

double floodfiller::decay() const
{
	return _decay;
}
