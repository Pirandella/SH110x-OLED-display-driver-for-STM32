# SH110x-OLED-display-driver-for-STM32
Basic __SH1106__/__SH1107__ OLED display driver written for __STM32__ MCU's using HAL. __SPI__ used as a communication interface. Only basic functionality is implemented. Additional GFX libraries can be use by passing _drawPixel_ and _fill_ functions to them.

## Driver capabilities
- Intialize OLED display in default orientation
- Change displays orientation (0&deg;, 90&deg;, 180&deg;, 270&deg;)
- Change display mode (Normal colors, Inverted colors, Sleep, Normal operation)
- Keeps track of display orientation, width and hight (available through constanst)
- Has function to put single pixel in buffer and function to fill whole buffer with single color
- Has function to write frame buffer into the screen (frame buffer that is not exposed to user)

## Setup
You may need to change following definitions inside _sh110x.h_ file.

### Defaul display resolution
Depending on display resolution you may need to change _default_ width and hight of the display.
``` C
#define _SH110X_WIDHT_DEFAULT					64
#define _SH110X_HEIGHT_DEFAULT					128
```
### Display page size and number
Depending on display resolution you may need to change __\_SH110X_PAGE_SIZE__ and __\_SH110X_NUMBER_OF_PAGES__.
- __\_SH110X_PAGE_SIZE__ value usually equels to display width in default orientation.
- __\_SH110X_NUMBER_OF_PAGES__ value usually equels to `display_height / 8` in default orientation.
> Refer to _[SH1107](https://www.displayfuture.com/Display/datasheet/controller/SH1107.pdf) Page 18 Fig 10(a)_ or _[SH1106](https://www.pololu.com/file/0J1813/SH1106.pdf) Page 16 Fig 8_ to see how display _RAM_ is mapped.
``` C
#define _SH110X_PAGE_SIZE						64
#define _SH110X_NUMBER_OF_PAGES					16
```

### SPI flow contro GPIO
Also you will need to change __CS__, __DC__ and __RST__ _HIGH_ and __LOW__ macros in case you named them not the same way as in _sh110x.h_ file.

> All of the above values was set with _Pico OLED 1.3 inch_ display module in mind.