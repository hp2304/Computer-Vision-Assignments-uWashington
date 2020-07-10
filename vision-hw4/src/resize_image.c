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

image projective_resize(image im, int p1x, int p1y, int p2x, int p2y, 
    int p3x, int p3y, int p4x, int p4y){
    image none = {0};
    if(!(im.w > p1x && p1x >= 0 && p1y < im.h && p1y >= 0
        && im.w > p2x && p2x >= 0 && p2y < im.h && p2y >= 0
        && im.w > p3x && p3x >= 0 && p3y < im.h && p3y >= 0
        && im.w > p4x && p4x >= 0 && p4y < im.h && p4y >= 0)){
        printf("Invalid coordinate values...\n");
        return none;
    }
    point out_points[4] = {make_point((float)p1x, (float)p1y),
                            make_point((float)p2x, (float)p2y),
                            make_point((float)p3x, (float)p3y),
                            make_point((float)p4x, (float)p4y)};
    point in_points[4] = {make_point(0., 0.), make_point((float)im.w-1, 0.),
                            make_point(0., (float)im.h-1), make_point((float)im.w-1, (float)im.h-1)};
    matrix M = make_matrix(8, 8);
    matrix b = make_matrix(8, 1);
    for(int i=0; i<4; ++i){
        b.data[2*i][0] = out_points[i].x;
        b.data[2*i + 1][0] = out_points[i].y;

        M.data[2*i][0] = in_points[i].x;
        M.data[2*i][1] = in_points[i].y;
        M.data[2*i][2] = 1;
        M.data[2*i][3] = 0;
        M.data[2*i][4] = 0;
        M.data[2*i][5] = 0;
        M.data[2*i][6] = -(in_points[i].x*out_points[i].x);
        M.data[2*i][7] = -(in_points[i].y*out_points[i].x);

        M.data[2*i + 1][0] = 0;
        M.data[2*i + 1][1] = 0;
        M.data[2*i + 1][2] = 0;
        M.data[2*i + 1][3] = in_points[i].x;
        M.data[2*i + 1][4] = in_points[i].y;
        M.data[2*i + 1][5] = 1;
        M.data[2*i + 1][6] = -(in_points[i].x*out_points[i].y);
        M.data[2*i + 1][7] = -(in_points[i].y*out_points[i].y);
    }
    matrix a = solve_system(M, b);
    free_matrix(M); free_matrix(b); 

    // If a solution can't be found, return empty matrix;
    if(!a.data){
        return none;
    }

    matrix H = make_matrix(3, 3);
    for(int i=0; i<8; ++i)
        H.data[i/3][i%3] = a.data[i][0];
    H.data[2][2] = 1;
    free_matrix(a);

    point temp, proj_temp;
    image out = make_image(im.w, im.h, im.c);
    for(int i=0; i<im.w; ++i){
        for(int j=0; j<im.h; ++j){
            for(int k=0; k<im.c; ++k){
                temp = make_point((float)i, (float)j);
                proj_temp = project_point(H, temp);
                if(proj_temp.x >= im.w || proj_temp.x < 0 || 
                    proj_temp.y >= im.h || proj_temp.y < 0){
                        continue;
                }
                set_pixel(out, i, j, k, bilinear_interpolate(im, proj_temp.x, proj_temp.y, k));
            }
        }
    }
    return out;                            
}

