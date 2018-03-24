/*
 * Maker Space - Welcome Sign
 * 
 * - Arduino Pro Mini
 * - RCWL-0516 Doppler Radar Microwave Sensor
 * - 64 NeoPixels
 */

#include <Adafruit_NeoPixel.h>

#define PROXSENOR_PIN (11)
#define NEOPIXEL_PIN  (10)
#define NUMPIXELS     (64)

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

#define NUMBER_OF_POINTS (24)

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

    calculateNextState();

    clearPixels();
    displayPoints(proximity);

    pixels.show();

    digitalWrite(LED_BUILTIN, (proximity ? HIGH: LOW));

    delay(20);
}

void clearPixels(void)
{
    for (int pixelIndex = 0; pixelIndex < NUMPIXELS; pixelIndex++)
        pixels.setPixelColor(pixelIndex, pixels.Color(0, 0, 0));
}

void initPoints(void)
{
    for (int pointIndex = 0; pointIndex < NUMBER_OF_POINTS; pointIndex++)
       randonizePoint(pointIndex);
}

void randonizePoint(int pointIndex)
{
    points[pointIndex].color     = pixels.Color(random(0, 256), random(0, 256), random(0, 256));
    points[pointIndex].location  = random(0, NUMPIXELS);
    points[pointIndex].speed     = (random(0, 2) == 0 ? -1: 1);
    points[pointIndex].duration  = random(16, 32);
    points[pointIndex].displayed = (random(0, 2) == 0);
}

void displayPoints(boolean proximity)
{
    for (int pointIndex = 0; pointIndex < NUMBER_OF_POINTS; pointIndex++)
        if (points[pointIndex].displayed)
            pixels.setPixelColor(points[pointIndex].location, points[pointIndex].color);

    if (proximity)
        pixels.setBrightness(63);
    else
        pixels.setBrightness(255);
}

void calculateNextState(void)
{
    for (int pointIndex = 0; pointIndex < NUMBER_OF_POINTS; pointIndex++)
    {
        points[pointIndex].duration = points[pointIndex].duration - 1;

        if (points[pointIndex].duration == 0)
            randonizePoint(pointIndex);
        else
            points[pointIndex].location = ((points[pointIndex].location + NUMPIXELS) + points[pointIndex].speed) % NUMPIXELS;
    }
}

