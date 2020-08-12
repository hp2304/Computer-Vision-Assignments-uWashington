#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"

lbph_out get_lbph_features(image im, float radius, int grid_x, int grid_y){
    image gray_im = rgb_to_grayscale(im);
    image lbp = make_image(im.w, im.h, 1);
    float val, angle;
    for(int i=0; i<im.w; ++i){
        for(int j=0; j<im.h; ++j){
            val = 0;
            angle = 3*(M_PI/4);
            for(int cnt = 7; cnt >= 0; --cnt){
                if(bilinear_interpolate(gray_im, i + cos(angle), j - sin(angle), 0)>=get_pixel(gray_im, i, j, 0)){
                    val += pow(2, cnt);
                }
                angle += (M_PI/4);
            }
            set_pixel(lbp, i, j, 0, val/255.);
        }
    }
    lbph_out lbph_features;
    lbph_features.lbp_im = lbp;
    int dx = (int)ceil(im.w/(float)grid_x), dy = (int)ceil(im.h/(float)grid_y), idx = 0;
    lbph_features.n = grid_x*grid_y;
    lbph_features.data = calloc(lbph_features.n*256, sizeof(int));
    for(int j=0; j<im.h; j += dy){
        for(int i=0; i<im.w; i += dx){
            for(int x=i; x<(i+dx) && x<im.w; ++x){
                for(int y=j; y<(j+dy) && y<im.h; ++y){
                    lbph_features.data[(256*idx) + (int)(255*get_pixel(lbp, x, y, 0))]++;
                }
            }
            ++idx;
        }
    }
    return lbph_features;
}

float find_squared_sum(int n, float *arr){
    float sum = 0;
    for(int i=0; i<n; ++i)
        sum += pow(arr[i], 2);
    return sum;
}

hog_out get_hog_features(image im){
    if(im.w*2 != im.h){
        hog_out none = {0};
        printf("Invalid aspect ratio. It must be 1:2 (width:height)");
        return none;
    }
    image inp = bilinear_resize(convolve_image(im, make_gaussian_filter(2), 1), 64, 128);
    image *grad_info = sobel_image(inp);
    float *cell_hists[16][8];
    for(int j=0; j<inp.h; j += 8){
        for(int i=0; i<inp.w; i += 8){
            cell_hists[j/8][i/8] = calloc(9, sizeof(float));
            float angle;
            for(int y = j; y<(j+8); ++y){
                for(int x = i; x<(i+8); ++x){
                    angle = get_pixel(grad_info[1], x, y, 0);
                    angle = abs((180/M_PI)*angle);
                    int temp = (int)(angle/20);
                    cell_hists[j/8][i/8][temp] += (((float)((temp+1)*20)-angle)/20)*get_pixel(grad_info[0], x, y, 0);
                    cell_hists[j/8][i/8][(temp+1)%9] += ((angle - (float)(temp*20))/20)*get_pixel(grad_info[0], x, y, 0);
                }
            }
        }
    }
    hog_out hog_features;
    hog_features.n = 3780;
    hog_features.data = calloc(hog_features.n, sizeof(float));
    int idx = 0;
    for(int i=0; i<15; ++i){
        for(int j=0; j<7; ++j){
            float l2_norm = 0;
            for(int x=i; x<(i+2); ++x){
                for(int y=j; y<(j+2); ++y)
                    l2_norm += find_squared_sum(9, cell_hists[x][y]);
            }
            l2_norm = sqrt(l2_norm);
            for(int x=i; x<(i+2); ++x){
                for(int y=j; y<(j+2); ++y){
                    for(int k=0; k<9; ++k)
                        hog_features.data[idx++] = cell_hists[x][y][k]/l2_norm;
                }
            }

        }
    }
    for(int i=0; i<16; ++i){
        for(int j=0; j<8; ++j)
            free(cell_hists[i][j]);
    }
    free_image(grad_info[0]);
    free_image(grad_info[1]);
    free(grad_info);
    return hog_features;
}


void findLines(image inp, double edgeThr){
    image *sobel = sobel_image(inp);
    int grid_width = 360;
    int min_dim = inp.h <= inp.w ? inp.h : inp.w;
    int grid_height = 2*min_dim + 1;
    image hough_acc = make_image(grid_width, grid_height, 1);
    for(int i=0; i<inp.w; ++i){
        for(int j=0; j<inp.h; ++j){
            double mag = get_pixel(sobel[0], i, j, 0);
            double theta = get_pixel(sobel[1], i, j, 0)*(180/M_PI);
            if(mag > edgeThr){
                int d = (int)(i*cos(theta) + j*sin(theta));
                if(abs(d) > min_dim)
                    continue;
                set_pixel(hough_acc, (int)(theta + 180) % 360, d + min_dim, 0, 
                    get_pixel(hough_acc, (int)(theta + 180) % 360, d + min_dim, 0) + mag);
            }
        }
    }
    double max_votes = 0, theta = 0, d = 0;
    for(int x=0; x<hough_acc.w; ++x){
        for(int y=0; y<hough_acc.h; ++y){
            double votes = get_pixel(hough_acc, x, y, 0);
            if(votes > max_votes){
                max_votes = votes;
                theta = x - 180;
                d = y - min_dim;
            }
        }
    }
    free_image(hough_acc);
    free_image(sobel[0]);
    free_image(sobel[1]);
    free(sobel);
    printf("max votes: %lf , theta: %lf, d: %lf\n", max_votes, theta, d);
    printf("%lf = x%lf + y%lf\n", d, cos(theta), sin(theta));
    int p2_x, p1_y;
    p2_x = (int)(d/cos(theta));
    p1_y = (int)(-d/sin(theta));
    printf("p2_x: %d\n", p2_x);
    for(int x=0; x<=6; ++x){
        for(int y=p1_y-6; y<=(p1_y+6); ++y){
            set_pixel(inp, x, y, 0, 0);
            set_pixel(inp, x, y, 1, 0);
            set_pixel(inp, x, y, 2, 1);
        }
    }
    for(int y=0; y<=6; ++y){
        for(int x=p2_x-6; x<=(p2_x+6); ++x){
            set_pixel(inp, x, y, 0, 0);
            set_pixel(inp, x, y, 1, 0);
            set_pixel(inp, x, y, 2, 1);
        }
    }
    save_image(inp, "out");
}