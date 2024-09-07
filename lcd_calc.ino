#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int x_pot_pin = A0;
const int d_pot_pin = A1;


LiquidCrystal_I2C lcd(0x27, 16, 2);

char add_1[8] = "0000000";
char add_2[8] = "0000000";
int state = 0; // 0: ADD, 1: SUB
int previousValue = 0;
int previousPosition = 0;
int res = 0;
int length = 0;

int calculate_result() {
  int add_1_int = atoi(add_1);
  int add_2_int = atoi(add_2);
  int result = (state == 0) ? (add_1_int + add_2_int) : (add_1_int - add_2_int);
  return result;
}

int res_length(int number) {
  char buffer[10];  
  itoa(number, buffer, 10);  
  return strlen(buffer); 
}

void display(int x, int res, int length){
  lcd.clear();
  //OPERATION sign
  lcd.setCursor(7,0);
  lcd.print((state == 0) ? "+" : "-");
    //first summand
  lcd.setCursor(0,0);
  lcd.print(add_1);

  //second summand
  lcd.setCursor(8,0);
  lcd.print(add_2);

  //result
  lcd.setCursor(16 - length,1);
  lcd.print(res);

  //equal sign
  lcd.setCursor(15,0);
  lcd.print("=");
  //cursor
  lcd.setCursor(x,0);
  lcd.cursor();  
}

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  lcd.init();
  lcd.backlight();
}

void loop() {
  // put your main code here, to run repeatedly:
  int potVal1 = analogRead(x_pot_pin);
  int x = map(potVal1, 0, 675, 0, 14);

  int potVal2 = analogRead(d_pot_pin);
  int d = map(potVal2, 0, 675, 0, 9);
  char val = '0' + d;

  if (x != previousPosition){
    display(x, res, length);
    previousPosition = x;
  }
  if (d != previousValue) {
    if (x < 7) {
      add_1[x] = val;
    }
    else if (x > 7) {
      add_2[x - 8] = val;
    }
    else if (x == 7) {
      if (d < 5) {
        state = 1;
      } else {
        state = 0;
      }
    }
    res = calculate_result();
    length = res_length(res);
    delay(10);
    display(x, res, length);
    previousValue = d;
  }
  delay(300);
}
