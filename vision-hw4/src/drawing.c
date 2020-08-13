#include "image.h"
#include <math.h>
#include <assert.h>

void drawLine(image im, double slope, double c, double r, double g, double b){
    assert(im.c == 3);
    int w = im.w, h = im.h;
    for(int x=0; x<w; ++x){
        int y = (int)(slope*x + c);
        if(y <= 0 && y > -h){
            set_pixel(im, x, -y, 0, r);
            set_pixel(im, x, -y, 1, g);
            set_pixel(im, x, -y, 2, b);
        }
    }
    for(int y=0; y<h; ++y){
        int x = (int)((-y - c)/slope);
        if(x >= 0 && x < w){
            set_pixel(im, x, y, 0, r);
            set_pixel(im, x, y, 1, g);
            set_pixel(im, x, y, 2, b);
        }
    }
}

void drawCircle(image im, point center, double radius, double r, double g, double b){
    assert(im.c == 3);
    center.y = -center.y;
    double step_size = M_PI/(100*r*180);
    for(double theta = 0; theta <= (2*M_PI); theta += step_size){
        int x = (int)(center.x + radius*cos(theta));
        int y = -(int)(center.y + radius*sin(theta));
        set_pixel(im, x, y, 0, r);
        set_pixel(im, x, y, 1, g);
        set_pixel(im, x, y, 2, b);
    }
}