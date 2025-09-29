#include <SPI.h>          // f.k. for Arduino-1.5.2
#include "Adafruit_GFX.h" // Hardware-specific library
#include <MCUFRIEND_kbv.h>

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4

#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

unsigned int colors[] = {BLACK, BLUE, RED, GREEN, CYAN, MAGENTA, YELLOW, WHITE};

MCUFRIEND_kbv tft;

void setup()
{
    Serial.begin(9600);
    uint16_t ID = tft.readID();
    tft.begin(ID);
    tft.setRotation(1);
    //tft.fillScreen(BLACK);
    
    for (auto &&i : colors)
    {
        tft.fillScreen(i);
    }
    
}

void loop()
{
}