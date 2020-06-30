

//www.elegoo.com
//2016.12.9

#include <LiquidCrystal.h>
#include <dht_nonblocking.h>
#include <RTClib.h>
#define DHT_SENSOR_TYPE DHT_TYPE_11

// define pins
#define BLUE 3
#define GREEN 5
#define RED 6


// define variables
int redValue;
int greenValue;
int blueValue;
float desirable_temp = 70;
float hypo = 68;
static const int DHT_SENSOR_PIN = 2;
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );
int buzzer = 13;//the pin of the active buzzer
//                BS  E  D4 D5  D6 D7
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup()
{
  /*lcd.begin(cols, rows)
    lcd: a variable of type LiquidCrystal
    cols: the number of columns that the display has
    rows: the number of rows that the display has 
  */ 
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("    WELCOME!    ");
  delay(4000);
  lcd.setCursor(0, 0);
  lcd.print("Temp          F ");
  lcd.setCursor(0, 1);
  lcd.print("Humid         % ");
  Serial.begin( 9600);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);
  spectrum();
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);
}



void greenLight()
{
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, LOW);
}

void redLight()
{
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);
}

void blueLight()
{
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, HIGH);
}



void breatheColor(String color)
{
  if(color == "RED")
  {
    for(int i = 0; i < 255; i++)
    {
        analogWrite(RED,i);
        delay(10);
    }

    for(int i = 255; i > 0; i--)
    {
        analogWrite(RED,i);
        delay(10);
    }
  }
  else if(color == "GREEN")
  {
    for(int i = 0; i < 255; i++)
    {
        analogWrite(GREEN,i);
        delay(10);
    }
    for(int i = 255; i > 0; i--)
    {
        analogWrite(GREEN,i);
        delay(10);
    }
  }
  else if(color == "BLUE")
  {
    for(int i = 0; i < 255; i++)
    {
        analogWrite(BLUE,i);
        delay(10);
    }
    for(int i = 255; i > 0; i--)
    {
        analogWrite(BLUE,i);
        delay(10);
    }
  }
}

void flashColor(String color)
{
  if(color == "RED")
  {
    redLight();
    delay(1000);
    digitalWrite(RED, LOW);
    delay(1000);
    redLight();
    delay(1000);
    digitalWrite(RED, LOW);
    delay(1000);
    redLight();
    delay(1000);
    digitalWrite(RED, LOW);
  }
  else if(color == "GREEN")
  {
    greenLight();
    delay(1000);
    digitalWrite(GREEN, LOW);
    delay(1000);
    greenLight();
    delay(1000);
    digitalWrite(GREEN, LOW);
    delay(1000);
    greenLight();
    delay(1000);
    digitalWrite(GREEN, LOW);
  }
  else if(color == "BLUE")
  {
    blueLight();
    delay(1000);
    digitalWrite(BLUE, LOW);
    delay(1000);
    blueLight();
    delay(1000);
    digitalWrite(BLUE, LOW);
    delay(1000);
    blueLight();
    delay(1000);
    digitalWrite(BLUE, LOW);
  }
}

void check_temp(float temp)
{
  if(temp >= 72)
  {
    //redLight();
    //flashColor("RED");
    breatheColor("RED");
  }
  else if(temp <=67)
  {
    //blueLight();
    //flashColor("BLUE");
    breatheColor("BLUE");
  }
  else
  {
    //greenLight();
    //flashColor("GREEN");
    breatheColor("GREEN");
  }  
}

void spectrum()
{
    #define delayTime 10 // fading time between colors
  
    redValue = 255; // choose a value between 1 and 255 to change the color.
    greenValue = 0;
    blueValue = 0;
  
    // this is unnecessary as we've either turned on RED in SETUP
    // or in the previous loop ... regardless, this turns RED off
    // analogWrite(RED, 0);
    // delay(1000);
  
    for (int i = 0; i < 255; i += 1) // fades out red bring green full when i=255
    {
      redValue -= 1;
      greenValue += 1;
      // The following was reversed, counting in the wrong directions
      // analogWrite(RED, 255 - redValue);
      // analogWrite(GREEN, 255 - greenValue);
      analogWrite(RED, redValue);
      analogWrite(GREEN, greenValue);
      delay(delayTime);
    }
  
    redValue = 0;
    greenValue = 255;
    blueValue = 0;
  
    for (int i = 0; i < 255; i += 1) // fades out green bring blue full when i=255
    {
      greenValue -= 1;
      blueValue += 1;
      // The following was reversed, counting in the wrong directions
      // analogWrite(GREEN, 255 - greenValue);
      // analogWrite(BLUE, 255 - blueValue);
      analogWrite(GREEN, greenValue);
      analogWrite(BLUE, blueValue);
      delay(delayTime);
    }
  
    redValue = 0;
    greenValue = 0;
    blueValue = 255;
  
    for (int i = 0; i < 255; i += 1) // fades out blue bring red full when i=255
    {
      // The following code has been rearranged to match the other two similar sections
      blueValue -= 1;
      redValue += 1;
      // The following was reversed, counting in the wrong directions
      // analogWrite(BLUE, 255 - blueValue);
      // analogWrite(RED, 255 - redValue);
      analogWrite(BLUE, blueValue);
      analogWrite(RED, redValue);
      delay(delayTime);
    }
}



float readVcc() 
{ 
  long result; 
  // Read 1.1V reference against AVcc 
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1); 
  delay(2); 
  // Wait for Vref to settle 
  ADCSRA |= _BV(ADSC); 
  // Convert 
  while (bit_is_set(ADCSRA,ADSC)); 
  result = ADCL; 
  result |= ADCH<<8;
  result = 1126400L / result; 
  // Back-calculate AVcc in mV 
  return result; 
}

static bool measure_environment( float *temperature, float *humidity )
{
  static unsigned long measurement_timestamp = millis( );

  /* Measure once every four seconds. */
  if( millis( ) - measurement_timestamp > 3000ul )
  {
    if( dht_sensor.measure( temperature, humidity ) == true )
    {
      measurement_timestamp = millis( );
      return( true );
    }
  }

  return( false );
}



void loop()
{
  float Ctemperature;
  float humidity;
  int num = 0;
  if( measure_environment( &Ctemperature, &humidity ) == true )
  {
    
    float Ftemperature = (Ctemperature * 9/5) + 32;
    check_temp(Ftemperature);
    Serial.println(Ftemperature);
    // Display Temperature in C
    //lcd.setCursor(0, 0);
    //lcd.print("Temp         C  ");
    // Display Temperature in F
    //lcd.print("Temp         F  ");
    lcd.setCursor(7, 0);
    // Display Temperature in C
    //lcd.print(tempC);
    // Display Temperature in F
    lcd.print(Ftemperature);
    lcd.setCursor(7, 1);
    lcd.print(humidity);
    //Serial.println(readVcc() / 1000, 3);
    //Serial.println( readVcc(), DEC );
    
  }
 
  /*  replaced
    float tempVolts = tempReading * 5.0 / 1024.0;
    float tempC = (tempVolts - 0.5) * 10.0;
    float tempF = tempC * 9.0 / 5.0 + 32.0;
  */
  
}
