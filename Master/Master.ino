#include <Wire.h>
#include "CONSTANTS.h"

void setup()
{
  Serial.begin(9600); // Open serial port and set data rate to 9600 bps
  Wire.begin(); // Join I2C bus as master
}

void compare() // Print the compared values by slave
{
  Serial.print(F("Enter slave address: "));
  byte addr = serial_input();
  Serial.print(F("Enter memory register variable A: "));
  byte val_a = serial_input();
  Serial.print(F("Enter memory register variable B: "));
  byte val_b = serial_input();

  Wire.beginTransmission(addr); // Transmit to device address
  Wire.write(COMPARE); // Write current mode
  Wire.write(val_a); // Variable A
  Wire.write(val_b); // Variable B
  RESULTS result = Wire.endTransmission(); // Slave response
  switch (result) // Compare response
  {
    case ACK:
      Serial.println("Data arrived succesfully!");
      break;
    case OVERFLOW_NACK:
      Serial.println("Too much data to transmit. Try again!");
      break;
    case ADDR_NACK:
      Serial.println("Address not acknowledged. Try again!");
      break;
    case DATA_NACK:
      Serial.println("Data not acknowledged. Try again!");
      break;

    case UNKNOWN_NACK:
      Serial.println("Something went wrong. Try again!");
      break;

    default:
      Serial.println("Unknown error. Try again!");
      break;
  }
  Serial.println();

  Wire.requestFrom(addr, 2, false); // Request two bytes from set address

  Serial.print("Max: ");
  Serial.println(Wire.read());
  Serial.print("Min: ");
  Serial.println(Wire.read());
  Serial.println();
}

void store() // Transmit values according to chosen mode
{
  Serial.print(F("Enter slave address: "));
  byte addr = serial_input();
  Serial.print(F("Enter memory register: "));
  byte reg = serial_input();
  Serial.print(F("Enter value to be store: "));
  byte val = serial_input();

  Wire.beginTransmission(addr); // Transmit to device address
  Wire.write(STORE); // Write character to declare mode
  Wire.write(reg); // Memory register address
  Wire.write(val); // Value to store
  RESULTS result = Wire.endTransmission(); // Slave response
  switch (result) // Compare response
  {
    case ACK:
      Serial.println("Value stored!");
      break;
    case OVERFLOW_NACK:
      Serial.println("Too much data to transmit!");
      break;
    case ADDR_NACK:
      Serial.println("Address not acknowledged");
      break;
    case DATA_NACK:
      Serial.println("Data not acknowledged");
      break;

    case UNKNOWN_NACK:
      Serial.println("Something went wrong. Try again!");
      break;

    default:
      Serial.println("Unknown error. Try again!");
      break;
  }
  Serial.println();
}

byte serial_input()
{
  int counter = 0;
  uint32_t number = 0;
  while (true)
  {
    byte received = Serial.read();
    if (received == -1) continue;
    if (received == '\n') break;
    if (received < '0' || received > '9' || counter >= 4) continue;
    number = number * 10 + received - '0';
    if (number > 255) number = 255;
    counter++;
  }
  Serial.println(number);
  return number;
}

void loop()
{
  Serial.print(F("Choose 0 to store, 1 to compare: "));
  MODES mode = serial_input();
  switch (mode)
  {
    case 0:
      store();
      break;

    case 1:
      compare();
      break;

    default:
      Serial.println(F("Wrong mode chosen. Try again!"));
      Serial.println();
      break;
  }
}
