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

SDL_Surface* merge_lines(SDL_Surface* image, corner* abs, corner* ord, int tolerance, int threshold)
{
    int moy = 0;
    int not_ready = 1;
    int k = 0;
    int tmp = 0;

    while (not_ready)
    {
        moy = 0;
        not_ready = 0;

        // Compare the distance with the others corner
        for (int x = 0; x < image->w; x++)
        {
            if (abs[x].val != -1 && abs[x].next != 0)
            {
                // if 2 corner à too close, merge them
                if (abs[x].next - tolerance < x)
                { 
                    moy = (abs[x].next + x) / 2;
                    
                    tmp = abs[abs[x].next].next;
                    abs[abs[x].next].val = -1;
                    abs[abs[x].next].next = abs[x].next;


                    abs[moy].val = threshold+1;
                    abs[moy].next = tmp;
                    
                    if (x != moy)
                    {
                        abs[x].val = -1;
                        abs[x].next = x;
                    }

                    not_ready = 1;                   
                }
            }
        }

        for (int y = 0; y < image->h; y++)
        {
            if (ord[y].val != -1 && ord[y].next != 0)
            {
                // if 2 corner à too close, merge them
                if (ord[y].next - tolerance < y && ord[y].next != y)
                {
                    moy = (ord[y].next + y) / 2;
                    
                    tmp = ord[ord[y].next].next;
                    ord[ord[y].next].val = -1;
                    ord[ord[y].next].next = ord[y].next;

                    ord[moy].val = threshold+1;
                    ord[moy].next = tmp;

                    if (y != moy)
                    {
                        ord[y].val = -1;
                        ord[y].next = y;
                    }    

                    not_ready = 1;   
                }
            }
        }
    }
    return image;
}

void gridDetect(SDL_Surface* image, corner* abs, corner* ord, int tolerance, int threshold)
{
    int corner_count = 0;
    int countX = 0;
    int countY = 0;
    int sum = 0;
    int average = 0;
    int tmp = 0;
    int last = -1;
    int k = 0;
    int enough_corner = 0;

    while (k++ < 1)//countX != 10 || countY != 10)
    {
        countX = 0;
        countY = 0;
        sum = 0;
        countX = 0;
        countY = 0;

        for (int x = 0; x < image->w; x++)
        {
            if (abs[x].val > threshold && abs[x].next != 0)
            {
                // printf("sumX = %d\n", abs[x].next - x);
                sum += abs[x].next - x;
                countX++;
            }
        }

        for (int y = 0; y < image->h; y++)
        {
            if (ord[y].val > threshold && ord[y].next != 0)
            {
                // printf("sumY = %d\n", ord[y].next - y);
                sum += ord[y].next - y;
                countY++;
            }
        }

        average = sum / (countX + countY);
        // printf("average = %d\n", average);
        last = -1;

        for (int x = 0; x < image->w; x++)
        {
            if (abs[x].val > threshold)
            {
                // if (abs[x].next == x && (last - x > average + tolerance || last - x < average - tolerance))
                // {
                //     abs[x].val = -2;
                //     abs[x].next = x;
                //     if (last != -1)
                //         abs[last].next = last;
                //     last = x;
                // }
                if (abs[x].next - x > average + tolerance || abs[x].next - x < average - tolerance)
                {
                    abs[x].val = -2;
                
                    if (last != -1 && abs[x].next != x)
                        abs[last].next = abs[x].next;
                    last = x;
                }
            }
        }

        last = -1;

        for (int y = 0; y < image->h; y++)
        {
            if (ord[y].val > threshold && (ord[y].next - y > average + tolerance || ord[y].next - y < average - tolerance))
            {
                ord[y].val = -2;

                if (last != -1 && ord[y].next != y)
                    ord[last].next = ord[y].next;
                last = y;
            }
        }

        countX = 0;
        countY = 0;

        // for (int x = 0; x < image->w; x++)
        // {
        //     if (abs[x].val > threshold && !enough_corner)
        //     {
        //         if (abs[x].next - x < average + tolerance && abs[x].next - x > average - tolerance)
        //         {
        //             countX++;
        //         }
        //         else
        //         {
        //             countX = 0;
        //         }
        //     }   
        //     if (countX == 10)
        //         enough_corner = 1;
        // }

        // FIXME: corner.prev && case counter

        for (int y = 0; y < image->h; y++)
        {
            if (ord[y].val > threshold)
                countY++;
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
            x++;
        }
        if (y < image->h)
        {
            ord[y].val = 0;
            ord[y].next = y;
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

    int last_index = 0;
    // int count = 0; // count the number of corner for the average
    // int sum = 0;
    for (int x = 0; x < image->w; x++)
    {
        if (abs[x].val > threshold)
        {
            abs[last_index].next = x;
            // count++;
            // sum += x - last_index;
            last_index = x;
        }
        else
            abs[x].val = -1;
    }
    abs[last_index].next = 0;

    
    last_index = 0;

    for (int y = 0; y < image->h; y++)
    {
        if (ord[y].val > threshold)
        {
            ord[last_index].next = y;
            // count++;
            // sum += y - last_index;
            last_index = y;
        }
        else
            ord[y].val = -1;
    }
    ord[last_index].next = 0;

    // int average = sum/count;
    int tolerance = 15; //FIXME: depend of the picture size

    merge_lines(image, abs, ord, tolerance, threshold);

    gridDetect(image, abs, ord, tolerance, threshold);


    // int count = 0; // count the number of corner (= 10)

    // for (int x = 0; x < image->w; x++)
    // {
    //     // if it's too different than the average, val = -1

        

    //     // count the number of corner, we need it 10
    //     // if (abs[x].prev + tolerance > x - average || abs[x].next - tolerance < x + average)
    //     // {
    //     //     count++;
    //     // }
    //     // else
    //     //     abs[x].val = -1;
    // }
    

    image = draw(image, abs, ord, threshold);

    free(abs);
    free(ord);

    return image;
}

