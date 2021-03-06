#ifndef IMAGE_H
#define IMAGE_H
#include <stdio.h>

#include "matrix.h"
#define TWOPI 6.2831853

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

// DO NOT CHANGE THIS FILE

typedef struct{
    int w,h,c;
    float *data;
} image;

// A 2d point.
// float x, y: the coordinates of the point.
typedef struct{
    float x, y;
} point;

// A descriptor for a point in an image.
// point p: x,y coordinates of the image pixel.
// int n: the number of floating point values in the descriptor.
// float *data: the descriptor for the pixel.
typedef struct{
    point p;
    int n;
    float *data;
} descriptor;

// A match between two points in an image.
// point p, q: x,y coordinates of the two matching pixels.
// int ai, bi: indexes in the descriptor array. For eliminating duplicates.
// float distance: the distance between the descriptors for the points.
typedef struct{
    point p, q;
    int ai, bi;
    float distance;
} match;

typedef struct{
    image lbp_im;
    int n;
    int* data;
}lbph_out;

typedef struct{
    int n;
    float *data;
}hog_out;



// Basic operations
float get_pixel(image im, int x, int y, int c);
void set_pixel(image im, int x, int y, int c, float v);
image copy_image(image im);
image rgb_to_grayscale(image im);
image grayscale_to_rgb(image im, float r, float g, float b);
void rgb_to_hsv(image im);
void hsv_to_rgb(image im);
void shift_image(image im, int c, float v);
void scale_image(image im, int c, float v);
void clamp_image(image im);
image get_channel(image im, int c);
int same_image(image a, image b);
image sub_image(image a, image b);
image add_image(image a, image b);

// Loading and saving
image make_image(int w, int h, int c);
image load_image(char *filename);
void save_image(image im, const char *name);
void save_png(image im, const char *name);
void free_image(image im);

// Resizing
float nn_interpolate(image im, float x, float y, int c);
image nn_resize(image im, int w, int h);
float bilinear_interpolate(image im, float x, float y, int c);
image bilinear_resize(image im, int w, int h);
image projective_resize(image im, int p1x, int p1y, int p2x, int p2y, 
    int p3x, int p3y, int p4x, int p4y); 

// Filtering
image convolve_image(image im, image filter, int preserve);
image make_box_filter(int w);
image make_highpass_filter();
image make_sharpen_filter();
image make_emboss_filter();
image make_gaussian_filter(float sigma);
image make_gx_filter();
image make_gy_filter();
void feature_normalize(image im);
void l1_normalize(image im);
void threshold_image(image im, float thresh);
image *sobel_image(image im);
image cannyEdgeDetect(image inp, double sigma, double upper_thr, double lower_thr);
image colorize_sobel(image im);
image smooth_image(image im, float sigma);

// Harris and Stitching
image structure_matrix(image im, float sigma);
image cornerness_response(image S);
void free_descriptors(descriptor *d, int n);
image cylindrical_project(image im, float f);
void mark_corners(image im, descriptor *d, int n);
image find_and_draw_matches(image a, image b, float sigma, float thresh, int nms);
void detect_and_draw_corners(image im, float sigma, float thresh, int nms);
int model_inliers(matrix H, match *m, int n, float thresh);
image combine_images(image a, image b, matrix H);
match *match_descriptors(descriptor *a, int an, descriptor *b, int bn, int *mn);
descriptor *harris_corner_detector(image im, float sigma, float thresh, int nms, int *n);
image panorama_image(image a, image b, float sigma, float thresh, int nms, float inlier_thresh, int iters, int cutoff);
point make_point(float a, float b);
point project_point(matrix A, point x);

// Optical Flow
image optical_flow_images(image im, image prev, int smooth, int stride);
void optical_flow_webcam(int smooth, int stride, int div);
void draw_flow(image im, image v, float scale);

// Feature extraction
lbph_out get_lbph_features(image im, float radius, int grid_x, int grid_y);
hog_out get_hog_features(image im);
image findLines(image inp, image canny_edge_inp, double maxvote_thr);
image findCircles(image inp, image canny_edge_inp, double maxvote_thr, int min_r, int max_r);


// Drawing
void drawLine(image im, double slope, double c, double r, double g, double b);
void drawCircle(image im, point center, double radius, double r, double g, double b);

#ifndef __cplusplus
    #ifdef OPENCV
        #include "opencv2/highgui/highgui_c.h"
        #include "opencv2/imgproc/imgproc_c.h"
        #include "opencv2/core/version.hpp"
        #if CV_MAJOR_VERSION == 3
            #include "opencv2/videoio/videoio_c.h"
            #include "opencv2/imgcodecs/imgcodecs_c.h"
        #endif
        image get_image_from_stream(CvCapture *cap);
        int show_image(image im, const char *name, int ms);
    #endif
#endif

// Machine Learning

typedef enum{LINEAR, LOGISTIC, RELU, LRELU, SOFTMAX} ACTIVATION;

typedef struct {
    matrix in;              // Saved input to a layer
    matrix w;               // Current weights for a layer
    matrix dw;              // Current weight updates
    matrix v;               // Past weight updates (for use with momentum)
    matrix out;             // Saved output from the layer
    ACTIVATION activation;  // Activation the layer uses
} layer;

typedef struct{
    matrix X;
    matrix y;
} data;

typedef struct {
    layer *layers;
    int n;
} model;

data load_classification_data(char *images, char *label_file, int bias);
void free_data(data d);
data random_batch(data d, int n);
char *fgetl(FILE *fp);

#endif

