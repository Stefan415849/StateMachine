#include <Wire.h>

void setup()
{
  Serial.begin(9600); // Open serial port and set data rate to 9600 bps
  Wire.begin(); // Join I2C bus as master
}

void compare() // Print the compared values by slave
{
  Serial.print(F("Enter slave address: "));
  byte addr = Serial.readStringUntil('\n').toInt();
  Serial.print(F("Enter memory register variable A: "));
  byte val_a = Serial.readStringUntil('\n').toInt();
  Serial.print(F("Enter value to be store: "));
  byte val_b = Serial.readStringUntil('\n').toInt();

  Wire.beginTransmission(addr); // Transmit to device address
  Wire.write('C'); // Write current mode
  Wire.write(val_a); // Variable A
  Wire.write(val_b); // Variable B
  switch (Wire.endTransmission()) // Slave response
  {
    case 0:
      Serial.println("Data arrived succesfully!");
      break;
    case 1:
      Serial.println("Too much data to transmit!");
      break;
    case 2:
      Serial.println("Address not acknowledged");
      break;
    case 3:
      Serial.println("Data not acknowledged");
      break;

    case 4:
      Serial.println("Something went wrong. Try again!");
      break;
  }
  Serial.println();

  Wire.requestFrom(addr, 2, false); // Request two bytes from set address

  Serial.print("Max: ");
  Serial.println(Wire.read());
  Serial.print("Min: ");
  Serial.println(Wire.read());
}

void store() // Transmit values according to chosen mode
{
  Serial.print(F("Enter slave address: "));
  byte addr = Serial.readStringUntil('\n').toInt();
  Serial.print(F("Enter memory register: "));
  byte reg = Serial.readStringUntil('\n').toInt();
  Serial.print(F("Enter value to be store: "));
  byte val = Serial.readStringUntil('\n').toInt();

  Wire.beginTransmission(addr); // Transmit to device address
  Wire.write('S'); // Write character to declare mode
  Wire.write(reg); // Memory register for 'store' or variable A for 'compare'
  Wire.write(val); // Value to store for 'store' or variabe B for 'compare'
  switch (Wire.endTransmission()) // Slave response
  {
    case 0:
      Serial.println("Value stored!");
      break;
    case 1:
      Serial.println("Too much data to transmit!");
      break;
    case 2:
      Serial.println("Address not acknowledged");
      break;
    case 3:
      Serial.println("Data not acknowledged");
      break;

    case 4:
      Serial.println("Something went wrong. Try again!");
      break;
  }
  Serial.println();
}

void loop()
{
  Serial.print(F("Choose 1 to store, 2 to compare: "));
  while (Serial.available() == 0) {}
  int mode = Serial.readStringUntil('\n').toInt();
  Serial.println(mode);
  switch (mode)
  {
    case 1:
      store();
      break;

    case 2:
      compare();
      break;

    default:
    Serial.println("Wrong mode chosen. Try again!");
      break;
  }
}
