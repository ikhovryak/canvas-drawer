#include "canvas.h"
#include <cassert>
#include <list> 

using namespace std;
using namespace agl;



canvas::canvas(int w, int h) : _canvas(w, h)
{
	this->_canvas = ppm_image(w, h);
	this->rect_height = 1;
	this->rect_width = 1;
	this->alpha_blend = false;
	this->alpha = 0.6;
	this->stop_alpha_blend();
}

canvas::~canvas()
{
	//delete this;
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
	else if (shape == TRIANGLES) {
		while (this->vertices.size() >= 3) {
			v a = vertices.front();
			this->vertices.pop_front();
			v b = vertices.front();
			this->vertices.pop_front();
			v c = vertices.front();
			this->vertices.pop_front();

			cout << "a = " << a.x << " " << a.y << endl << "b = " << b.x << " " << b.y << "c = " << c.x << " " << c.y  << endl << endl;
			this->draw_triangle(a, b, c);
			
		}
	}
	else if (shape == RECTANGLES) {
		if (this->vertices.size() >= 1) {
			v a = vertices.front();
			this->vertices.pop_front();
			this->draw_rectangle(a);
		}
	}
	else if (shape == POINTS) {
		while (this->vertices.size() >= 1) {
			v a = vertices.front();
			this->vertices.pop_front();
			this->draw_point(a);
		}
	}
	this->vertices.clear();
}

ppm_pixel canvas::get_overlay(ppm_pixel a, int y, int x) {
	if (alpha_blend) {
		unsigned char red = (unsigned char)a.r * (1 - alpha) + _canvas.get(y,x).r * alpha;
		unsigned char green = (unsigned char)a.g * (1 - alpha) + _canvas.get(y, x).g* alpha;
		unsigned char blue = (unsigned char)a.b * (1 - alpha) + _canvas.get(y, x).b * alpha;
		return ppm_pixel{ red, green, blue };
	}
	else {
		return a;
	}

	
}

void canvas::draw_line(v a, v b)
{
	cout << "inside draw line \n";
	int W = b.x - a.x;
	int H = b.y - a.y;
	if (abs(H) < abs(W)) {
		if (a.x < b.x) {
			this->draw_lineLow(a, b);
		}
		else {
			this->draw_lineLow(b, a);

		}
	}
	else {
		if (a.y < b.y) {
			this->draw_lineHigh(a, b);
		}
		else {
			this->draw_lineHigh(b, a);
		}
	}
}
void canvas::draw_lineLow(v a, v b)
{
	cout << "Draw low\n";
	int y = a.y;
	int W = b.x - a.x;
	int H = b.y - a.y;
	int F = 2 * H - W;

	int adder = 1;

	if (H < 0) {
		H = H * (-1);
		adder = -1;
	}

	for (int x = a.x; x <= b.x; x++) {
		float t = (x - a.x) / float(b.x - a.x);
		unsigned char red = a.color.r * (1 - t) + b.color.r * t;
		unsigned char green = a.color.g * (1 - t) + b.color.g * t;
		unsigned char blue = a.color.b * (1 - t) + b.color.b * t;
		ppm_pixel new_color = ppm_pixel{ red , green, blue };
		_canvas.set(y, x, get_overlay(new_color, y,x)); // HAD TO SWAP
		if (F > 0) {
			y+=adder;
			F += 2 * (H - W);
		}
		else {
			F += 2 * H;
		}
	}
	
}

void canvas::draw_lineHigh(v a, v b)
{
	cout << "Draw high\n";

	int x = a.x;
	int W = b.x - a.x;
	int H = b.y - a.y;
	int F = 2 * W - H;
	int adder = 1;

	if (W < 0) {
		W = W * (-1);
		adder = -1;
	}
	for (int y = a.y; y <= b.y; y++) {
		float t = (y - a.y) / float(b.y - a.y);
		unsigned char red = a.color.r * (1 - t) + b.color.r * t;
		unsigned char green = a.color.g * (1 - t) + b.color.g * t;
		unsigned char blue = a.color.b * (1 - t) + b.color.b * t;
		ppm_pixel new_color = ppm_pixel{ red , green, blue };
		_canvas.set(y, x, get_overlay(cur_color, y, x)); // HAD TO SWAP
		if (F > 0) {
			x+=adder;
			F += 2 * (W - H);
		}
		else {
			F += 2 * W;
		}
	}

}


float get_bar(v a, v b, v p) {
	return (b.y - a.y) * (p.x - a.x) - (b.x - a.x) * (p.y - a.y);
}

float get_bar_float(v a, v b, float px, float py) {
	return (b.y - a.y) * (px - a.x) - (b.x - a.x) * (py - a.y);
}

void canvas::draw_point(v center)
{
	int x = (center.x >= 0 || center.x < this->_canvas.width() ? center.x : 0);
	int y = (center.y >= 0 || center.y < this->_canvas.height() ? center.y : 0);
	this->_canvas.set(y, x, cur_color);
}

void canvas::draw_rectangle(v center)
{
	int start_x = (center.x - this->rect_width/ 2 >= 0 ? center.x - this->rect_width / 2 : 0);
	int end_x = (center.x + this->rect_width / 2 <= this->_canvas.width() -1 ? center.x + this->rect_width / 2 : this->_canvas.width()-1);
	int start_y = (center.y - this->rect_height / 2 >= 0 ? center.y - this->rect_height / 2 : 0);
	int end_y = (center.y + this->rect_height / 2 <= this->_canvas.height() - 1 ? center.y + this->rect_height / 2 : this->_canvas.height() - 1);

	for (int i = start_x; i <= end_x; i++) {
		for (int j = start_y; j <= end_y; j++) {
			this->_canvas.set(j, i, get_overlay(cur_color, j, i));
		}
	}
}

void canvas::draw_triangle(v a, v b, v c) {
	//find bounding box:
	cout << "inside printTriangle" << endl;
	int xmin = min(c.x, min(a.x, b.x));
	int xmax = max(c.x, max(a.x, b.x));
	int ymin = min(c.y, min(a.y, b.y));
	int ymax = max(c.y, max(a.y, b.y));

	float f_alpha = get_bar(b, c, a);
	float f_beta = get_bar(a, c, b);
	float f_gamma = get_bar(a, b, c);

	cout << "f_alphas:\n" << f_alpha << " " << f_beta << " " << f_gamma << endl;
	for (int i = ymin; i <= ymax; i++) {
		//cout << "in the first loop\n";
		for (int j = xmin; j <= xmax; j++)
		{
			//cout << "in the second loop\n";

			v p = v{ j, i, ppm_pixel{0,0,0} };
			float alpha = get_bar(b, c, p) / f_alpha;
			float beta = get_bar(a, c, p) / f_beta;
			float gamma = get_bar(a, b, p) / f_gamma;

			if (alpha >= 0 && beta >= 0 && gamma >= 0) {

				v fake = v{ -1, -1, ppm_pixel{0,0,0} };
				unsigned char c_red = alpha * a.color.r + beta * b.color.r + gamma * c.color.r;
				unsigned char c_green = alpha * a.color.g + beta * b.color.g + gamma * c.color.g;
				unsigned char c_blue = alpha * a.color.b + beta * b.color.b + gamma * c.color.b;
				ppm_pixel final_color = ppm_pixel{ c_red, c_green, c_blue };


				if (alpha > 0 || f_alpha * get_bar_float(b, c, -1.1, -2.1) > 0) {
					//cout << "1st i= " << i << " j= " << j << endl;
					
					_canvas.set(i, j, get_overlay(final_color, i, j));
				}
				if (alpha > 0 || f_beta * get_bar_float(a, c, -1.1, -2.1) > 0) {
					//cout << "2nd i= " << i << " j= " << j << endl;

					_canvas.set(i, j, get_overlay(final_color, i, j));
				}
				if (alpha > 0 || f_gamma * get_bar_float(a, b, -1.1, -2.1) > 0) {
					//cout << "3rd i= " << i << " j= " << j << endl;
					_canvas.set(i, j, get_overlay(final_color, i, j));
				}
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

void canvas::rect_width_height(int width, int height)
{
	this->rect_height = height;
	this->rect_width = width;
}

void canvas::start_alpha_blend(float alpha) {
	this->alpha = alpha;
	this->alpha_blend = true;
}

void canvas::stop_alpha_blend() {
	this->alpha_blend = false;
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

