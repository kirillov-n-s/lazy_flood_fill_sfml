#include "application.h"

void application::draw(uint32_t x, uint32_t y)
{
	_tile.setPosition({ (float)(x * _tile_dim), (float)(y * _tile_dim) });
	_tile.setFillColor(_floodfiller->get_color(x, y));
	_window->draw(_tile);
}

void application::animate(double delta)
{
	for (int i = 0; i < 8; i++)
		_floodfiller->lazy_flood_fill(i + 1, _decay - (1 - i) * delta);
	_floodfiller->smooth();
}

void application::handle_events()
{
	sf::Event event;
	while (_window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			_window->close();

		if (event.type == sf::Event::KeyReleased)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::Enter:
				_floodfiller->clear();
				break;
			case sf::Keyboard::RControl:
				_floodfiller->smooth();
				break;

			case sf::Keyboard::Num1:
				_floodfiller->lazy_flood_fill(1, _decay);
				break;
			case sf::Keyboard::Num2:
				_floodfiller->lazy_flood_fill(2, _decay);
				break;
			case sf::Keyboard::Num3:
				_floodfiller->lazy_flood_fill(3, _decay);
				break;
			case sf::Keyboard::Num4:
				_floodfiller->lazy_flood_fill(4, _decay);
				break;
			case sf::Keyboard::Num5:
				_floodfiller->lazy_flood_fill(5, _decay);
				break;
			case sf::Keyboard::Num6:
				_floodfiller->lazy_flood_fill(6, _decay);
				break;
			case sf::Keyboard::Num7:
				_floodfiller->lazy_flood_fill(7, _decay);
				break;

			case sf::Keyboard::Left:
				_decay -= _delta;
				break;
			case sf::Keyboard::Right:
				_decay += _delta;
				break;
			case sf::Keyboard::RShift:
				_decay = _decay_base;
				break;

			case sf::Keyboard::Space:
				_animate ^= true;
				break;

			case sf::Keyboard::Escape:
				_window->close();
				break;
			}
		}
	}
}

void application::render()
{
	_window->clear();
	for (int x = 0; x < _floodfiller->width(); x++)
		for (int y = 0; y < _floodfiller->height(); y++)
			draw(x, y);
	_window->display();
}

application::application(floodfiller* floodfiller, const std::string& title, uint32_t dimension)
	: _floodfiller(floodfiller), _title(title)
{
	_tile_dim = dimension;
	_tile = sf::RectangleShape({ (float)_tile_dim, (float)_tile_dim });

	auto width = _floodfiller->width() * _tile_dim;
	auto height = _floodfiller->height() * _tile_dim;

	_window = new sf::RenderWindow(sf::VideoMode(width, height), _title);
	//_window->setFramerateLimit(5);
	_window->setVerticalSyncEnabled(true);
}

application::~application()
{
	delete _window;
}

void application::run()
{
	while (_window->isOpen())
	{
		std::chrono::system_clock clock;
		auto then = clock.now();
	
		handle_events();
		if (_animate)
			animate();
		render();

		uint64_t fps = 1000 / std::chrono::duration_cast<std::chrono::milliseconds>(clock.now() - then).count();
		std::string log = "[FPS: " + std::to_string(fps)
			+ "] [Iterations: " + std::to_string(_floodfiller->iterations())
			+ "] [Decay: " + std::to_string(_decay) + "]";
		_window->setTitle(_title + " " + log);
	}
}
