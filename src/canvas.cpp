#include "canvas.h"
#include <cassert>
#include <list> 

using namespace std;
using namespace agl;



canvas::canvas(int w, int h) : _canvas(w, h)
{
	this->_canvas = ppm_image(w, h);
	
	
}

canvas::~canvas()
{
	
}

void canvas::save(const std::string& filename)
{
   _canvas.save(filename);
}

void canvas::begin(PrimitiveType type)
{
	this->shape = type;
}



void canvas::end()
{
	if (shape == LINES) {
		while (this->vertices.size() >= 2) {
			v a = vertices.front();
			this->vertices.pop_front();
			v b = vertices.front();
			this->vertices.pop_front();
			cout << "a = " << a.x << " " << a.y << endl << "b = " << b.x << " " << b.y << endl << endl;
			this->draw_line(a, b);
		}
	}
	this->vertices.clear();
}

void canvas::draw_line(v a, v b)
{
	if (a.x > b.x) {
		v temp = a;
		a = b;
		b = temp;
	}

	int y = a.y;
	int W = b.x - a.x;
	int H = b.y - a.y;
	int F = 2 * H - W;
	if (H / float(W) >= 0 && H / float(W) <= 1) {
		for (int x = a.x; x <= b.x; x++) {
			_canvas.set(y, x, cur_color); // HAD TO SWAP
			if (F > 0) {
				y++;
				F += 2 * (H - W);
			}
			else {
				F += 2 * H;
			}
		}
	}
	
}

void canvas::vertex(int x, int y)
{
	if (x >= _canvas.width()) {
		x = _canvas.width()-1;
	}
	if (y >= _canvas.height()) {
		y = _canvas.height() - 1;
	}
	//cout << "Set a = " << a.x << " " << a.y << endl << "Set b = " << b.x << " " << b.y << endl << endl;

	this->vertices.push_back(v{ x , y , cur_color });
}

void canvas::color(unsigned char r, unsigned char g, unsigned char b)
{
	this->cur_color = ppm_pixel{ r, g, b };
}

void canvas::background(unsigned char r, unsigned char g, unsigned char b)
{

	this->bg_color = ppm_pixel{ r, g, b };

	for (int i = 0; i < _canvas.height(); i++) {
		for (int j = 0; j < _canvas.width(); j++) {
			this->_canvas.set(i, j, bg_color);
		}
	}
}

