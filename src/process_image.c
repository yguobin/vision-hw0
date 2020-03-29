#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    // TODO Fill this in
    x = x < 0 ? 0 : (x >= im.w ? im.w - 1 : x);
    y = y < 0 ? 0 : (y >= im.h ? im.h - 1 : y);
    c = c < 0 ? 0 : (c >= im.c ? im.c - 1 : c);
    return im.data[c * im.h * im.w + y * im.w + x];
}

void set_pixel(image im, int x, int y, int c, float v)
{
    // TODO Fill this in
    if( x >= 0 && x < im.w
        && y >= 0 && y < im.h
        && c >= 0 && c < im.c ) {
        im.data[c * im.h * im.w + y * im.w + x] = v;
    }
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    // TODO Fill this in
    memcpy(copy.data, im.data, im.w * im.h * im.c * sizeof(float));
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    // TODO Fill this in
    for(int i=0; i<im.h * im.w; ++i) {
	float R = im.data[i];
	float G = im.data[im.w * im.h + i];
        float B = im.data[2 * im.w * im.h + i];
	gray.data[i] = 0.299 * R + 0.587 * G + .114 * B;
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    // TODO Fill this in
    float* base = im.data + c * im.h * im.w;
    for(int i=0; i<im.h * im.w; i++) {
        *(base + i) += v;
    }
}

void clamp_image(image im)
{
    // TODO Fill this in
    for(int i=0; i<im.c * im.h * im.w; i++) {
        float d = im.data[i];
        d = d < 0 ? 0 : (d > 1.0d ? 1.0 : d);
        im.data[i] = d;
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
    int size = im.h * im.w;
    for(int i=0; i<size; ++i) {
        float* ptr_R = im.data + i;
        float* ptr_G = ptr_R + size;
        float* ptr_B = ptr_G + size;
        float R = *ptr_R, G = *ptr_G, B = *ptr_B;
        
        float V = three_way_max(R, G, B);
        float m = three_way_min(R, G, B);
        float C = V - m;
        float S = V == 0 ? 0 : C / V;
        float H_prime = 0;
        if( C > 0 ) {
            if( V == R ) {
                H_prime = (G - B) / C;
            } else if( V == G ) {
                H_prime = (B - R) / C + 2;
           } else {
                H_prime = (R - G) / C + 4;
           }
        }
        float H = H_prime < 0 ? H_prime / 6 + 1 : H_prime / 6;
        
        *ptr_R = H, *ptr_G = S, *ptr_B = V;
    }
}

void hsv_to_rgb(image im)
{
    // TODO Fill this in
    int size = im.h * im.w;
    for(int i=0; i<size; ++i) {
        float* ptr_H = im.data + i;
        float* ptr_S = ptr_H + size;
        float* ptr_V = ptr_S + size;
        float H = *ptr_H, S = *ptr_S, V = *ptr_V;

        float H_prime = 6 * H, M = V, C = S * V, m = V - C;
        float R = 0, G = 0, B = 0;
        if(H_prime < 1) {
            R = M, G = m + C * H_prime, B = m;
        } else if(H_prime < 2) {
            R = m - C * (H_prime - 2), G = M, B = m;
        } else if(H_prime < 3) {
            R = m, G = M, B = m + C * (H_prime - 2);
        } else if(H_prime < 4) {
            R = m, G = m - C * (H_prime - 4), B = M;
        } else if(H_prime < 5) {
            R = m + C * (H_prime - 4), G = m, B = M;
        } else {
            R = M, G = m, B = m - C * (H_prime - 6);
        }

        *ptr_H = R, *ptr_S = G, *ptr_V = B;
    }
}
