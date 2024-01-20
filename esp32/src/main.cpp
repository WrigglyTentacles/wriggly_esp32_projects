#include "esp32-hal-adc.h"
#include "esp32-hal-gpio.h"
#include <Arduino.h>

using namespace std;

// Encoder Variables
#define ENCODER_CLK 25
#define ENCODER_DT 33
#define ENCODER_CW 1
#define ENCODER_CCW 0


// Joystick Variables
#define VRX_PIN 27
#define VRY_PIN 26

// Encoder Vars
int direction = ENCODER_CW;
int counter = 0;
int CLK_state;
int prev_CLK_state;

// Joystick Vars
int centerx = 4096/2;
int centery = 4096/2;
int xthreshold = 300;
int ythreshold = 300;
int maxx = 1920;
int maxy = 1080;
int inc_x = 20;
int inc_y = 20;

int valuex = centerx;
int valuey = centery;
int prevvaluex = centerx;
int prevvaluey = centery;
int truevaluex = maxx/2;
int truevaluey = maxy/2;

void checkJoystick(){
	valuex = analogRead(VRX_PIN);
	valuey = analogRead(VRY_PIN);

	if ((valuex >= centerx + xthreshold)||(valuex <= centerx-xthreshold)){
		if(valuex>=centerx+xthreshold){
			truevaluex += (inc_x);
		}
		if(valuex<=centerx-xthreshold){
			truevaluex -= (inc_x);
		}
		if(truevaluex>maxx){truevaluex=maxx;}
		if(truevaluex<0){truevaluex=0;}
		Serial.print("true x = ");
		Serial.println(truevaluex);
		//Serial.print("x = ");
		//Serial.println(valuex);
	}
	if ((valuey >= centery + ythreshold)||(valuey <= centery-ythreshold)){
		if(valuey>=centery+ythreshold){
			truevaluey -= inc_y;
		}
		if(valuey<=centery-ythreshold){
			truevaluey += inc_y;
		}
		if(truevaluey>maxy){truevaluey=maxy;}
		if(truevaluey<0){truevaluey=0;}
		Serial.print("true y = ");
		Serial.println(truevaluey);
		//Serial.print("y = ");
		//Serial.println(valuey);
	}
}

void checkEncoder(){
	 // read the current state of the rotary encoder's CLK pin
  CLK_state = digitalRead(ENCODER_CLK);
  int DT_state = digitalRead(ENCODER_DT);
  // If the state of CLK is changed, then pulse occurred
  // React to only the rising edge (from LOW to HIGH) to avoid double count
  if (CLK_state != prev_CLK_state) {
    // if the DT state is HIGH
    // the encoder is rotating in counter-clockwise direction => decrease the counter
    if (DT_state-CLK_state == 0) {
      counter--;
      direction = ENCODER_CCW;
    } else {
      // the encoder is rotating in clockwise direction => increase the counter
      counter++;
      direction = ENCODER_CW;
    }

    Serial.print(" - count: ");
    Serial.println(counter);

	prev_CLK_state = CLK_state;
  }
}

void setup(){
	Serial.begin(9600);

	// Setup JoyStick pins
	centerx = analogRead(VRX_PIN);
	centery = analogRead(VRY_PIN);

	// Setup Encoder pins
	pinMode(ENCODER_CLK, INPUT);
	pinMode(ENCODER_DT, INPUT);
	prev_CLK_state = digitalRead(ENCODER_CLK);
}

void loop(){
	checkJoystick();
	checkEncoder();
	delay(1);
}
