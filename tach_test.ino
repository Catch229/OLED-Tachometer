//DIY ATMEL 32u4 based vehicle tach sensor.
//For old cars with no tach!

#include <U8g2lib.h>
#include <U8x8lib.h>

U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);


const int NUMBER_OF_CYLINDERS = 4;
const int FIRES_PER_REV = (360 / (720 / NUMBER_OF_CYLINDERS));

const double MAX_RPM = 6000;
const double REDLINE_RPM = 5000;
const double START_RPM = 1000;

double current_rpm = 1531;
volatile int spark_fire_count = 0;

const int BOX_WIDTH = 128;
const int BOX_HEIGHT = 22;
const int BOX_POSX = 0;
const int BOX_POSY = 39;

const int UPDATE_INTERVAL = 100;
int last_update_time = 0;

int redline_posx = 128 - ((REDLINE_RPM - START_RPM) / MAX_RPM) * BOX_WIDTH;

void setup() {
  // put your setup code here, to run once:
  u8g2.begin();
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(7), incFireCount, FALLING);
}

void loop() {
  if ((millis() - last_update_time) > UPDATE_INTERVAL) {
    current_rpm = (spark_fire_count * (1000 / UPDATE_INTERVAL) * 60) / FIRES_PER_REV;
    Serial.println(spark_fire_count);
  
    u8g2.firstPage();
    do {              //Now with data draw to the screen
      u8g2.setDisplayRotation(U8G2_R0);
      u8g2.drawFrame(BOX_POSX,BOX_POSY,BOX_WIDTH,BOX_HEIGHT);
      drawTachBar();
      drawRedline();
      u8g2.setFont(u8g2_font_cupcakemetoyourleader_tr); //Set font for display
      u8g2.setCursor(50,23);
      u8g2.print(current_rpm, 0);
      u8g2.setDisplayRotation(U8G2_R3);
      u8g2.drawStr(28, 13, "RPM");
    } while(u8g2.nextPage());
    spark_fire_count = 0;
    last_update_time = millis();
  }

}

void drawTachBar(){
  int bar_width = 128 - ((current_rpm - START_RPM) / (MAX_RPM)) * BOX_WIDTH;
  u8g2.drawBox(bar_width,BOX_POSY,128,BOX_HEIGHT);
}

void drawRedline(){
  u8g2.drawVLine(redline_posx,BOX_POSY,BOX_HEIGHT);
}

void incFireCount(){
  spark_fire_count++;
}

