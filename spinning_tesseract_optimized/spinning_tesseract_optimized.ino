#include <SPI.h>
#include "Adafruit_GFX.h"
#include <MCUFRIEND_kbv.h>

MCUFRIEND_kbv tft;

#define BLACK 0x0000
#define WHITE 0xFFFF

// 16 Eckpunkte (±1 in 4 Dimensionen)
const float tesseract[16][4] = {
    {-1, -1, -1, -1}, {1, -1, -1, -1}, {1, 1, -1, -1}, {-1, 1, -1, -1}, {-1, -1, 1, -1}, {1, -1, 1, -1}, {1, 1, 1, -1}, {-1, 1, 1, -1}, {-1, -1, -1, 1}, {1, -1, -1, 1}, {1, 1, -1, 1}, {-1, 1, -1, 1}, {-1, -1, 1, 1}, {1, -1, 1, 1}, {1, 1, 1, 1}, {-1, 1, 1, 1}};

// 32 Kanten (von Hand definiert)
const uint8_t edges[32][2] = {
    {0, 1}, {1, 2}, {2, 3}, {3, 0}, {4, 5}, {5, 6}, {6, 7}, {7, 4}, {0, 4}, {1, 5}, {2, 6}, {3, 7}, {8, 9}, {9, 10}, {10, 11}, {11, 8}, {12, 13}, {13, 14}, {14, 15}, {15, 12}, {8, 12}, {9, 13}, {10, 14}, {11, 15}, {0, 8}, {1, 9}, {2, 10}, {3, 11}, {4, 12}, {5, 13}, {6, 14}, {7, 15}};

float angleXY = 0, angleXZ = 0, angleXW = 0;

void setup()
{
    uint16_t ID = tft.readID();
    tft.begin(ID);
    tft.setRotation(1);
    tft.fillScreen(BLACK);
}

void loop()
{
    // tft.fillScreen(BLACK);

    int w = tft.width();
    int h = tft.height();

    angleXY += 0.03f;
    angleXZ += 0.02f;
    angleXW += 0.04f;

    int projected[16][2];

    for (int i = 0; i < 16; i++)
    {
        float x = tesseract[i][0];
        float y = tesseract[i][1];
        float z = tesseract[i][2];
        float w4 = tesseract[i][3];

        // --- 4D Rotationen ---
        // XY
        float x1 = x * cos(angleXY) - y * sin(angleXY);
        float y1 = x * sin(angleXY) + y * cos(angleXY);

        // XZ
        float x2 = x1 * cos(angleXZ) - z * sin(angleXZ);
        float z1 = x1 * sin(angleXZ) + z * cos(angleXZ);

        // XW
        float x3 = x2 * cos(angleXW) - w4 * sin(angleXW);
        float w1 = x2 * sin(angleXW) + w4 * cos(angleXW);

        // --- Projektion 4D → 3D ---
        float wFactor = 1.0f / (2.0f - w1); // perspektivische Verzerrung
        float x3d = x3 * wFactor;
        float y3d = y1 * wFactor;
        float z3d = z1 * wFactor;

        // --- Projektion 3D → 2D ---
        const float scale = 50.0f; // halbe Größe (vorher 100)
        projected[i][0] = (int)(x3d * scale) + w / 2;
        projected[i][1] = (int)(y3d * scale) + h / 2;
    }

    // Kanten zeichnen
    for (int e = 0; e < 32; e++)
    {
        int p1 = edges[e][0];
        int p2 = edges[e][1];
        tft.drawLine(projected[p1][0], projected[p1][1],
                     projected[p2][0], projected[p2][1],
                     WHITE);
    }

    delay(30); // schneller Refresh

    for (int e = 0; e < 32; e++)
    {
        int p1 = edges[e][0];
        int p2 = edges[e][1];
        tft.drawLine(projected[p1][0], projected[p1][1],
                     projected[p2][0], projected[p2][1],
                     BLACK);
    }
}
