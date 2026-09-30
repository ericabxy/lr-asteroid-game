#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "gamedef.h"
#include "r_draw.h"

//
// R_SetPixel
//
// Draw a pixel to the frame buffer, wrapping if necessary.
//
void R_SetPixel(uint32_t *buf, int x, int y, uint32_t rgba)
{
   int stride = WINDOW_WIDTH;
   int pixels = WINDOW_WIDTH * WINDOW_HEIGHT;

   x %= WINDOW_WIDTH;
   y %= WINDOW_HEIGHT;
   if (y * stride + x <= pixels && y * stride + x >= 0)
      buf[y * stride + x] = rgba;
}

//
// R_DrawLine
//
// Bresenham line function from
// https://zingl.github.io/bresenham.html#line
//
void R_DrawLine(uint32_t *buf, int x0, int y0, int x1, int y1, uint32_t rgba)
{
   int dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
   int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1; 
   int err = dx+dy, e2; /* error value e_xy */
 
   for(;;){  /* loop */
      R_SetPixel(buf, x0, y0, rgba);
      if (x0==x1 && y0==y1) break;
      e2 = 2*err;
      if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
      if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
   }
}

//
// R_DrawCircle
//
// Bresenham circle function from
// https://zingl.github.io/bresenham.html#circle
//
void R_DrawCircle(uint32_t *buf, int xm, int ym, int r, uint32_t rgba)
{
   int x = -r, y = 0, err = 2-2*r; /* II. Quadrant */ 
   do {
      R_SetPixel(buf, xm-x, ym+y, rgba); /*   I. Quadrant */
      R_SetPixel(buf, xm-y, ym-x, rgba); /*  II. Quadrant */
      R_SetPixel(buf, xm+x, ym-y, rgba); /* III. Quadrant */
      R_SetPixel(buf, xm+y, ym+x, rgba); /*  IV. Quadrant */
      r = err;
      if (r <= y) err += ++y*2+1;           /* e_xy+e_y < 0 */
      if (r > x || err > y) err += ++x*2+1; /* e_xy+e_x > 0 or no 2nd y-step */
   } while (x < 0);
}

//
// R_FillCircle
//
// Derived from Bresenham circle function from
// https://zingl.github.io/bresenham.html#circle
//
void R_FillCircle(uint32_t *buf, int xm, int ym, int r, uint32_t rgba)
{
   int x = -r, y = 0, err = 2-2*r; /* II. Quadrant */ 
   do {
      R_DrawLine(buf, xm - x, ym - y, xm + x, ym - y, rgba);
      R_DrawLine(buf, xm - x, ym + y, xm + x, ym + y, rgba);
      r = err;
      if (r <= y) err += ++y*2+1;           /* e_xy+e_y < 0 */
      if (r > x || err > y) err += ++x*2+1; /* e_xy+e_x > 0 or no 2nd y-step */
   } while (x < 0);
}

//
// R_FillRect
//
// Draw a filled, axis-aligned rectangle, wrapping if necessary.
//
void R_FillRect(uint32_t *buf, int x0, int y0, int x1, int y1, uint32_t rgba)
{
   int stride = WINDOW_WIDTH;
   int pixels = WINDOW_WIDTH * WINDOW_HEIGHT;

   for (unsigned y = y0; y < y1; y++)
      for (unsigned x = x0; x < x1; x++)
         if (y * stride + x <= pixels && y * stride + x >= 0)
            buf[y * stride + x] = rgba;
}

/* EOF */
