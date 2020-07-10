#include <math.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
    // TODO Fill in
    return get_pixel(im, (int)round(x), (int)round(y), c);
}

image nn_resize(image im, int w, int h)
{
    // TODO Fill in (also fix that first line)
    image newIm = make_image(w,h,im.c);
    float m_x = (im.w/(float)w), c_x = (im.w - w)/(float)(2*w);
    float m_y = (im.h/(float)h), c_y = (im.h - h)/(float)(2*h);
    float xNew, yNew;
    for(int i=0; i<w; ++i){
        xNew = m_x*i + c_x;
        for(int j=0; j<h; ++j){
            yNew = m_y*j + c_y;
            for(int c=0; c<im.c; ++c)
                set_pixel(newIm, i, j, c, nn_interpolate(im, xNew, yNew, c));
        }
    }
    return newIm;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    // TODO
    float x1 = floor(x), x2 = x1+1;
    float y1 = floor(y), y2 = y1+1;
    float d1 = x-x1, d2 = x2-x, d3 = y-y1, d4 = y2 - y;
    float a1 = d1*d3, a2 = d2*d3, a3 = d1*d4, a4 = d2*d4;
    return a4*get_pixel(im, (int)x1, (int)y1, c) + a3*get_pixel(im, (int)x2, (int)y1, c) +
            a2*get_pixel(im, (int)x1, (int)y2, c) + a1*get_pixel(im, (int)x2, (int)y2, c);
}

image bilinear_resize(image im, int w, int h)
{
    // TODO
    image newIm = make_image(w,h,im.c);
    float m_x = (im.w/(float)w), c_x = (im.w - w)/(float)(2*w);
    float m_y = (im.h/(float)h), c_y = (im.h - h)/(float)(2*h);
    float xNew, yNew;
    for(int i=0; i<w; ++i){
        xNew = m_x*i + c_x;
        for(int j=0; j<h; ++j){
            yNew = m_y*j + c_y;
            for(int c=0; c<im.c; ++c)
                set_pixel(newIm, i, j, c, bilinear_interpolate(im, xNew, yNew, c));
        }
    }
    return newIm;
}

