/* 
   Arduino Nintendo Entertainment System (NES) Controller Interface
   Richard Morris 26/12/2019

  NES controller connector pinout:
   _________
  |         \
  | 1 O      \    1 - Ground (0V)
  |           |   2 - Data Clock
  | 2 O   O 5 |   3 - Data Latch
  |           |   4 - Serial Data (standard gamepad)
  | 3 O   O 6 |   5 - Data (special controller) / No Connection
  |           |   6 - Data (special controller) / No Connection
  | 4 O   O 7 |   7 - VCC (+5V)
  |___________|

*/

#include <Arduino.h>

/* NES controller connections */
#define NES_CLOCK_PIN 2   /* shift register clock (green) */
#define NES_LATCH_PIN 3   /* shift register latch (blue) */
#define NES_DATA_PIN 4    /* data (yellow) */

/* NES controller button bit-mask patterns */
#define NES_BUTTON_A      0x01
#define NES_BUTTON_B      0x02
#define NES_BUTTON_SELECT 0x04
#define NES_BUTTON_START  0x08
#define NES_BUTTON_UP     0x10
#define NES_BUTTON_DOWN   0x20
#define NES_BUTTON_LEFT   0x40
#define NES_BUTTON_RIGHT  0x80

uint8_t data2;

uint8_t read_nes_controller()
{
  uint8_t data = 0x00;
 data2 = 0x00;
  /* latch shift register data */
  digitalWrite(NES_LATCH_PIN, HIGH);
  delayMicroseconds(12);
  digitalWrite(NES_LATCH_PIN, LOW);
  for (int x = 0; x < 8; x++)
  {
    /* read data from shift register */
    data |= (digitalRead(NES_DATA_PIN) << x);
    /* clock shift register to read next bit */
    digitalWrite(NES_CLOCK_PIN, HIGH);
    delayMicroseconds(12);
    digitalWrite(NES_CLOCK_PIN, LOW);
  }

  for (int x = 0; x < 8; x++)
  {
    /* read data from shift register */
    data2 |= (digitalRead(NES_DATA_PIN) << x);
    /* clock shift register to read next bit */
    digitalWrite(NES_CLOCK_PIN, HIGH);
    delayMicroseconds(12);
    digitalWrite(NES_CLOCK_PIN, LOW);
  }

  
  return data;
}

void setup() {
  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  /* set appropriate NES controller pins to inputs/outputs */
  pinMode(NES_CLOCK_PIN, OUTPUT);
  pinMode(NES_LATCH_PIN, OUTPUT);
  
  pinMode(NES_DATA_PIN, INPUT_PULLUP);

  /* set intial states */
  digitalWrite(NES_CLOCK_PIN, LOW);
  digitalWrite(NES_LATCH_PIN, LOW);
}

void loop() {

  uint8_t data_register = read_nes_controller();
  // Serial.println( data_register, HEX );

#if 0
  if ((data_register & NES_BUTTON_A) == 0)
    Serial.println("A");
  if ((data_register & NES_BUTTON_B) == 0)
    Serial.println("B");
  if ((data_register & NES_BUTTON_SELECT) == 0)
    Serial.println("SELECT");
  if ((data_register & NES_BUTTON_START) == 0)
    Serial.println("START");
  if ((data_register & NES_BUTTON_UP) == 0)
    Serial.println("UP");
  if ((data_register & NES_BUTTON_DOWN) == 0)
    Serial.println("DOWN");
  if ((data_register & NES_BUTTON_LEFT) == 0)
    Serial.println("LEFT");
  if ((data_register & NES_BUTTON_RIGHT) == 0)
    Serial.println("RIGHT");
#endif

    Serial.println(data2, HEX);

  delay(100);
}
