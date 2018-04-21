/*
 * Maker Space - Welcome Sign
 * 
 * - Arduino Pro Mini, 5V
 * - RCWL-0516 Doppler Radar Microwave Sensor
 * - 64 NeoPixels
 */

#include <Adafruit_NeoPixel.h>

#define PROXSENOR_PIN (11)
#define NEOPIXEL_PIN  (10)
#define NUMPIXELS     (64)

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

#define NUMBER_OF_POINTS (32)

struct Point
{
    uint32_t color;
    uint8_t  location;
    uint8_t  speed;
    uint8_t  duration;
    boolean  displayed;
};

Point points[NUMBER_OF_POINTS];

void setup(void)
{
    pinMode(PROXSENOR_PIN, INPUT);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    initPoints();

    clearPixels();

    pixels.begin();
    pixels.show();
}

void loop(void)
{
    boolean proximity = digitalRead(PROXSENOR_PIN) == LOW;

    calculateNextState(proximity);

    clearPixels();
    displayPoints();

    pixels.show();

    digitalWrite(LED_BUILTIN, (proximity ? HIGH: LOW));

    delay(100);
}

void clearPixels(void)
{
    for (int pixelIndex = 0; pixelIndex < NUMPIXELS; pixelIndex++)
        pixels.setPixelColor(pixelIndex, pixels.Color(0, 0, 0));
}

void initPoints(void)
{
    for (int pointIndex = 0; pointIndex < NUMBER_OF_POINTS; pointIndex++)
       randonizePoint(pointIndex, true);
}

void randonizePoint(int pointIndex, boolean proximity)
{
    points[pointIndex].color     = randonizeColour(proximity);
    points[pointIndex].location  = random(0, NUMPIXELS);
    points[pointIndex].speed     = (random(0, 2) == 0 ? -1: 1);
    points[pointIndex].duration  = random(16, 32);
    points[pointIndex].displayed = true;
}

byte farColourPallet[12][3]  = { { 255, 0, 0 }, { 0, 255, 0 }, { 0, 0, 255 }, { 255, 31, 31 }, { 31, 255, 31 }, { 31, 31, 255 },
                                 { 191, 0, 0 }, { 191, 0, 0 }, { 191, 0, 0 }, { 191, 31, 31 }, { 31, 191, 31 }, { 31, 31, 191 } };

byte closeColourPallet[8][3] = { { 0, 255, 0 }, { 31, 255, 0 }, { 0, 255, 31 }, { 31, 255, 31 },
                                 { 0, 191, 0 }, { 31, 191, 0 }, { 0, 191, 31 }, { 31, 191, 31 } };

uint32_t randonizeColour(boolean proximity)
{
    if (proximity)
    {
        int palletIndex = random(0, 8);

        return pixels.Color(farColourPallet[palletIndex][0], closeColourPallet[palletIndex][1], closeColourPallet[palletIndex][2]);
    }
    else
    {
        int palletIndex = random(0, 12);

        return pixels.Color(closeColourPallet[palletIndex][0], farColourPallet[palletIndex][1], farColourPallet[palletIndex][2]);
    }
}

void displayPoints(void)
{
    for (int pointIndex = 0; pointIndex < NUMBER_OF_POINTS; pointIndex++)
        if (points[pointIndex].displayed)
            pixels.setPixelColor(points[pointIndex].location, points[pointIndex].color);

    pixels.setBrightness(32);
}

void calculateNextState(boolean proximity)
{
    for (int pointIndex = 0; pointIndex < NUMBER_OF_POINTS; pointIndex++)
    {
        points[pointIndex].duration = points[pointIndex].duration - 1;

        if (points[pointIndex].duration == 0)
            randonizePoint(pointIndex, proximity);
        else
            points[pointIndex].location = ((points[pointIndex].location + NUMPIXELS) + points[pointIndex].speed) % NUMPIXELS;
    }
}

