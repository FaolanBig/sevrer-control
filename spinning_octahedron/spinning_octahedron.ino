#include <SPI.h>
#include "Adafruit_GFX.h"
#include <MCUFRIEND_kbv.h>

MCUFRIEND_kbv tft;

#define BLACK 0x0000
#define WHITE 0xFFFF

// 6 Eckpunkte des Oktaeders
float octa[6][3] = {
    {1, 0, 0},  // 0: rechts
    {-1, 0, 0}, // 1: links
    {0, 1, 0},  // 2: oben
    {0, -1, 0}, // 3: unten
    {0, 0, 1},  // 4: vorne
    {0, 0, -1}  // 5: hinten
};

// 12 Kanten (Index-Paare)
int edges[12][2] = {
    {0, 2}, {0, 3}, {0, 4}, {0, 5}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {2, 4}, {2, 5}, {3, 4}, {3, 5}};

float angleX = 0, angleY = 0;

void setup()
{
    Serial.begin(9600);
    uint16_t ID = tft.readID();
    tft.begin(ID);
    tft.setRotation(1); // Landscape
    tft.fillScreen(BLACK);
}

void loop()
{
    // tft.fillScreen(BLACK);

    int w = tft.width();
    int h = tft.height();

    // Rotationswinkel anpassen
    angleX += 0.05;
    angleY += 0.07;

    // Eckpunkte nach Rotation und Projektion
    int projected[6][2];
    for (int i = 0; i < 6; i++)
    {
        float x = octa[i][0];
        float y = octa[i][1];
        float z = octa[i][2];

        // Rotation um X
        float y1 = y * cos(angleX) - z * sin(angleX);
        float z1 = y * sin(angleX) + z * cos(angleX);

        // Rotation um Y
        float x2 = x * cos(angleY) + z1 * sin(angleY);
        float z2 = -x * sin(angleY) + z1 * cos(angleY);

        // einfache Projektion
        float scale = 120; // Größe
        int px = (int)(x2 * scale) + w / 2;
        int py = (int)(y1 * scale) + h / 2;

        projected[i][0] = px;
        projected[i][1] = py;
    }

    // Kanten zeichnen
    for (int e = 0; e < 12; e++)
    {
        int p1 = edges[e][0];
        int p2 = edges[e][1];
        tft.drawLine(projected[p1][0], projected[p1][1],
                     projected[p2][0], projected[p2][1],
                     WHITE);
    }

    //delay(30); // Geschwindigkeit

    for (int e = 0; e < 12; e++)
    {
        int p1 = edges[e][0];
        int p2 = edges[e][1];
        tft.drawLine(projected[p1][0], projected[p1][1],
                     projected[p2][0], projected[p2][1],
                     BLACK);
    }
}
