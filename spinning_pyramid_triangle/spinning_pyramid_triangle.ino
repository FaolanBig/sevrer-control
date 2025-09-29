#include <SPI.h>
#include "Adafruit_GFX.h"
#include <MCUFRIEND_kbv.h>

MCUFRIEND_kbv tft;

#define BLACK   0x0000
#define WHITE   0xFFFF

// 4 Eckpunkte des Tetraeders
float tetra[4][3] = {
  { 0,  1, -1},   // 0: vorne
  {-1, -1, -1},   // 1: hinten links
  { 1, -1, -1},   // 2: hinten rechts
  { 0,  0,  1}    // 3: Spitze
};

// 6 Kanten (Index-Paare)
int edges[6][2] = {
  {0,1}, {1,2}, {2,0},   // Basis-Dreieck
  {0,3}, {1,3}, {2,3}    // Seitenflächen zur Spitze
};

float angleX = 0, angleY = 0;

void setup() {
  Serial.begin(9600);
  uint16_t ID = tft.readID();
  tft.begin(ID);
  tft.setRotation(1);        // Landscape
  tft.fillScreen(BLACK);
}

void loop() {
  //tft.fillScreen(BLACK);

  int w = tft.width();
  int h = tft.height();

  // Rotationswinkel anpassen
  angleX += 0.05;
  angleY += 0.07;

  // Eckpunkte nach Rotation und Projektion
  int projected[4][2];
  for (int i = 0; i < 4; i++) {
    float x = tetra[i][0];
    float y = tetra[i][1];
    float z = tetra[i][2];

    // Rotation um X
    float y1 = y * cos(angleX) - z * sin(angleX);
    float z1 = y * sin(angleX) + z * cos(angleX);

    // Rotation um Y
    float x2 = x * cos(angleY) + z1 * sin(angleY);
    float z2 = -x * sin(angleY) + z1 * cos(angleY);

    // einfache Projektion
    float scale = 120;   // Größe
    int px = (int)(x2 * scale) + w / 2;
    int py = (int)(y1 * scale) + h / 2;

    projected[i][0] = px;
    projected[i][1] = py;
  }

  // Kanten zeichnen
  for (int e = 0; e < 6; e++) {
    int p1 = edges[e][0];
    int p2 = edges[e][1];
    tft.drawLine(projected[p1][0], projected[p1][1],
                 projected[p2][0], projected[p2][1],
                 WHITE);
  }

  delay(30); // Geschwindigkeit
    for (int e = 0; e < 6; e++) {
        int p1 = edges[e][0];
        int p2 = edges[e][1];
        tft.drawLine(projected[p1][0], projected[p1][1],
                     projected[p2][0], projected[p2][1],
                     BLACK);
    }
}
