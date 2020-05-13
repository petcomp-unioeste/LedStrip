// Use if you want to force the software SPI subsystem to be used for some reason (generally, you don't)
// #define FASTLED_FORCE_SOFTWARE_SPI
// Use if you want to force non-accelerated pin access (hint: you really don't, it breaks lots of things)
// #define FASTLED_FORCE_SOFTWARE_SPI
// #define FASTLED_FORCE_SOFTWARE_PINS
#include <FastLED.h>

///////////////////////////////////////////////////////////////////////////////////////////
//
// Move a white dot along the strip of leds.  This program simply shows how to configure the leds,
// and then how to turn a single pixel white and then off, moving down the line of pixels.
// 

// How many leds are in the strip?
#define NUM_LEDS 4

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN 4
#define CLOCK_PIN 5

// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];


const byte ledPin = 13;
const byte button1 = 2;
const byte button2 = 3;
volatile int count_button1=0;
volatile int count_button2=0;
int bt1Led= 0;
int bt2Led= NUM_LEDS-1;


// This function sets up the ledsand tells the controller about them
void setup() {
    Serial.begin(115200);
	// sanity check delay - allows reprogramming if accidently blowing power w/leds
   	delay(2000);
 
    pinMode(ledPin, OUTPUT);

    pinMode(button1, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(button1), button1Func, CHANGE);

    pinMode(button2, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(button2), button2Func, CHANGE);


    // Uncomment/edit one of the following lines for your leds arrangement.
    // ## Clockless types ##
    // FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
    // FastLED.addLeds<SM16703, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<TM1829, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<TM1812, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<TM1809, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<TM1804, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<TM1803, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<UCS1903, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<UCS1903B, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<UCS1904, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<UCS2903, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
    // FastLED.addLeds<WS2852, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
    // FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
    // FastLED.addLeds<GS1903, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<SK6812, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
    // FastLED.addLeds<SK6822, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<APA106, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<PL9823, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<SK6822, DATA_PIN, RGB>(leds, NUM_LEDS);
    FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<WS2813, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<APA104, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<WS2811_400, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<GE8822, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<GW6205, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<GW6205_400, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<LPD1886, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<LPD1886_8BIT, DATA_PIN, RGB>(leds, NUM_LEDS);
    // ## Clocked (SPI) types ##
    // FastLED.addLeds<LPD6803, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
    // FastLED.addLeds<LPD8806, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
    // FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<WS2803, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<SM16716, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<P9813, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // BGR ordering is typical
    // FastLED.addLeds<DOTSTAR, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // BGR ordering is typical
    // FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // BGR ordering is typical
    // FastLED.addLeds<SK9822, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // BGR ordering is typical
      leds[bt1Led] = CRGB::Green;
      leds[bt2Led] = CRGB::Purple;
      // Show the leds (only one of which is set to white, from above)
      FastLED.show();
}


void button1Func() {
 static unsigned long last_interrupt_time_bt1 = 0;
 unsigned long interrupt_time_bt1 = millis();
 // If interrupts come faster than 200ms, assume it's a bounce and ignore
 if (interrupt_time_bt1 - last_interrupt_time_bt1 > 50)
 {
   count_button1++;
 }
 last_interrupt_time_bt1 = interrupt_time_bt1;
}

void button2Func() {
 static unsigned long last_interrupt_time_bt2 = 0;
 unsigned long interrupt_time_bt2 = millis();
 // If interrupts come faster than 200ms, assume it's a bounce and ignore
 if (interrupt_time_bt2 - last_interrupt_time_bt2 > 50)
 {
   count_button2++;
 }
 last_interrupt_time_bt2 = interrupt_time_bt2;
}


int whiteLed=0;
int winner=0;
// This function runs over and over, and is where you do the magic to light
// your leds.
void loop() {
   // Move a single white led 

                          //BBRRGG
      leds[bt1Led] = 0x00FF00;
      leds[bt2Led] = 0xFF0000;
      if (bt1Led== bt2Led) leds[bt2Led]= 0x0000FF;
      // Show the leds (only one of which is set to white, from above)
      FastLED.show();
      
      if (count_button1 >= 20){ 
          count_button1=0;
          leds[bt1Led] = CRGB::Black;
          bt1Led++;
          if (bt1Led >= NUM_LEDS && winner==0) {                     
             winner=1;        
             bt1Led= 0;     
          }   
          //leds[bt1Led] = CRGB::White;             
      }  

      if (count_button2 >= 20){ 
          count_button2=0;
          leds[bt2Led] = CRGB::Black;
          bt2Led--;
          if (bt2Led < 0 && winner == 0) {                     
             winner=2;     
             bt2Led= NUM_LEDS-1;        
          }   
          
      }  
      
      //delay(100);
      

      if (winner !=0) {
            if (winner==1) {
              for (int j=0; j <10; j++){
                for (int i=0; i < NUM_LEDS;i++)
                   leds[i]= 0x00FF00;                
                   FastLED.show();
                   delay(250);
                for (int i=0; i < NUM_LEDS;i++)
                   leds[i]= 0x000000;
                   FastLED.show();
                   delay(250);  

              }              
                                   
            } else {
              for (int j=0; j <10; j++){
                for (int i=0; i < NUM_LEDS;i++)
                   leds[i]= 0xFF0000;                
                   FastLED.show();
                   delay(250);
                for (int i=0; i < NUM_LEDS;i++)
                   leds[i]= 0x000000;
                   FastLED.show();
                   delay(250);  

              }              

            }
            winner=0;
            count_button2=0;
            count_button1=0;
            bt1Led=0;
            bt2Led=NUM_LEDS-1;
            
      }
              

        
     
      
      
      // Turn our current led back to black for the next loop around
      
}
