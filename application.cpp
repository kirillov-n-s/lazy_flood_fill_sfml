#include "application.h"

void application::draw(uint32_t x, uint32_t y)
{
	_tile.setPosition({ (float)(x * _tile_dim), (float)(y * _tile_dim) });
	_tile.setFillColor(COLORS[_floodfiller->get_value(x, y)]);
	_window->draw(_tile);
}

void application::set_up_distribution()
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
	_iters_init = _iters = std::uniform_int_distribution<uint32_t>(min, max)(_engine);

	if (_centered)
	{
		_poisson_x = std::poisson_distribution<uint32_t>(_center.x);
		_poisson_y = std::poisson_distribution<uint32_t>(_center.y);

		int dx = max * _scatter, dy = min * _scatter;
		if (!x_over_y)
			std::swap(dx, dy);
		_dx = std::uniform_int_distribution<int>(-dx, dx);
		_dy = std::uniform_int_distribution<int>(-dy, dy);
	}
}

void application::print(const std::string& string, const sf::Vector2f& start, uint32_t index)
{
	_caption.setString(string);
	_caption.setPosition(start + 2.f * index * _offset);
	_window->draw(_caption);
}

std::string application::name_limit()
{
	switch (_ilimit)
	{
	case 1:
		return "waters";
	case 2:
		return "greenery";
	case 3:
		return "soil";
	case 4:
		return "desert";
	case 5:
		return "redhot";
	default:
		return "none";
	}
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
			auto key = event.key.code;

			switch (key)
			{
			case sf::Keyboard::Space:
				if ((_gen ^= true) && !_iters)
					set_up_distribution();
				break;
			case sf::Keyboard::Enter:
				_floodfiller->clear();
				_iters_init = _iters = 0;
				_centered = _gen = false;
				break;
			case sf::Keyboard::RShift:
				_dir = -_dir;
				break;
			case sf::Keyboard::RAlt:
				_centered = false;
				break;
			case sf::Keyboard::RControl:
				_iters_init = _iters = 0;
				_gen = false;
				break;

			case sf::Keyboard::A:
				_scatter -= 0.05;
				if (_scatter < 0.)
					_scatter = 0.;
				break;
			case sf::Keyboard::D:
				_scatter += 0.05;
				if (_scatter > 0.5)
					_scatter = 0.5;
				break;
			case sf::Keyboard::W:
				_scatter = _scatter_base;
				break;
			case sf::Keyboard::S:
				_scatter = 0.;
				break;

			case sf::Keyboard::Left:
				_decay -= _delta;
				if (_decay < 0.)
					_decay = 0.;
				break;
			case sf::Keyboard::Right:
				_decay += _delta;
				if (_decay > 1.)
					_decay = 1.;
				break;
			case sf::Keyboard::Down:
				_decay -= _delta * 10.;
				if (_decay < 0.)
					_decay = 0.;
				break;
			case sf::Keyboard::Up:
				_decay += _delta * 10.;
				if (_decay > 1.)
					_decay = 1.;
				break;
			case sf::Keyboard::Home:
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
			case sf::Keyboard::Delete:
				_delta = _delta_base;
				break;

			case sf::Keyboard::Escape:
				_window->close();
				break;
			}

			auto num0 = sf::Keyboard::Num0;
			if (key >= num0 && key < num0 + LIMITS.size())
				_ilimit = key - num0;
		}

		if (event.type == sf::Event::MouseButtonReleased)
		{
			auto mouse = sf::Vector2u(sf::Mouse::getPosition(*_window)) / _tile_dim;
			if (mouse.x < 0 || mouse.y < 0 || mouse.x > _floodfiller->width() || mouse.y > _floodfiller->height())
				continue;

			switch (event.mouseButton.button)
			{
			case sf::Mouse::Left:
				_floodfiller->lazy_flood_fill(mouse.x, mouse.y, 1, _decay, LIMITS[_ilimit]);
				break;
			case sf::Mouse::Right:
				_floodfiller->lazy_flood_fill(mouse.x, mouse.y, -1, _decay, LIMITS[_ilimit]);
				break;
			case sf::Mouse::Middle:
				_center = mouse;
				_centered = true;
				break;
			}
		}
	}
}

void application::update()
{
	uint32_t x, y;
	if (_centered)
		x = _poisson_x(_engine) + _dx(_engine),
		y = _poisson_y(_engine) + _dy(_engine);
	else
		x = _uni_x(_engine),
		y = _uni_y(_engine);
	_floodfiller->lazy_flood_fill(x, y, _dir, _decay, LIMITS[_ilimit]);
}

void application::render()
{
	_window->clear();

	for (int x = 0; x < _floodfiller->width(); x++)
		for (int y = 0; y < _floodfiller->height(); y++)
			draw(x, y);

	_window->draw(_info);

	auto start = _info.getPosition() + _offset_init;
	int i = 0;
	print("Iterations:\n  " + std::to_string(_floodfiller->iterations()), start, i);
	print("Decay:\n  " + std::to_string(_decay), start, ++++i);
	print("Delta:\n  " + std::to_string(_delta), start, ++i);
	print("Direction:\n  " + std::string(_dir == 1 ? "positive" : "negative"), start, ++++i);
	print("Limit:\n  " + name_limit(), start, ++i);
	if (_centered)
		print("Centered at:\n  (" + std::to_string(_center.x) + ", " + std::to_string(_center.y) + ")", start, ++++i),
		print("Scatter:\n  " + std::to_string(_scatter), start, ++i);
	else
		print("Non-centered", start, ++++i);
	if (_iters)
		print("Generating...\n  " + std::to_string(_iters_init - _iters) + " / " + std::to_string(_iters_init), start, ++++i);

	_window->display();
}

//public interface
application::application(floodfiller* floodfiller, const std::string& title, uint32_t dimension)
	: _floodfiller(floodfiller), _title(title)
{
	_tile_dim = dimension ? dimension : _tile_dim_base;
	_tile = sf::RectangleShape({ (float)_tile_dim, (float)_tile_dim });

	auto width_orig = _floodfiller->width();
	auto height_orig = _floodfiller->height();
	auto width = width_orig * _tile_dim;
	auto height = height_orig * _tile_dim;

	_window = new sf::RenderWindow(sf::VideoMode(width + _info_width, height), _title);
	_window->setVerticalSyncEnabled(true);

	_uni_x = std::uniform_int_distribution<uint32_t>(0, width_orig);
	_uni_y = std::uniform_int_distribution<uint32_t>(0, height_orig);

	_font.loadFromFile("cour.ttf");

	_info = sf::RectangleShape({ (float)_info_width, (float)height });
	_info.setPosition({ (float)width, 0.f });
	_info.setFillColor(sf::Color::Black);
	_info.setOutlineColor(sf::Color::White);
	_info.setOutlineThickness(-4.f);

	_caption = sf::Text("", _font, _font_size);
	_caption.setFillColor(sf::Color::White);
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
				_gen ^= true, _centered = false;
		}
		render();

		uint64_t fps = 1000 / std::chrono::duration_cast<std::chrono::milliseconds>(clock.now() - then).count();
		std::string log = "[FPS: " + std::to_string(fps) + "]";
			/*+ "] [Iterations: " + std::to_string(_floodfiller->iterations())
			+ "] [Decay: " + std::to_string(_decay) + ", delta: " + std::to_string(_delta)
			+ "] [Mod: " + (_mod == 1 ? "higher" : "lower")
			+ "] [" + (_centered
				? "Centered at { " + std::to_string(_center.x) + ", " + std::to_string(_center.y) + " }, scatter: " + std::to_string(_scatter)
				: "Non-centered")
			+ "]";*/
		_window->setTitle(_title + " " + log);
	}
}
