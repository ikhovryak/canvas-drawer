#ifndef canvas_H_
#define canvas_H_

#include <string>
//#include <vector>
#include "ppm_image.h"
#include <list> 
#include <iostream>
#include <queue> 

namespace agl
{
    struct v
    {
        int x;
        int y;
        ppm_pixel color;
    };

   enum PrimitiveType {UNDEFINED, LINES, TRIANGLES, RECTANGLES, POINTS};
   class canvas
   {
   public:
      canvas(int w, int h);
      virtual ~canvas();

      // Save to file
      void save(const std::string& filename);

      // Draw primitives with a given type (either LINES or TRIANGLES)
      // For example, the following draws a red line followed by a green line
      // begin(LINES);
      //    color(255,0,0);
      //    vertex(0,0);
      //    vertex(100,0);
      //    color(0,255,0);
      //    vertex(0, 0);
      //    vertex(0,100);
      // end();
      void begin(PrimitiveType type);
      void end();

      // Specifiy a vertex at raster position (x,y)
      // x corresponds to the column; y to the row
      void vertex(int x, int y);

      // Specify a color. Color components are in range [0,255]
      void color(unsigned char r, unsigned char g, unsigned char b);
      void rect_width_height(int width, int height);
      // Fill the canvas with the given background color
      void background(unsigned char r, unsigned char g, unsigned char b);
      void draw_line(v a, v b);
      void draw_lineLow(v a, v b);
      void draw_lineHigh(v a, v b);
      void draw_rectangle(v center);
      void draw_triangle(v a, v b, v c);
      void draw_point(v a);
      void start_alpha_blend(float alpha);
      void stop_alpha_blend();
      ppm_pixel get_overlay(ppm_pixel a, int y, int x);

      //void draw_triangle(v v1, v v2, v v3);

   private:
      ppm_image _canvas;
      ppm_pixel cur_color;
      ppm_pixel bg_color;
      PrimitiveType shape;
      std::list<v> vertices;
      int rect_width;
      int rect_height;
      bool alpha_blend;
      float alpha;
   };
}

#endif

