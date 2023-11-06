#include <Arduino.h>

// which analog pins to connect
#define THERMISTORPIN0 A0
#define THERMISTORPIN1 A1   
#define THERMISTORPIN2 A2    
#define THERMISTORPIN3 A3    
#define THERMISTORPIN4 A4    
#define THERMISTORPIN5 A5    
#define THERMISTORPIN6 A6    
#define THERMISTORPIN7 A7    
#define THERMISTORPIN8 A8     
#define THERMISTORPIN9 A9           
// resistance at 25 degrees C
#define THERMISTORNOMINAL 10000      
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25   
// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES 10
// number of thermistors being measured 
#define NUMTHERM 10
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3950
// the value of the 'other' resistor
#define SERIESRESISTOR 10000    
// Change in temperature to print value
#define THRESHOLD 1

int samples[NUMSAMPLES][NUMTHERM];
unsigned long t;

void setup(void) {
  Serial.begin(9600);
}

void loop(void) {
  
uint8_t i;
uint8_t j;
int  average[NUMTHERM];
double  Rt;
double prevTemp[NUMTHERM] = {};
double currentTemp[NUMTHERM] = {};

  // take N samples in a row, with a slight delay
  for (i=0; i< NUMSAMPLES; i++) {
   samples[i][0] = analogRead(THERMISTORPIN0);
   samples[i][1] = analogRead(THERMISTORPIN1);
   samples[i][2] = analogRead(THERMISTORPIN2);
   samples[i][3] = analogRead(THERMISTORPIN3);
   samples[i][4] = analogRead(THERMISTORPIN4);
   samples[i][5] = analogRead(THERMISTORPIN5);
   samples[i][6] = analogRead(THERMISTORPIN6);
   samples[i][7] = analogRead(THERMISTORPIN7);
   samples[i][8] = analogRead(THERMISTORPIN8);
   samples[i][9] = analogRead(THERMISTORPIN9);
   
   delay(10);
  }
  
  t = millis();
  // average all the samples out per thermistor
  for (i=0;i < NUMTHERM; i++){
    average[i] = 0; 
     for(j = 0;j < NUMSAMPLES; j++){
            average[i] += samples[j][i];
     }
     average[i] /= NUMSAMPLES;
    
/*    Serial.print("Average analog reading for thermistor ");
    Serial.print(i+1);
    Serial.print(" ");
    Serial.println(average[i]); */
    
  
    Rt = (5./1023)*average[i];
    Rt = 1000*((10*Rt)/(5-Rt));
 
 /* Serial.print("Thermistor resistance "); 
  Serial.print(i+1);
  Serial.print(" ");
  Serial.println(Rt); */
 
    currentTemp[i] = Rt / THERMISTORNOMINAL;     // (R/Ro)
    currentTemp[i] = log(currentTemp[i]);                  // ln(R/Ro)
    currentTemp[i] /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
    currentTemp[i] += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
    currentTemp[i] = 1.0 /currentTemp[i];                 // Invert
    currentTemp[i] -= 273.15;                         // convert absolute temp to C
    
    // Check to see if previous value changed by at least the amount defined in threshold to print it
    if(abs(currentTemp[i] - prevTemp[i]) >= THRESHOLD){
      Serial.print(i+1);
      Serial.print(" , ");
      Serial.print(currentTemp[i]);
      Serial.print(" , ");
      Serial.println(t / 1000);
    }
    prevTemp[i] = currentTemp[i];

 }
  
  delay(1000);

}


