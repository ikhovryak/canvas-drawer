#include <iostream>
#include "canvas.h"
using namespace std;
using namespace agl;

int main(int argc, char** argv)
{
   canvas drawer(640, 380);
   // your code here
   ppm_image test = ppm_image(10, 20);
   test.save("../test.png");
}

