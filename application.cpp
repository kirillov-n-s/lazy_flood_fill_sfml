#include "application.h"

void application::draw(uint32_t x, uint32_t y)
{
	_tile.setPosition({ (float)(x * _tile_dim), (float)(y * _tile_dim) });
	_tile.setFillColor(_floodfiller->get_color(x, y));
	_window->draw(_tile);
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
			case sf::Keyboard::Space:
				if (_gen ^= true)
				{
					auto width = _floodfiller->width();
					auto height = _floodfiller->height();

					double scalar = 0.75 * _tile_dim_base / _tile_dim;
					uint32_t width_scaled = width * scalar;
					uint32_t height_scaled = height * scalar;
					uint32_t min = height_scaled, max = width_scaled;
					bool x_over_y = max >= min;
					if (!x_over_y)
						std::swap(min, max);
					_iters = std::uniform_int_distribution<uint32_t>(min, max)(_engine);

					if (_posed)
					{
						_poisson_x = std::poisson_distribution<uint32_t>(_pos.x);
						_poisson_y = std::poisson_distribution<uint32_t>(_pos.y);
						int dx, dy;
						int dmax = max / 2, dmin = min / 2;
						if (x_over_y)
							dx = dmax, dy = dmin;
						else
							dx = dmin, dy = dmax;
						_dx = std::uniform_int_distribution<int>(-dx, dx);
						_dy = std::uniform_int_distribution<int>(-dy, dy);
					}
				}
				break;
			case sf::Keyboard::Enter:
				_floodfiller->clear();
				_posed = false;
				break;

			case sf::Keyboard::RControl:
				_mod = -_mod;
				break;
			case sf::Keyboard::Left:
				_decay -= _delta;
				break;
			case sf::Keyboard::Right:
				_decay += _delta;
				break;
			case sf::Keyboard::Down:
				_decay -= _delta * 10.;
				break;
			case sf::Keyboard::Up:
				_decay += _delta * 10.;
				break;
			case sf::Keyboard::RShift:
				_decay = _decay_base;
				break;
			case sf::Keyboard::End:
				_decay = 1.;
				break;
			case sf::Keyboard::PageDown:
				_delta /= 2.;
				break;
			case sf::Keyboard::PageUp:
				_delta *= 2.;
				break;
			case sf::Keyboard::Home:
				_delta = _delta_base;
				break;

			case sf::Keyboard::Escape:
				_window->close();
				break;
			}
		}

		if (event.type == sf::Event::MouseButtonReleased)
		{
			auto pos = sf::Vector2u(sf::Mouse::getPosition(*_window)) / _tile_dim;
			if (pos.x < 0 || pos.y < 0 || pos.x > _floodfiller->width() || pos.y > _floodfiller->height())
				continue;

			switch (event.mouseButton.button)
			{
			case sf::Mouse::Left:
				_floodfiller->lazy_flood_fill(pos.x, pos.y, 1, _decay);
				break;
			case sf::Mouse::Right:
				_floodfiller->lazy_flood_fill(pos.x, pos.y, -1, _decay);
				break;
			case sf::Mouse::Middle:
				_pos = pos;
				_posed = true;
				break;
			}
		}
	}
}

void application::update()
{
	uint32_t x, y;
	if (_posed)
		x = _poisson_x(_engine) + _dx(_engine),
		y = _poisson_y(_engine) + _dy(_engine);
	else
		x = _uni_x(_engine),
		y = _uni_y(_engine);
	_floodfiller->lazy_flood_fill(x, y, _mod, _decay);
}

void application::render()
{
	_window->clear();
	for (int x = 0; x < _floodfiller->width(); x++)
		for (int y = 0; y < _floodfiller->height(); y++)
			draw(x, y);
	_window->display();
}

//public interface
application::application(floodfiller* floodfiller, const std::string& title, uint32_t dimension)
	: _floodfiller(floodfiller), _title(title)
{
	_tile_dim = dimension ? dimension : _tile_dim_base;
	_tile = sf::RectangleShape({ (float)_tile_dim, (float)_tile_dim });

	auto width = _floodfiller->width();
	auto height = _floodfiller->height();

	_window = new sf::RenderWindow(sf::VideoMode(width * _tile_dim, height * _tile_dim), _title);
	//_window->setFramerateLimit(5);
	_window->setVerticalSyncEnabled(true);

	_uni_x = std::uniform_int_distribution<uint32_t>(0, width);
	_uni_y = std::uniform_int_distribution<uint32_t>(0, height);
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
		if (_gen)
		{
			update();
			_iters--;
			if (!_iters)
				_gen ^= true, _posed = false;
		}
		render();

		uint64_t fps = 1000 / std::chrono::duration_cast<std::chrono::milliseconds>(clock.now() - then).count();
		std::string log = "[FPS: " + std::to_string(fps)
			+ "] [Iterations: " + std::to_string(_floodfiller->iterations())
			+ "] [Decay: " + std::to_string(_decay)
			+ "] [Delta: " + std::to_string(_delta)
			+ "] [Mod: " + (_mod == 1 ? "higher" : "lower") + "]";
		_window->setTitle(_title + " " + log);
	}
}
