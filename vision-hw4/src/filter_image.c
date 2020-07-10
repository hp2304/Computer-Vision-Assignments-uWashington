#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{
    float sum;
    int nb = im.w*im.h;
    for(int i=0; i<im.c; ++i){
        sum = 0;
        for(int j=0; j<nb; ++j)
            sum += *(im.data + j + nb*i);
        for(int j=0; j<nb; ++j)
            *(im.data + j + nb*i) = *(im.data + j + nb*i)/sum;
    }
}

image make_box_filter(int w)
{
    // TODO
    image filter = make_image(w, w, 1);
    float normFactor = w*(float)w;
    for(int i=0; i<(w*w); ++i)
        *(filter.data + i) = 1/normFactor;
    return filter;
}

image convolve_image(image im, image filter, int preserve)
{
    // TODO
    image filtered_image;
    float sum;
    int x, y, subX = filter.w/2, subY = filter.h/2;
    if(im.c == filter.c){
        filtered_image = make_image(im.w, im.h, 1);
        for(int im_x = 0; im_x<im.w; ++im_x){
            x = im_x - subX;
            for(int im_y = 0; im_y<im.h; ++im_y){
                y = im_y - subY;
                sum = 0;
                for(int fX = 0; fX<filter.w; ++fX){
                    for(int fY = 0; fY<filter.h; ++fY){
                        for(int fC = 0; fC<im.c; ++fC)
                            sum += (get_pixel(im, x+fX, y+fY, fC)*get_pixel(filter, fX, fY, fC));
                    }
                }
                set_pixel(filtered_image, im_x, im_y, 0, sum);
            }
        }
    }
    else if(preserve){
        filtered_image = make_image(im.w, im.h, im.c);
        for(int fC = 0; fC<im.c; ++fC){
            for(int im_x = 0; im_x<im.w; ++im_x){
                x = im_x - subX;
                for(int im_y = 0; im_y<im.h; ++im_y){
                    y = im_y - subY;
                    sum = 0;
                    for(int fX = 0; fX<filter.w; ++fX){
                        for(int fY = 0; fY<filter.h; ++fY)
                            sum += (get_pixel(im, x+fX, y+fY, fC)*get_pixel(filter, fX, fY, 0));
                    }
                    set_pixel(filtered_image, im_x, im_y, fC, sum);
                }
            }
        }
    }
    else{
        filtered_image = make_image(im.w, im.h, 1);
        for(int im_x = 0; im_x<im.w; ++im_x){
            x = im_x - subX;
            for(int im_y = 0; im_y<im.h; ++im_y){
                y = im_y - subY;
                sum = 0;
                for(int fC = 0; fC<im.c; ++fC)
                    for(int fX = 0; fX<filter.w; ++fX){
                        for(int fY = 0; fY<filter.h; ++fY){
                            sum += (get_pixel(im, x+fX, y+fY, fC)*get_pixel(filter, fX, fY, 0)); 
                    }
                }
                set_pixel(filtered_image, im_x, im_y, 0, sum);
            }
        }
    }
    return filtered_image;
}

image make_highpass_filter()
{
    // TODO
    image filter = make_image(3,3,1);
    set_pixel(filter, 0, 0, 0, 0);
    set_pixel(filter, 1, 0, 0, -1);
    set_pixel(filter, 2, 0, 0, 0);
    set_pixel(filter, 0, 1, 0, -1);
    set_pixel(filter, 1, 1, 0, 4);
    set_pixel(filter, 2, 1, 0, -1);
    set_pixel(filter, 0, 2, 0, 0);
    set_pixel(filter, 1, 2, 0, -1);
    set_pixel(filter, 2, 2, 0, 0);
    return filter;
}

image make_sharpen_filter()
{
    // TODO
    image filter = make_image(3,3,1);
    set_pixel(filter, 0, 0, 0, 0);
    set_pixel(filter, 1, 0, 0, -1);
    set_pixel(filter, 2, 0, 0, 0);
    set_pixel(filter, 0, 1, 0, -1);
    set_pixel(filter, 1, 1, 0, 5);
    set_pixel(filter, 2, 1, 0, -1);
    set_pixel(filter, 0, 2, 0, 0);
    set_pixel(filter, 1, 2, 0, -1);
    set_pixel(filter, 2, 2, 0, 0);
    return filter;
}

image make_emboss_filter()
{
    // TODO
    image filter = make_image(3,3,1);
    set_pixel(filter, 0, 0, 0, -2);
    set_pixel(filter, 1, 0, 0, -1);
    set_pixel(filter, 2, 0, 0, 0);
    set_pixel(filter, 0, 1, 0, -1);
    set_pixel(filter, 1, 1, 0, 1);
    set_pixel(filter, 2, 1, 0, 1);
    set_pixel(filter, 0, 2, 0, 0);
    set_pixel(filter, 1, 2, 0, 1);
    set_pixel(filter, 2, 2, 0, 2);
    return filter;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: TODO

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: TODO

image make_gaussian_filter(float sigma)
{
    // TODO
    int f = ceil(6*sigma);
    f = f%2?f:(f+1);
    image filter = make_image(f,f,1);
    float num, denom = pow(sigma, 2);
    for(int i=0; i<f; ++i){
        for(int j=0; j<f; ++j){
            num = -(pow(i-(f/2), 2) + pow(j-(f/2), 2));
            set_pixel(filter, i, j, 0, (1/(TWOPI*denom))*exp(num/(2*denom)));
        }
    }
    l1_normalize(filter);
    return filter;
}

image add_image(image a, image b)
{
    // TODO
    if(!(a.w == b.w && a.h == b.h && a.c == b.c))
        return make_image(a.w, a.h, a.c);
    image final = make_image(a.w, a.h, a.c);
    for(int i=0; i<a.w; ++i){
        for(int j=0; j<a.h; ++j){
            for(int c=0; c<a.c; ++c){
                set_pixel(final, i, j, c,
                 get_pixel(a, i, j, c) + get_pixel(b, i, j, c));
            }
        }
    }
    return final;
}

image sub_image(image a, image b)
{
    // TODO
    if(!(a.w == b.w && a.h == b.h && a.c == b.c))
        return make_image(a.w, a.h, a.c);
    image final = make_image(a.w, a.h, a.c);
    for(int i=0; i<a.w; ++i){
        for(int j=0; j<a.h; ++j){
            for(int c=0; c<a.c; ++c){
                set_pixel(final, i, j, c,
                 get_pixel(a, i, j, c) - get_pixel(b, i, j, c));
            }
        }
    }
    return final;
}

image make_gx_filter()
{
    // TODO
    image filter = make_image(3, 3, 1);
    set_pixel(filter, 0, 0, 0, -1);
    set_pixel(filter, 1, 0, 0, 0);
    set_pixel(filter, 2, 0, 0, 1);
    set_pixel(filter, 0, 1, 0, -2);
    set_pixel(filter, 1, 1, 0, 0);
    set_pixel(filter, 2, 1, 0, 2);
    set_pixel(filter, 0, 2, 0, -1);
    set_pixel(filter, 1, 2, 0, 0);
    set_pixel(filter, 2, 2, 0, 1);
    return filter;
}

image make_gy_filter()
{
    // TODO
    image filter = make_image(3, 3, 1);
    set_pixel(filter, 0, 0, 0, -1);
    set_pixel(filter, 1, 0, 0, -2);
    set_pixel(filter, 2, 0, 0, -1);
    set_pixel(filter, 0, 1, 0, 0);
    set_pixel(filter, 1, 1, 0, 0);
    set_pixel(filter, 2, 1, 0, 0);
    set_pixel(filter, 0, 2, 0, 1);
    set_pixel(filter, 1, 2, 0, 2);
    set_pixel(filter, 2, 2, 0, 1);
    return filter;
}

void feature_normalize(image im)
{
    // TODO
    float min, max, val;
    for(int c=0; c<im.c; ++c){
        min = __FLT_MAX__;
        max = __FLT_MIN__;
        for(int i=0; i<im.w; ++i){
            for(int j=0; j<im.h; ++j){
                val = get_pixel(im, i, j, c);
                if(val > max)
                    max = val;
                if(val < min)
                    min = val;
            }
        }
        if((max-min) == 0){
            for(int i=0; i<im.w; ++i){
                for(int j=0; j<im.h; ++j)
                    set_pixel(im, i, j, c, 0);
            }
        }
        else{
            for(int i=0; i<im.w; ++i){
                for(int j=0; j<im.h; ++j){
                    val = get_pixel(im, i, j, c);
                    set_pixel(im, i, j, c, (val-min)/(max-min));
                }
            }
        }
    }
}

image *sobel_image(image im)
{
    // TODO
    image mag = make_image(im.w, im.h, 1), theta = make_image(im.w, im.h, 1);
    image image_gx = convolve_image(im, make_gx_filter(), 0);
    image image_gy = convolve_image(im, make_gy_filter(), 0);
    float val;
    for(int i=0; i<im.w; ++i){
        for(int j=0; j<im.h; ++j){
            val = sqrt(pow(get_pixel(image_gx, i, j, 0), 2) + pow(get_pixel(image_gy, i, j, 0), 2));
            set_pixel(mag, i, j, 0, val);
            set_pixel(theta, i, j, 0,
                atan2(get_pixel(image_gy, i, j, 0), get_pixel(image_gx, i, j, 0)));
        }
    }
    image *sobel = calloc(2, sizeof(image));
    sobel[0] = mag;
    sobel[1] = theta;
    return sobel;
}

image colorize_sobel(image im)
{
    // TODO
    image out = copy_image(im);
    image *sobel = sobel_image(im);
    feature_normalize(sobel[0]);
    feature_normalize(sobel[1]);
    for(int i=0; i<im.w; ++i){
        for(int j=0; j<im.h; ++j){
            set_pixel(out, i, j, 1, get_pixel(sobel[0], i, j, 0));
            set_pixel(out, i, j, 2, get_pixel(sobel[0], i, j, 0));
        }
    }
    for(int i=0; i<im.w; ++i){
        for(int j=0; j<im.h; ++j)
            set_pixel(out, i, j, 0, get_pixel(sobel[1], i, j, 0));
    }
    hsv_to_rgb(out);
    return out;
}
