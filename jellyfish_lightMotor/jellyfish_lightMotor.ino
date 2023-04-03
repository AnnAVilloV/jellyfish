#include <FastLED.h>
#define NUM_LEDS 16 
#define DATA_PIN 5 //light

int controlPin1 = 2;
int controlPin2 = 3;

int enablePin = 9;
int potPin = A0;

int motorSpeed = 0;

int lightStep = 0;
int paletteFast = 0;
int paletteSlow = 0;

CRGB leds[NUM_LEDS]; // 定义CRGB的led等数组

String serialRead;
String start;
String num;
String value;

boolean d2 = false;
boolean d3 = false;
boolean d4 = false;
int clickD4 = 0;

int hue = 130;
int bright = 255;

CRGBPalette16 tempPalette;
int tempi = 0;
CRGBPalette16 purpleBlue = CRGBPalette16(
CRGB::DarkBlue,CRGB::DarkBlue, CRGB::DarkViolet, CRGB::DarkViolet,
CRGB::Purple, CRGB::Purple, CRGB::Navy, CRGB::Navy,
CRGB::DarkBlue, CRGB::DarkBlue, CRGB::BlueViolet, CRGB::BlueViolet,
CRGB::Purple, CRGB::Purple, CRGB::MidnightBlue, CRGB::MidnightBlue );   

CRGBPalette16 paList[] = {
  OceanColors_p,
  LavaColors_p,
  ForestColors_p,
  purpleBlue
  };

void setup() {
  Serial.begin(9600);
   FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); 
   FastLED.setBrightness(bright);


  pinMode(controlPin1,OUTPUT);
  pinMode(controlPin2,OUTPUT);
  pinMode(enablePin, OUTPUT);
  
  digitalWrite(enablePin,LOW);
  
  digitalWrite(controlPin2,HIGH);
  digitalWrite(controlPin1,LOW);

  Serial.begin(9600);


  pinMode(7,OUTPUT);
}

void loop() {
digitalWrite(7,LOW);

  if(Serial.available()>0){
//     serialRead = Serial.readString();
      char nextChar;
      byte readByte;
     do {
      readByte = Serial.read();
//      Serial.println(readByte);
      nextChar = readByte;
      if (readByte != 255) serialRead += nextChar;

      if (nextChar == 'd') serialRead = "d";
      if (nextChar == 'a') serialRead = "a";
     } while (nextChar != '\n' && nextChar != '\0' && readByte != 255);
//    Serial.println(serialRead);
    

    if(serialRead.length() >=5 ){
      if (nextChar == '\n') {
      Serial.println("ardu cmd: "+serialRead);
       split(serialRead);
      if(start.equals(String('a'))){
        if(num.equals(String('1'))){
          Serial.println("a1");
        }
        else if(num.equals(String('2'))){
            bright = (value.toInt())/50;
            FastLED.setBrightness(bright); 
            
        }else if(num.equals(String('3'))){
            motorSpeed = (value.toInt())/100;
            analogWrite(enablePin,motorSpeed);

        }
        serialRead = "";
      }else if(start.equals(String('d'))){
        switch (num.charAt(0)) {
          case '1':
            //  code
            //random hue
              hue = random(0,256);
              serialRead = "";
             break;
          case '2':
              d4 = false;
              d3 = false;
              d2 = !d2; 
              serialRead = "";
             break;
          case '3':
            //  code
              d4 = false;
              d2 = false;
              d3 = !d3;
              serialRead = "";
             break;
          case '4':
              d4 = true;
              d2 = false;
              d3 = false;
              tempPalette = paList[tempi];
              tempi = (tempi+1)%4;
              
              serialRead = "";
        }
      }
    }

    }
    

  }
  







  if(d3){
    //rainble mode
    rainbow();  
  }else if(d4){

        fill_palette(leds,NUM_LEDS,paletteSlow,8,tempPalette,255,LINEARBLEND);
        FastLED.show();
        paletteFast = (paletteFast + 2) % 256;
        if(paletteFast % 24 == 0){
          paletteSlow++;
        }

  }
  else{
    if(d2)    
     oneLightCircle();
    else if(!d3 && d2){
     oneLightCircle();
    }
  }

}


void split(String str){
  int index = str.indexOf(':');
  start = str.substring(0,index);
  String temp = str.substring(index+1,str.length());
  index = temp.indexOf(':');
  num = temp.substring(0,index);
  value = temp.substring(index+1,temp.length());
}

void oneLightCircle(){
  for(int i = 0; i < NUM_LEDS; i++ ) {
    leds[i] = CHSV( hue , 255, bright); 
    FastLED.show();
    delay(100);
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(10);
  }
}

void rainbow(){

    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV( (lightStep + (255 / NUM_LEDS) * i), 255, bright); //用HSV色彩空间，不断改变H即可
      FastLED.show();
    }
    lightStep = (lightStep + 2) % 256;
//    delay(1);
//    h = (h + 2) % 255;
  
}
