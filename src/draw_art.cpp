#include <iostream>
#include "canvas.h"
using namespace std;
using namespace agl;

int main(int argc, char** argv)
{
   canvas drawer(100, 100);

   

   //my art:
   canvas my_drawer(700, 700);
   my_drawer.background(255, 255, 255);
   ppm_pixel blue = ppm_pixel{ 82, 227, 220 };
   ppm_pixel pink = ppm_pixel{ 237, 0, 126 };
   ppm_pixel yellow = ppm_pixel{ 255, 222, 5 };
   my_drawer.begin(TRIANGLES);
   //big top triangle
   my_drawer.color(0, 219, 209);
   my_drawer.vertex(49, 349);

   my_drawer.color(255, 0, 142);
   my_drawer.vertex(349, 49);

   my_drawer.color(255, 250, 33);
   my_drawer.vertex(649, 349);

   //big bottom triangle

   my_drawer.color(0, 219, 209);
   my_drawer.vertex(49, 349);

   my_drawer.color(255, 0, 142);
   my_drawer.vertex(349, 649);

   my_drawer.color(255, 250, 33);
   my_drawer.vertex(649, 349);

   my_drawer.end();
	// upsidedown big triangle with alpha
   my_drawer.begin(TRIANGLES);

   my_drawer.start_alpha_blend(0.45);
   my_drawer.color(255, 250, 33);
   my_drawer.vertex(99, 149);

   my_drawer.color(255, 0, 142);
   my_drawer.vertex(349, 449);

   my_drawer.color(0, 219, 209);
   my_drawer.vertex(600, 149);


   my_drawer.color(255, 250, 33);
   my_drawer.vertex(99, 549);

   my_drawer.color(255, 0, 142);
   my_drawer.vertex(349, 249);

   my_drawer.color(0, 219, 209);

   my_drawer.vertex(600, 549);

   my_drawer.end();
   my_drawer.start_alpha_blend(0.55);
   /*my_drawer.begin(RECTANGLES);
   my_drawer.color(255, 255, 255);
   my_drawer.vertex(349, 349);
   my_drawer.rect_width_height(79, 79);
   my_drawer.end();*/
   my_drawer.begin(TRIANGLES);
   my_drawer.color(255, 250, 33);

   //my_drawer.color(255, 0, 142);
   my_drawer.vertex(289, 349);
   my_drawer.color(255, 250, 33);
   my_drawer.vertex(349, 419);
   my_drawer.color(0, 219, 209);
   my_drawer.vertex(409, 349);


   my_drawer.color(255, 250, 33);
   //my_drawer.color(255, 0, 142);
   my_drawer.vertex(289, 348);
   my_drawer.color(255, 250, 33);
   my_drawer.vertex(349, 279);
   my_drawer.color(0, 219, 209);

   my_drawer.vertex(409, 348);
   my_drawer.end();

   my_drawer.save("myart.png");
}

