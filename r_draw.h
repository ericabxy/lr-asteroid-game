#include <stdint.h>

#ifndef __R_DRAW__
#define __R_DRAW__

void R_SetPoint(uint32_t *buf, int x, int y, uint32_t rgba);
void R_DrawLine(uint32_t *buf, int x0, int y0, int x1, int y1, uint32_t rgba);
void R_DrawCircle(uint32_t *buf, int xm, int ym, int r, uint32_t rgba);
void R_FillCircle(uint32_t *buf, int xm, int ym, int r, uint32_t rgba);
void R_FillRect(uint32_t *buf, int x0, int y0, int x1, int y1, uint32_t rgba);

#endif
