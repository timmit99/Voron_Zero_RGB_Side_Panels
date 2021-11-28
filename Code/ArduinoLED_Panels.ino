/*
  Voron Zero WS2812 Side panel control code
  Timothy Abraham - 11/27/2021


  Connect 4 bits of data between pins 3(MSB) -> 6(LSB)
  Connect clock pin to digital 2
  Connect Neopixel panel(s) to pin 8


  Patterns used from Adafruit Neopixel example code
*/
#include <Adafruit_NeoPixel.h>
#define LED_PIN    8
// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 16

int data0 = 6; //LSB
int data1 = 5;
int data2 = 4;
int data3 = 3; //MSB
int clockPin = 2;
int data = 0;
int dataChange = 0;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // make the data pins inputs:
  pinMode(data0, INPUT);
  pinMode(data1, INPUT);
  pinMode(data2, INPUT);
  pinMode(data3, INPUT);
  pinMode(clockPin, INPUT_PULLUP); // and add pullup to clock pin
  attachInterrupt(digitalPinToInterrupt(clockPin), getData, FALLING);
  //attach a falling edge trigger interupt ot the clock pin

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(150); // Set BRIGHTNESS to about 1/5 (max = 255)

  // Flash orange to show everything is working correctly!
  colorWipe(strip.Color(255,   50,   0), 40); // Orange
  colorWipe(strip.Color(0,   0,   0), 40); // Off
  colorWipe(strip.Color(255,   50,   0), 40); // Orange
  colorWipe(strip.Color(0,   0,   0), 40); // Off
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input pin:
  if (dataChange) { // If the interupt is triggered and the data has changed
    Serial.println(data); // Print the input data out over serial (for debugging)
    dataChange = 0; // reset dataChange to 0 to signal that the data has been read in
  }

  switch (data) {
    case 1:
      colorWipe(strip.Color( 255,   0,   0), 50); // Red
      break;
    case 2:
      colorWipe(strip.Color( 255, 120,   0), 50); // Orange
      break;
    case 3:
      colorWipe(strip.Color( 255, 255,   0), 50); // Yellow
      break;
    case 4:
      colorWipe(strip.Color(   0, 255,   0), 50); // Green
      break;
    case 5:
      colorWipe(strip.Color(   0,   0, 255), 50); // Blue
      break;
    case 6:
      colorWipe(strip.Color( 255,   0, 255), 50); // Purple
      break;
    case 7:
      colorWipe(strip.Color( 255, 255, 255), 50); // White
      break;
    case 8:
      christmasLights(750); // flash red and green on alternate pixels
      break;
    case 15:
      rainbow(10);
      break;
    default:
      colorWipe(strip.Color( 0, 0, 0), 50); // Off (for anything that ISN'T defined above
      break;
      delay(1);        // delay in between reads for stability
  }
}

void getData() {
  //BitWrite the data from the 4 data pins into the "data" intiger
  bitWrite(data, 0, digitalRead(data0));
  bitWrite(data, 1, digitalRead(data1));
  bitWrite(data, 2, digitalRead(data2));
  bitWrite(data, 3, digitalRead(data3));
  dataChange = 1; // set the data change flag to triger on the next loop iteration and break from any loops in the patterns
}

void colorWipe(uint32_t color, int wait) {
  for (int i = 0; i < strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    if (dataChange) {
      return;
    }
    delay(wait);                           //  Pause for a moment
  }
}

void theaterChase(uint32_t color, int wait) {
  for (int a = 0; a < 10; a++) { // Repeat 10 times...
    for (int b = 0; b < 3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for (int c = b; c < strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
        if (dataChange) {
          return;
        }
      }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}

void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for (long firstPixelHue = 0; firstPixelHue < 5 * 65536; firstPixelHue += 256) {
    for (int i = 0; i < strip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
      if (dataChange) {
        return;
      }
    }
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

void christmasLights(int wait) {
  for (int i = 0; i < strip.numPixels(); i += 2) {
    strip.setPixelColor(i,     strip.Color( 255,   0,   0)); // Set every even pixel to red.
    strip.setPixelColor(i + 1, strip.Color(   0, 255,   0)); // Set every odd pixel to green
    if (dataChange) {
      return;
    }
  }
  strip.show(); // Update strip with new contents
  delay(wait);  // Pause for a moment
  for (int i = 0; i < strip.numPixels(); i += 2) {
    strip.setPixelColor(i,     strip.Color(   0, 255,   0)); // Set every even pixel to green.
    strip.setPixelColor(i + 1, strip.Color( 255,   0,   0)); // Set every odd pixel to red
    if (dataChange) {
      return;
    }

  }
  strip.show(); // Update strip with new contents
  delay(wait);  // Pause for a moment
}
