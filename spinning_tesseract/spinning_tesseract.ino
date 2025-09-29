#include <SPI.h>
#include "Adafruit_GFX.h"
#include <MCUFRIEND_kbv.h>

MCUFRIEND_kbv tft;

#define BLACK 0x0000
#define WHITE 0xFFFF

// 16 Eckpunkte des Tesserakts (alle Kombinationen von ±1 in 4 Dimensionen)
float tesseract[16][4] = {
    {-1, -1, -1, -1}, {1, -1, -1, -1}, {1, 1, -1, -1}, {-1, 1, -1, -1}, {-1, -1, 1, -1}, {1, -1, 1, -1}, {1, 1, 1, -1}, {-1, 1, 1, -1}, {-1, -1, -1, 1}, {1, -1, -1, 1}, {1, 1, -1, 1}, {-1, 1, -1, 1}, {-1, -1, 1, 1}, {1, -1, 1, 1}, {1, 1, 1, 1}, {-1, 1, 1, 1}};

// Kanten: zwei Punkte sind verbunden, wenn sie sich in genau einer Koordinate unterscheiden
int edges[32][2];
int edgeCount = 0;

float angleXY = 0, angleXZ = 0, angleXW = 0;

void setupEdges()
{
    edgeCount = 0;
    for (int i = 0; i < 16; i++)
    {
        for (int j = i + 1; j < 16; j++)
        {
            int diff = 0;
            for (int k = 0; k < 4; k++)
            {
                if (tesseract[i][k] != tesseract[j][k])
                    diff++;
            }
            if (diff == 1)
            {
                edges[edgeCount][0] = i;
                edges[edgeCount][1] = j;
                edgeCount++;
            }
        }
    }
}

void rotate4D(float in[4], float out[4])
{
    // Kopie
    float x = in[0], y = in[1], z = in[2], w = in[3];

    // Rotation XY
    float x1 = x * cos(angleXY) - y * sin(angleXY);
    float y1 = x * sin(angleXY) + y * cos(angleXY);

    // Rotation XZ
    float x2 = x1 * cos(angleXZ) - z * sin(angleXZ);
    float z1 = x1 * sin(angleXZ) + z * cos(angleXZ);

    // Rotation XW
    float x3 = x2 * cos(angleXW) - w * sin(angleXW);
    float w1 = x2 * sin(angleXW) + w * cos(angleXW);

    out[0] = x3;
    out[1] = y1;
    out[2] = z1;
    out[3] = w1;
}


void setup()
{
    Serial.begin(9600);
    uint16_t ID = tft.readID();
    tft.begin(ID);
    tft.setRotation(1);
    tft.fillScreen(BLACK);

    setupEdges();
}

void loop()
{
    // tft.fillScreen(BLACK);
    int w = tft.width();
    int h = tft.height();

    angleXY += 0.03;
    angleXZ += 0.02;
    angleXW += 0.04;

    // Projektion 4D -> 3D -> 2D
    int projected[16][2];
    for (int i = 0; i < 16; i++)
    {
        float rotated[4];
        rotate4D(tesseract[i], rotated);

        // 4D -> 3D Projektion (W als Tiefe)
        float distance = 2.0;
        float wFactor = 1.0 / (distance - rotated[3]);

        float x3 = rotated[0] * wFactor;
        float y3 = rotated[1] * wFactor;
        float z3 = rotated[2] * wFactor;

        // 3D -> 2D Projektion
        float scale = 100;
        int px = (int)(x3 * scale) + w / 2;
        int py = (int)(y3 * scale) + h / 2;

        projected[i][0] = px;
        projected[i][1] = py;
    }

    // Kanten zeichnen
    for (int e = 0; e < edgeCount; e++)
    {
        int p1 = edges[e][0];
        int p2 = edges[e][1];
        tft.drawLine(projected[p1][0], projected[p1][1],
                     projected[p2][0], projected[p2][1],
                     WHITE);
    }

    delay(30);

    for (int i = 0; i < edgeCount; i++)
    {
        int p1 = edges[i][0];
        int p2 = edges[i][1];
        tft.drawLine(projected[p1][0], projected[p1][1],
                     projected[p2][0], projected[p2][1],
                     BLACK);
    }
}
