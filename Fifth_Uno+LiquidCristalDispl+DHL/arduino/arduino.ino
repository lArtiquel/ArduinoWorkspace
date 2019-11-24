// include the library code:
#include <LiquidCrystal.h>
#include <DHT.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define DHT1PIN 9       // Digital pin connected to the DHT1 sensor
#define DHT2PIN 8       // Digital pin connected to the DHT1 sensor
#define DHT3PIN 7       // Digital pin connected to the DHT1 sensor
#define DHT4PIN 6       // Digital pin connected to the DHT1 sensor
#define DHTTYPE DHT11   // Type: DHT 11

DHT dht1(DHT1PIN, DHTTYPE);
DHT dht2(DHT2PIN, DHTTYPE);
DHT dht3(DHT3PIN, DHTTYPE);
DHT dht4(DHT4PIN, DHTTYPE);

const byte BUTTON_INPUT = 0;  // Button pin

int MAX_TEMP = 0;
int MAX_HUM = 0;

// actual sensor temperature
int TSENS1 = 0;
int TSENS2 = 0;
int TSENS3 = 0;
int TSENS4 = 0;

// actual sensor humidity
int HSENS1 = 0;
int HSENS2 = 0;
int HSENS3 = 0;
int HSENS4 = 0;

// this one contain number of frames before update, using 32, max 40 
int FRAMES_BEFORE_UPDATE = 0;


void checkMaximums(){
  if(TSENS1 > MAX_TEMP) MAX_TEMP = TSENS1;
  if(TSENS2 > MAX_TEMP) MAX_TEMP = TSENS2;
  if(TSENS3 > MAX_TEMP) MAX_TEMP = TSENS3;
  if(TSENS4 > MAX_TEMP) MAX_TEMP = TSENS4;

  if(HSENS1 > MAX_TEMP) MAX_HUM = HSENS1;
  if(HSENS2 > MAX_TEMP) MAX_HUM = HSENS2;
  if(HSENS3 > MAX_TEMP) MAX_HUM = HSENS3;
  if(HSENS4 > MAX_TEMP) MAX_HUM = HSENS4;
}

void setNANstoERRCodes(){
  // -1 is error code
  if(isnan(HSENS1)) HSENS1 = -1;
  if(isnan(HSENS2)) HSENS2 = -1;
  if(isnan(HSENS3)) HSENS3 = -1;
  if(isnan(HSENS4)) HSENS4 = -1;
  
  if(isnan(TSENS1)) TSENS1 = -1;
  if(isnan(TSENS2)) TSENS2 = -1;
  if(isnan(TSENS3)) TSENS3 = -1;
  if(isnan(TSENS4)) TSENS4 = -1;
}

void readInfoFromSensors(){
  // forcing to read humidity
  HSENS1 = dht1.readHumidity(true);
  HSENS2 = dht2.readHumidity(true);
  HSENS3 = dht3.readHumidity(true);
  HSENS4 = dht4.readHumidity(true);
  // reading Celcium temperature
  TSENS1 = dht1.readTemperature();
  TSENS2 = dht2.readTemperature();
  TSENS3 = dht3.readTemperature();
  TSENS4 = dht4.readTemperature();
}

void printInfoBlocs(){
  // clearing lcd once again to write info
  lcd.clear();
  lcd.home();
  
  // form blocs of Temp. data
  lcd.print("T.1=");  // 4 chars
  lcd.print(TSENS1);  // aprox 2 chars
  lcd.print("; ");    // 2 more
  
  lcd.print("T.2=");  
  lcd.print(TSENS2);      
  lcd.print("; ");
  
  lcd.print("T.3=");  
  lcd.print(TSENS3);      
  lcd.print("; ");    
  
  lcd.print("T.4=");  
  lcd.print(TSENS4);      
  lcd.print("; ");  
  
  // changing cursor position
  lcd.setCursor(0, 1);
  
  // form blocs of Humidity
  lcd.print("H.1=");  // 4 chars
  lcd.print(HSENS1);  // aprox 2 chars
  lcd.print("; ");    // 2 more
  
  lcd.print("H.2="); 
  lcd.print(HSENS2);          
  lcd.print("; ");     
  
  lcd.print("H.3=");  
  lcd.print(HSENS3);          
  lcd.print("; ");            
  
  lcd.print("H.4=");  
  lcd.print(HSENS4);          
  lcd.print("; ");
  delay(400);  
}


void setup() {
  // begin DHT's
  dht1.begin();
  dht2.begin();
  dht3.begin();
  dht4.begin();
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // greetings
  lcd.print(" Hello, Pups!!! ");
  delay(1000); 
  lcd.clear();
  lcd.home();
  // init button pin
  pinMode(BUTTON_INPUT, INPUT); //установка вывода №0 Arduino как входного порта
}

void loop() {
  if(digitalRead(BUTTON_INPUT) == HIGH){
    // cleaning input before
    lcd.clear();
    lcd.home();
    
    // printing Max. Temperature
    lcd.print("Max. Temp.: ");
    lcd.print(MAX_TEMP);
    lcd.setCursor(0, 1);
    // printing Max. Humidity
    lcd.print("Max. Humid.: ");
    lcd.print(MAX_HUM);
    delay(5000);
    
    // and cleaning once again
    lcd.clear();
    lcd.home();

    // update info
    FRAMES_BEFORE_UPDATE = 0;
  } else {
    if(FRAMES_BEFORE_UPDATE == 0){
      // updating
      lcd.clear();
      lcd.home();
      lcd.print("Updating info...");
      delay(2000);
      
      // reading info about Temp. and Hum.
      // usually taking 50 ms; but
      // can be delays up to 2 secs
      // (very slow hardware):

      // reading info from sensors
      readInfoFromSensors();
  
      // checking if info from hardware is correct and set 0 if NAN
      setNANstoERRCodes();

      // updating Temp and Humidity maximums
      checkMaximums();

      // updating num of sets of frames
      FRAMES_BEFORE_UPDATE = 32;

      // printing blocs of data
      printInfoBlocs();   
    } else {
      FRAMES_BEFORE_UPDATE--; // decrementing frames
      lcd.scrollDisplayLeft();// shifting data left
      delay(300);              // delaying
    }
  }
}
