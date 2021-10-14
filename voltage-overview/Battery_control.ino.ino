#include <Adafruit_SSD1306.h>
#include <splash.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>

//Project adjusted to Arduino Nano.
//Using an OLED I2C to show the results
//Pin-number avaliable at http://www.circuitstoday.com/arduino-nano-tutorial-pinout-schematics
#define SDA A4 //A4 
#define SCL A5 //A5
#define greenLight 7 //D7
#define yellowLight 8 //D8
#define redLight 9 //D9
#define battery A0 //A0
#define DISPLAY_ADDRESS 0x3C //

//OLED defines
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 32 
#define OLED_RESET -1
#define TEXT_SIZE 3

//resistor values defines
#define R_1 4800.0
#define R_2 1400.0

//scale for testing with different voltage-levels during testing
#define scale 1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  pinMode(SDA, OUTPUT);
  pinMode(SCL, OUTPUT);
  pinMode(greenLight, OUTPUT);
  pinMode(yellowLight, OUTPUT);
  pinMode(redLight,OUTPUT);
  pinMode(battery, INPUT);

  Serial.begin (115200);
  if(!display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDRESS)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("Hello, Vortex!");
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
}

//change the LED's lights
void changeLights(const float& voltage){
  digitalWrite(greenLight, LOW);
  digitalWrite(yellowLight, LOW);
  digitalWrite(redLight, LOW);
  if(voltage > 14.5 && voltage < 17){
    digitalWrite(greenLight, HIGH);
    delay(1000);
    digitalWrite(greenLight, LOW);
  }
  else if(voltage > 14 && voltage <= 14.5){
    for(int i = 0; i < 2; i++){
        digitalWrite(yellowLight, HIGH);
        delay(400);
        digitalWrite(yellowLight, LOW);
        delay(100);
    }
  }
  else if(voltage <= 14){
    for(int i = 0; i < 4; i++){
        digitalWrite(redLight, HIGH);
        delay(150);
        digitalWrite(redLight, LOW);
        delay(100);
    }
  }
  else{
    digitalWrite(greenLight, HIGH);
    digitalWrite(yellowLight, HIGH);
    digitalWrite(redLight, HIGH);
    delay(1000);
    digitalWrite(greenLight, LOW);
    digitalWrite(yellowLight, LOW);
    digitalWrite(redLight, LOW);
  }
  return;
}

//main loop. Calculate and show current voltage, while showing voltage on LED
//Updating every second
void loop(){   
  float voltage = analogRead(battery) * (5.0 / 1023.0);
  voltage = (voltage*(R_1+R_2))/(scale*R_2);
  while(1){
    voltage = analogRead(battery) * (5.0 / 1023.0);
    voltage = (voltage*(R_1+R_2))/(scale*R_2);
    display.clearDisplay();
    display.setTextSize(TEXT_SIZE);
    display.setCursor(0,10);
    display.println(voltage);
    display.display();
    Serial.print("Voltage: ");
    Serial.println(voltage); 
    changeLights(voltage);
    display.clearDisplay();
    delay(1000);
  }
}
