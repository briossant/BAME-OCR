#include "ImageProcess.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <err.h>
#include <math.h>
#include <stdlib.h>

typedef struct {
    int val;
    int next;
    int prev;
} corner;

void draw_line(SDL_Surface* image, int x1, int y1, int x2, int y2)
{

    Uint32 color = SDL_MapRGBA(image->format, 255, 0, 0, 255);

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx, sy;

    if (x1 < x2) 
        sx = 1;
    else 
        sx = -1;

    if (y1 < y2) 
        sy = 1;
    else 
        sy = -1;

    int err = dx - dy;
    int current_x = x1;
    int current_y = y1;

    while (current_x != x2 || current_y != y2) 
    {
        if (current_x >=0 && current_x < image->w 
                && current_y >= 0 && current_y < image->h) 
        {
            Uint8 *pixel = (Uint8 *)image->pixels + current_y * image->pitch 
                + current_x * 4;

            *(Uint32 *)pixel = color;
        }

        int err2 = 2 * err;
        if (err2 > -dy) 
        {
            err -= dy;
            current_x += sx;
        }
        if (err2 < dx) 
        {
            err += dx;
            current_y += sy;
        }
    }
}

SDL_Surface* draw(SDL_Surface * image, corner* abs, corner* ord, int threshold)
{
    // Get the coordonates of the lines
    for (int x = 0; x < image->w; x++)
    {
        if (abs[x].val > threshold)
        {
            draw_line(image, x, 0, x, image->h);
        }
    }

    for (int y = 0; y < image->h; y++)
    {
        if (ord[y].val > threshold)
        {
            draw_line(image, 0, y, image->w, y);
        }
    }

    return image;
}

SDL_Surface* merge_lines(SDL_Surface* image, corner* abs, corner* ord, int tolerance)
{
    int moy = 0;
    int not_ready = 1;
    int tmp = 0;

    while (not_ready)
    {
        moy = 0;
        not_ready = 0;

        // Compare the distance with the others corner
        for (int x = 0; x < image->w; x++)
        {
            // if 2 corner à too close, merge them
            if (abs[x].val != -1 && abs[x].next != x && abs[x].next - tolerance < x)
            {

                // printf("x = %d, next = %d\n", x, abs[x].next);
                moy = (abs[x].next + x) / 2;
                
                tmp = abs[abs[x].next].next;
                abs[abs[x].next].val = -1;
                abs[abs[x].next].next = abs[x].next;
                abs[abs[x].next].prev = abs[x].next; //


                abs[moy].val = abs[x].val; //
                abs[moy].next = tmp;
                abs[moy].prev = abs[x].prev;
                
                if (x != moy)
                {
                    abs[x].val = -1;
                    abs[x].next = x;
                    abs[x].prev = x;
                }

                not_ready = 1;                   
            }
        }

        for (int y = 0; y < image->h; y++)
        {
            // if 2 corner à too close, merge them
            if (ord[y].val != -1 && ord[y].next != y && ord[y].next - tolerance < y)
            {
                // printf("y = %d, next = %d, prev = %d\n", y, ord[y].next, ord[y].prev);
                moy = (ord[y].next + y) / 2;
                tmp = ord[ord[y].next].next;

                ord[moy].val = ord[y].val;

            }
        }
    }
    return image;
}

int averagePos(SDL_Surface* image, corner* abs, corner* ord, int threshold)
{
    int count = 0;
    int sum = 0;

    // Count the number of corners
    for (int x = 0; x < image->w; x++)
    {
        if (abs[x].val > threshold && abs[x].next != 0)
        {
            sum += abs[x].next - x;
            count++;
        }
    }

    for (int y = 0; y < image->h; y++)
    {
        if (ord[y].val > threshold && ord[y].next != 0)
        {
            sum += ord[y].next - y;
            count++;
        }
    }
    // printf("Average: %d\n", sum / (count));
    return sum / (count);
}

void excludeLine(SDL_Surface* image, corner* abs, corner* ord, int tolerance, int threshold)
{
    int average = averagePos(image, abs, ord, threshold);

    for (int x = 0; x < image->w; x++)
    {
        // if (abs[x].val > threshold)
        // {
        //     printf("x = %d had to be exclude : %d\n", x, (abs[x].next - x > average + tolerance || abs[x].next - x < average - tolerance) 
        //     || (x - abs[x].prev > average + tolerance || x - abs[x].prev < average - tolerance));
        //     printf("x = %d, x.next = %d\n", x, abs[x].next);
        //     printf("x = %d, x.prev = %d\n", x, abs[x].prev);
        // }

        if (abs[x].val > threshold 
            && (abs[x].next - x > average + tolerance || abs[x].next - x < average - tolerance) 
            && (x - abs[x].prev > average + tolerance || x - abs[x].prev < average - tolerance))
        {
            abs[x].val = -2;
        
            if (abs[x].prev != x && abs[x].next != x)
            {
                abs[abs[x].prev].next = abs[x].next;
                abs[abs[x].next].prev = abs[x].prev;
            }   
        }
    }

    for (int y = 0; y < image->h; y++)
    {
        if (ord[y].val > threshold 
            && (ord[y].next - y > average + tolerance || ord[y].next - y < average - tolerance)
            && (y - ord[y].prev > average + tolerance || y - ord[y].prev < average - tolerance))
        {
            ord[y].val = -2;

            if (ord[y].prev != y && ord[y].next != y)
            {
                ord[ord[y].prev].next = ord[y].next;
                ord[ord[y].next].prev = ord[y].prev;
            }
        }
    }
}

void gridDetection(SDL_Surface* image, corner* abs, corner* ord, int tolerance, int threshold, int average)
{
    // int countX = 0;
    // int sequenceStarted = -1;

    for (int x = 0; x < image->w; x++)
    {
        // if the line is ok
        if (abs[x].val > threshold 
                && abs[x].next - x < average + tolerance && abs[x].next - x > average - tolerance
                && x - abs[x].prev < average + tolerance && x - abs[x].prev > average - tolerance)
        {
             
        }
    }

    for (int y = 0; y < image->h; y++)
    {
        // if the line is ok
        if (ord[y].val > threshold 
            && ord[y].next - y < average + tolerance && ord[y].next - y > average - tolerance
            && y - ord[y].prev < average + tolerance && y - ord[y].prev > average - tolerance)
        {

        }
    }
}

SDL_Surface* hough_transform(SDL_Surface * image, int threshold)
{
    // Init tab
    corner* abs = (corner*)malloc(image->w * sizeof(corner));
    corner* ord = (corner*)malloc(image->h * sizeof(corner));

    int x = 0;
    int y = 0;
    while (x < image->w || y < image->h)
    {
        if (x < image->w)
        {
            abs[x].val = 0;
            abs[x].next = x;
            abs[x].prev = x;
            x++;
        }
        if (y < image->h)
        {
            ord[y].val = 0;
            ord[y].next = y;
            ord[y].prev = y;
            y++;
        }
    }

    // Init SDL var
    Uint8 r,g,b,a;
    Uint32* pixtab = image->pixels;
    const SDL_PixelFormat* format = image->format;

    // Get the lines
    for (int x = 0; x < image->w; x++)
    {
        for (int y = 0; y < image->h; y++)
        {
            SDL_GetRGBA(pixtab[y*image->w + x], format, &r, &g, &b, &a);

            if (r > 200)
            {
                abs[x].val += 1;
                ord[y].val += 1;
            }
        }
    }

    // Remove value under threshold
    // If a value is above threshold : set next and prev
    // Prop:    the fisrt value prev = itself
    //          the last value next = itself

    int last_index = 0;
    for (int x = 0; x < image->w; x++)
    {
        if (abs[x].val > threshold)
        {
            if (last_index == 0)
                abs[x].prev = x;
            else
            {
                abs[x].prev = last_index;
                abs[last_index].next = x;
            }
            last_index = x;
        }
        else
            abs[x].val = -1;
    }
    abs[last_index].next = last_index;

    last_index = 0;

    for (int y = 0; y < image->h; y++)
    {
        if (ord[y].val > threshold)
        {
            if (last_index == 0)
                ord[y].prev = y;
            else
            {
                ord[y].prev = last_index;
                ord[last_index].next = y;
            }
            last_index = y;
        }
        else
            ord[y].val = -1;
    }
    ord[last_index].next = last_index;

    // int tolerance = threshold/20; //FIXME: depend of the picture size

    // merge_lines(image, abs, ord, tolerance, threshold);

    // printf("tabX = [\n");
    // for (int i = 0; i < image->w; i++)
    // {
    //     if (abs[i].val != -1)
    //         printf("%d:\t(val = %d, next = %d, prev = %d),\n",i , abs[i].val, abs[i].next, abs[i].prev);
    // }
    // printf("]\n\n");

    // printf("tabY = [\n");
    // for (int i = 0; i < image->h; i++)
    // {
    //     if (ord[i].val != -1)
    //         printf("%d:\t(val = %d, next = %d, prev = %d),\n",i , ord[i].val, ord[i].next, ord[i].prev);
    // }
    // printf("]\n\n");

    // excludeLine(image, abs, ord, tolerance, threshold);
  
    image = draw(image, abs, ord, threshold);

    free(abs);
    free(ord);

    return image;
}

