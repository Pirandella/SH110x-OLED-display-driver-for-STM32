#include "sh110x.h"

static enum SH110X_SCREEN_ROT _rotation = SCREEN_ROT_0;
static uint8_t _width = _SH110X_WIDHT_DEFAULT;
static uint8_t _height = _SH110X_HEIGHT_DEFAULT;
const uint8_t *const _sh110x_width = &_width;
const uint8_t *const _sh110x_height = &_height;
const uint8_t *const _sh110x_displayOrientation = &_rotation;

static const uint8_t _displayMode[4] = {0xA6, 0xA7, 0xAE, 0xAF};

/* Display frame buffer */
static uint8_t _buffer[_SH110X_PAGE_SIZE * _SH110X_NUMBER_OF_PAGES] = {0};

static const uint8_t init_cmd[24] = {
	0xAE,			// Turn off display
	0xDC, 0x00,		// Set display start line
	0x81, 0x2F,		// Set display contrast
	0x20,			// Page addressing mode (Horizontal addressing mode)
	0xA0,			// Segment re-map (Default portrait orientation)
	0xC0,			// Output scan direction (Default for portrait orientation)
	0xA8, 0x7F,		// Multiplexer ration
	0xD3, 0x60,		// Display offset
	0xD5, 0x51,		// Display clock divider
	0xD9, 0x22,		// Dis-charge / Pre-charge period
	0xDB, 0x35,		// VCOM deselect level
	0xB0,			// Set page address
	0xDA, 0x12,		// Common pads hardware configuration (Only for SH1106)
	0xA4,			// Force SH110x RAM to be displayed
	0xA6,			// Display mode (Normal)
	0xAF			// Turn on display
};

/* Set of command that goes before sending page of the frame buffer */
static uint8_t preamble[3] = {
	0x10,	// Column start address high nibble (0x10 | ((column >> 4) & 0x0F))
	0x00,	// Column start address low nibble (column & 0x0F)
	0xB0	// Page number (0 ... 15)
};

void sh110x_init()
{
	SH110X_CS_LOW;
	SH110X_RST_LOW;
	SH110X_RST_HI;
	SH110X_DC_LOW;
	HAL_SPI_Transmit(&SH110X_SPI_INTERFACE, (uint8_t*)init_cmd, sizeof(init_cmd), 100);
	SH110X_CS_HI;
}

void sh110x_displayOrientation(enum SH110X_SCREEN_ROT rotation)
{
	static uint8_t cmd[2];
	_rotation = rotation;
	switch(rotation) {
		case SCREEN_ROT_0:
			cmd[0] = 0xA0;
			cmd[1] = 0xC0;
			_width = _SH110X_WIDHT_DEFAULT;
			_height = _SH110X_HEIGHT_DEFAULT;
			break;
		case SCREEN_ROT_90:
			cmd[0] = 0xA0;
			cmd[1] = 0xC8;
			_width = _SH110X_HEIGHT_DEFAULT;
			_height = _SH110X_WIDHT_DEFAULT;
			break;
		case SCREEN_ROT_180:
			cmd[0] = 0xA1;
			cmd[1] = 0xC8;
			_width = _SH110X_WIDHT_DEFAULT;
			_height = _SH110X_HEIGHT_DEFAULT;
			break;
		case SCREEN_ROT_270:
			cmd[0] = 0xA1;
			cmd[1] = 0xC0;
			_width = _SH110X_HEIGHT_DEFAULT;
			_height = _SH110X_WIDHT_DEFAULT;
			break;
		default:
			return;
	}

	SH110X_CS_LOW;
	SH110X_DC_LOW;
	HAL_SPI_Transmit(&SH110X_SPI_INTERFACE, (uint8_t*)cmd, sizeof(cmd), 100);
	SH110X_DC_HI;
	SH110X_CS_HI;
}

void sh110x_displayMode(enum SH110X_DISPLAY_MODE mode)
{
	SH110X_CS_LOW;
	SH110X_DC_LOW;
	HAL_SPI_Transmit(&SH110X_SPI_INTERFACE, &_displayMode[mode], 1, 100);
	SH110X_DC_HI;
	SH110X_CS_HI;
}

void sh110x_update()
{
	SH110X_CS_LOW;

	for (uint8_t page = 0; page < _SH110X_NUMBER_OF_PAGES; page++) {
		preamble[2] = 0xB0 | page;

		SH110X_DC_LOW;
		HAL_SPI_Transmit(&SH110X_SPI_INTERFACE, (uint8_t*)preamble, sizeof(preamble), 100);
		SH110X_DC_HI;
		HAL_SPI_Transmit(&SH110X_SPI_INTERFACE, &_buffer[page * _SH110X_PAGE_SIZE], _SH110X_PAGE_SIZE, HAL_MAX_DELAY);
	}
	SH110X_CS_HI;
}

void sh110x_fill(enum SH110X_COLOR color)
{
	for (uint16_t i = 0; i < sizeof(_buffer); i++) _buffer[i] = color;
}

static void _drawPixel(uint8_t x, uint8_t y, enum SH110X_COLOR color)
{
	if (color)
		_buffer[x + (y / 8) * _SH110X_PAGE_SIZE] |= (1 << (y % 8));
	else
		_buffer[x + (y / 8) * _SH110X_PAGE_SIZE] &= ~(1 << (y % 8));
}

void sh110x_drawPixel(uint8_t x, uint8_t y, enum SH110X_COLOR color)
{
	if ((x >= _width) || (y >= _height)) return;

	if ((_rotation == SCREEN_ROT_0) || (_rotation == SCREEN_ROT_180)) {
		_drawPixel(x, y, color);
	} else {
		_drawPixel(y, x, color);
	}
}
