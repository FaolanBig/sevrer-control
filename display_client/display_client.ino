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

// #############################################################################
// 8 Eckpunkte des Würfels
float cube[8][3] = {
  {-1, -1, -1},
  { 1, -1, -1},
  { 1,  1, -1},
  {-1,  1, -1},
  {-1, -1,  1},
  { 1, -1,  1},
  { 1,  1,  1},
  {-1,  1,  1}
};

// Kanten (Index-Paare der Punkte)
int edges[12][2] = {
  {0,1}, {1,2}, {2,3}, {3,0},   // hinten
  {4,5}, {5,6}, {6,7}, {7,4},   // vorne
  {0,4}, {1,5}, {2,6}, {3,7}    // Verbindungen
};

float angleX = 0, angleY = 0;
// #############################################################################

void setup()
{
    Serial.begin(9600);
    uint16_t ID = tft.readID();
    tft.begin(ID);
    tft.setRotation(1);
    tft.fillScreen(BLACK);
    
    /*for (auto &&i : colors)
    {
        tft.fillScreen(i);
    }
    delay(1000);
    tft.fillScreen(BLACK);

    for (int i = 0; i < 100; i+10)
    {
        tft.drawRect(i, i, 320 - 2 * i, 240 - 2 * i, WHITE);
        delay(100);
    }
    */
}

void loop()
{
    // #########################################################################
    tft.fillScreen(BLACK);

  int w = tft.width();
  int h = tft.height();

  // Rotationswinkel anpassen
  angleX += 0.05;
  angleY += 0.07;

  // Eckpunkte nach Rotation und Projektion
  int projected[8][2];
  for (int i = 0; i < 8; i++) {
    float x = cube[i][0];
    float y = cube[i][1];
    float z = cube[i][2];

    // Rotation um X
    float y1 = y * cos(angleX) - z * sin(angleX);
    float z1 = y * sin(angleX) + z * cos(angleX);

    // Rotation um Y
    float x2 = x * cos(angleY) + z1 * sin(angleY);
    float z2 = -x * sin(angleY) + z1 * cos(angleY);

    // einfache Projektion (orthographisch, mit Skalierung)
    float scale = 80;   // Würfelgröße
    int px = (int)(x2 * scale) + w / 2;
    int py = (int)(y1 * scale) + h / 2;

    projected[i][0] = px;
    projected[i][1] = py;
  }

  // Kanten zeichnen
  for (int e = 0; e < 12; e++) {
    int p1 = edges[e][0];
    int p2 = edges[e][1];
    tft.drawLine(projected[p1][0], projected[p1][1],
                 projected[p2][0], projected[p2][1],
                 WHITE);
  }

  delay(30); // Geschwindigkeit
    // #########################################################################
}