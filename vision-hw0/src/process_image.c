#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    int h = im.h, w = im.w;
    if(x<0)
        x = 0;
    else if(x>=w)
        x = w-1;
    if(y<0)
        y = 0;
    else if(y>=h)
        y = h-1;
    if(c<0)
        c = 0;
    else if(c>=im.c)
        c = im.c-1;
    
    return *(im.data+(c*h*w)+(w*y)+x);
}

void set_pixel(image im, int x, int y, int c, float v)
{
    int h = im.h, w = im.w, nbC = im.c;
    if(x>=0 && x<w && y>=0 && y<h && c>=0 && c<nbC)
       *(im.data+(c*h*w)+(w*y)+x) = v;
}

image copy_image(image im)
{
    int nbPixels = im.w * im.h * im.c;
    image copy = make_image(im.w, im.h, im.c);
    // TODO Fill this in
    memcpy(copy.data, im.data, nbPixels * sizeof(im.data[0]));
    return copy;
}

image rgb_to_grayscale(image im)
{
    int skipAmt = im.w * im.h;
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    // TODO Fill this in
    float *temp, grayVal;
    for(int i = 0; i < skipAmt; ++i){
        grayVal = 0;
        temp = im.data + i;
        grayVal += (*temp * 0.299);
        temp += skipAmt;
        grayVal += (*temp * 0.587);
        temp += skipAmt;
        grayVal += (*temp * 0.114);
        *(gray.data + i) = grayVal;
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    // TODO Fill this in
    float *temp = im.data + (im.w * im.h * c);
    for(int i = 0; i < (im.w*im.h); ++i)
        *(temp + i) += v;
}

void scale_image(image im, int c, float v)
{
    // TODO Fill this in
    float *temp = im.data + (im.w * im.h * c);
    for(int i = 0; i < (im.w*im.h); ++i)
        *(temp + i) *= v;
}

void clamp_image(image im)
{
    // TODO Fill this in
    int nbPixels = im.w * im.h * im.c;
    float val;
    for(int i = 0; i < nbPixels; ++i){
        val = *(im.data + i);
        if(val<0)
            val = 0;
        else if(val > 1)
            val = 1;
        *(im.data + i) = val;
    }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    // TODO Fill this in
    int nbPixels = im.h * im.w;
    float r,g,b,c,v,s,h,h_des, *temp;
    for(int i = 0; i<nbPixels; ++i){
        temp = (im.data + i);
        r = *temp;
        temp += nbPixels;
        g = *temp;
        temp += nbPixels;
        b = *temp;
        v = three_way_max(r,g,b);
        c = v - three_way_min(r,g,b);
        if(v)
            s = c/v;
        else
            s = 0;
        if(c==0)
            h = 0;
        else{
            if(v==r)
                h_des = (g-b)/c;
            else if(v==g)
                h_des = ((b-r)/c) + 2;
            else
                h_des = ((r-g)/c) + 4;
            if(h_des<0)
                h = h_des/6 + 1;
            else
                h = h_des/6;
        }
        temp = (im.data + i);
        *temp = h;
        temp += nbPixels;
        *temp = s;
        temp += nbPixels;
        *temp = v;
    }
}

float max(float a, float b){
    return a>=b?a:b;
}

void hsv_to_rgb(image im)
{
    int nbPixels = im.h * im.w;
    float r,g,b,v,s,h,c,h_des,m, *temp;
    for(int i = 0; i<nbPixels; ++i){
        temp = (im.data + i);
        h = *temp;
        temp += nbPixels;
        s = *temp;
        temp += nbPixels;
        v = *temp;
        c = s*v;
        m = v-c;
        if(c==0){
            r=v;
            g=v;
            b=v;
        }
        else{
            if(h<=1 && h>(5/(float)6)){
                h_des = (h-1)*6;
                r=v;g=m;b=g-(h_des*c);
            }
            else{
                h_des = h*6;
                if(h_des==0){r=v;g=m;b=m;}
                else if(h_des>0 && h_des<1){r=v;b=m;g= b+(h_des*c);}
                else if(h_des<2){g=v;b=m;r= b - (h_des-2)*c;}
                else if(h_des==2){b=m;r=m;g=v;}
                else if(h_des>2 && h_des<3){g=v;r=m;b= r + (h_des-2)*c;}
                else if(h_des < 4){b=v;r=m;g = r - (h_des-4)*c;}
                else if(h_des == 4){b=v;g=m;r=m;}
                else{b=v;g=m;r = (h_des-4)*c + g;}
            }
        }
        temp = (im.data + i);
        *temp = r;
        temp += nbPixels;
        *temp = g;
        temp += nbPixels;
        *temp = b;
    }
}
