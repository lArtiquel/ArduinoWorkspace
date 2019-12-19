#include <SPI.h>
#include <Wire.h>
#include <DS3231.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 8
#define TEMPERATURE_PRECISION 9  // temp precision (9 ... 12)

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
byte TempSensorsCount = 0;
float T = 0.0;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
                         OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

DS3231 clock;
RTCDateTime dt;

const byte LED1 = 1;
const byte LED2 = 2;

byte DISPLAY_MODE = 1;  // 1 - to display current time and 1st temp sensor's data
            // 2 - same as 1st but 2nd sensor's data
            // 3 - same as 1st but 3rd sensor's data
            // 4 - alarm1 mode: blinking led1 and time on display ( Every 5s in each minute)
            // 5 - alarm1 mode: blinking led2 and time on display ( Each minute)
            
byte FRAMES_TO_BLINK_LED_1 = 0;   // these are indicating how long time and led will be blinking
byte FRAMES_TO_BLINK_LED_2 = 0;

/* Time Functionality */
//////////////////////////////////////////////////////////////////////
void getTime(){                     // retrieves time
  dt = clock.getDateTime();
}
// to draw time on display
void drawTime(){                    // draw time only
  // this one only for drawing time
  display.setTextSize(2);           
  display.setCursor(24, 20);
  display.print(dt.hour);
  display.print(":");
  display.print(dt.minute);
  display.print(":");
  display.print(dt.second);
}
void displayNewTime(){              // gets new time and displays it
  // this one retrieves new time and draws it
  getTime();
  drawTime();
}
//////////////////////////////////////////////////////////////////////

/* Temperature Functionality */
//////////////////////////////////////////////////////////////////////
void getTemperature(){                      // retrieve temp by Display Mode index
    sensors.requestTemperatures();              // calling temp sensor to issue a global temperature
    T = sensors.getTempCByIndex(DISPLAY_MODE-1);// temp in Celsius degrees of current Display Mode Sensor
}
// to draw dispaly mode temp on display
void drawTemperature(){                     // draw temp on screen
    // this one only for drawing time
    display.setTextSize(1);
    display.setCursor(0, 54);
    display.print("Temp " + String(DISPLAY_MODE) + " = ");
    display.print(String(T) + " ");
    display.print((char)247);
    display.print("C");
}
void displayTemperature(){                  // gets new info from Display Mode sensor and displays it
    // this one retrieves new time and draws it
    getTemperature();
    drawTemperature();
}
//////////////////////////////////////////////////////////////////////

          
void setup() {

  // startup Adafruit display lib on SSD1306
  display.begin(SSD1306_SWITCHCAPVCC);
  // print some Greetings to me
  display.setTextSize(1);                
  display.setTextColor(BLACK, WHITE);    // setting inversed text
  display.setCursor(5, 20);
  display.print("Oh, shit, here we go again...");
  display.display();
  delay(1000);
  display.clearDisplay();
  display.setCursor(32, 20);
  display.print("Hello, Pups!");
  display.display();
  delay(1000);
  display.clearDisplay();
  display.setTextColor(WHITE);          // setting normal text

  // startup DS3231 lib
  clock.begin();
  // Disarm alarms and clear alarms for this example, because alarms is battery backed.
  // Under normal conditions, the settings should be reset after power and restart microcontroller.
  clock.armAlarm1(false);
  clock.armAlarm2(false);
  clock.clearAlarm1();
  clock.clearAlarm2();
  // Manual (Year, Month, Day, Hour, Minute, Second)
  clock.setDateTime(2019, 10, 10, 10, 10, 1);
  // setting up two alarms
  clock.setAlarm1(0, 0, 0, 5, DS3231_MATCH_S);
  clock.setAlarm2(0, 0, 0, 0, DS3231_EVERY_MINUTE);

  // startup the temp-re sensor library
  sensors.begin();    
  // getting number of connected T sensors
  TempSensorsCount = sensors.getDeviceCount();
  
  // bind leds with arduino pins
  pinMode (LED1, OUTPUT);
  pinMode (LED2, OUTPUT);
}


void loop() {
  display.clearDisplay();     
  
  // checking alarms
  if (clock.isAlarm1(false))
  {
    DISPLAY_MODE = 4;
    FRAMES_TO_BLINK_LED_1 = 5;
    clock.clearAlarm1();
  }
  if (clock.isAlarm2(false))
  {
    DISPLAY_MODE = 5;
    FRAMES_TO_BLINK_LED_2 = 5;
    clock.clearAlarm2();
  }
  
  switch(DISPLAY_MODE){
    
    case 1: {
      displayTemperature();
      DISPLAY_MODE = 2;
      break;
    }
    
    case 2: {
      displayTemperature();   
      DISPLAY_MODE = 3;
      break;
    }
    
    case 3: {
      displayTemperature();
      DISPLAY_MODE = 1;
      break;
    }
    
    case 4: {
      if(FRAMES_TO_BLINK_LED_2 != 0 ) {   // blinking with second led if it's his time too(in this situation it'll work cuz they are both blinking for 5 frames)
        if(FRAMES_TO_BLINK_LED_2%2 == 1){  // firing only on odd num of frame
          digitalWrite (LED2, HIGH); 
        } else {
          digitalWrite (LED2, LOW); 
        }
        FRAMES_TO_BLINK_LED_2--;
      } else {
        digitalWrite (LED2, LOW); 
      }
      
      if(FRAMES_TO_BLINK_LED_1 != 0 ) {   // blinking with first led
        if(FRAMES_TO_BLINK_LED_1%2 == 1){ // firing only on odd num of frame
          digitalWrite (LED1, HIGH);        
        } else {
          digitalWrite (LED1, LOW); 
        }
        FRAMES_TO_BLINK_LED_1--;
      } else {
        DISPLAY_MODE = 1;       // if frames are ended -> switch display mode #1
        digitalWrite (LED1, LOW); 
      }
      break;
    }
    
    case 5: {
      if(FRAMES_TO_BLINK_LED_1 != 0 ) {   // same situation as in 4th case
        if(FRAMES_TO_BLINK_LED_1%2 == 1){ 
          digitalWrite (LED1, HIGH); 
        } else {
          digitalWrite (LED1, LOW); 
        }
        FRAMES_TO_BLINK_LED_1--;
      } else digitalWrite (LED1, LOW); 
      
      if(FRAMES_TO_BLINK_LED_2 != 0 ) {   // blinking with second led
        if(FRAMES_TO_BLINK_LED_2%2 == 1){ // firing only on odd num of frame
          digitalWrite (LED2, HIGH); 
        } else {
          digitalWrite (LED2, LOW); 
        }
        FRAMES_TO_BLINK_LED_2--;
      } else {
        DISPLAY_MODE = 1;       // if frames are ended -> switch display modes
        digitalWrite (LED2, LOW);
      }
      break;
    } 
  }
  displayNewTime();         // always display fresh time
  display.display();
  delay (950);
}
