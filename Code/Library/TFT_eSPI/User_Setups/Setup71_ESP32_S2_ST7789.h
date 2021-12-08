// Setup for the ESP32 S2 with ST7789 display
// Note SPI DMA with ESP32 S2 is not currently supported

// See SetupX_Template.h for all options available

#define ST7789_DRIVER     // Configure all registers


#define TFT_CS   5 //     10 or 34
#define TFT_MOSI 37 //     11 or 35
#define TFT_SCLK 36 //     12 or 36
#define TFT_MISO 38 //     13 or 37

#define TFT_DC   6
#define TFT_RST  4

#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFF

#define SMOOTH_FONT

#define SPI_FREQUENCY   27000000

#define SPI_TOUCH_FREQUENCY  2500000