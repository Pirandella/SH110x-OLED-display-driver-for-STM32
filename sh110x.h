/* Copyright (c) 2022 Pirandella

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. */

#ifndef SRC_SH110X_H_
#define SRC_SH110X_H_

#include "main.h"

/* --------- Change this to match you configuration --------------- */
#define SH110X_SPI_INTERFACE					hspi1
extern SPI_HandleTypeDef 						SH110X_SPI_INTERFACE;

#define SH110X_RST_LOW							(RST_GPIO_Port->ODR &= ~RST_Pin)
#define SH110X_RST_HI							(RST_GPIO_Port->ODR |= RST_Pin)
#define SH110X_DC_LOW							(DC_GPIO_Port->ODR &= ~DC_Pin)
#define SH110X_DC_HI							(DC_GPIO_Port->ODR |= DC_Pin)
#define SH110X_CS_LOW							(CS_GPIO_Port->ODR &= ~CS_Pin)
#define SH110X_CS_HI							(CS_GPIO_Port->ODR |= CS_Pin)
/* ---------------------------------------------------------------- */

/* --- !!! Change this to 128 if 128x128 px display is used !!! --- */
#define _SH110X_PAGE_SIZE						64
#define _SH110X_NUMBER_OF_PAGES					16

#define _SH110X_WIDHT_DEFAULT					64
#define _SH110X_HEIGHT_DEFAULT					128

extern const uint8_t *const _sh110x_width;
extern const uint8_t *const _sh110x_height;
#define SH110X_WIDTH							((uint8_t)(*_sh110x_width))
#define SH110X_HEIGHT							((uint8_t)(*_sh110x_height))

extern const uint8_t *const _sh110x_displayOrientation;
#define SH110X_SCREE_ORIENTATION				((enum SH110X_SCREEN_ROT)(*_sh110x_displayOrientation))

enum SH110X_COLOR {BLACK = 0x00, WHITE = 0xFF};
enum SH110X_SCREEN_ROT {SCREEN_ROT_0, SCREEN_ROT_90, SCREEN_ROT_180, SCREEN_ROT_270};
enum SH110X_DISPLAY_MODE {NORMAL, INVERSE, SLEEP, WAKE_UP};

void sh110x_init(void);
void sh110x_displayOrientation(enum SH110X_SCREEN_ROT rotation);
void sh110x_displayMode(enum SH110X_DISPLAY_MODE mode);
void sh110x_update(void);
void sh110x_fill(enum SH110X_COLOR color);
void sh110x_drawPixel(uint8_t x, uint8_t y, enum SH110X_COLOR color);

#endif /* SRC_SH110X_H_ */
