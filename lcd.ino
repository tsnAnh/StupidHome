u#include <Wire.h>

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

String intro = "01 Nam Ky Khoi Nghia, Ngu Hanh Son, Da Nang";

void setup() {

  lcd.init();

  lcd.backlight();

}

void loop() {

  for (int i=0; i<intro.length()-16; i++){

    for (int j=0; j<16; j++){

      lcd.setCursor(j,0);

      lcd.print(intro[i+j]);

    }
   
    

    delay(250);

  }
}
