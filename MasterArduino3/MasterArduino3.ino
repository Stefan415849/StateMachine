#include <Wire.h>

void setup()
{
  Serial.begin(9600); // Open serial port and set data rate to 9600 bps
  Wire.begin(); // Join I2C bus as master
}

void compare(int address) // Print the compared values by slave
{
  byte count = 0;
  Wire.requestFrom(address, 2, false); // Request two bytes from set address
  while (count < 2) // Iterate twice
  {
    if (count == 0) Serial.print("Max: ");
    else Serial.print("Min: ");
    byte received = Wire.read(); // Read incoming byte
    Serial.println(received);
    count++;
  }
}

void transmit(int mode) // Transmit values according to chosen mode
{
  int data [3];
  byte index = 0;
  while (index < 3) // Iterate three times
  {
    if (mode == 1) // Print lines according to mode
      switch (index)
      {
        case 0:
          Serial.print(F("Enter slave address: "));
          break;
        case 1:
          Serial.print(F("Enter memory register: "));
          break;
        case 2:
          Serial.print(F("Enter value to be store: "));
          break;
      }
    else
      switch (index)
      {
        case 0:
          Serial.print(F("Enter slave address: "));
          break;
        case 1:
          Serial.print(F("Enter memory register variable A: "));
          break;
        case 2:
          Serial.print(F("Enter memory register variable B: "));
          break;
      }
    int input = serial_input(); // Set integer to serial input
    data[index] = input; // Store input for later usage
    index++;
  }

  Wire.beginTransmission(data[0]); // Transmit to device address
  if (mode == 1) Wire.write('S'); // Write character to declare mode
  else Wire.write('C');
  Wire.write(data[1]); // Memory register for 'store' or variable A for 'compare'
  Wire.write(data[2]); // Value to store for 'store' or variabe B for 'compare'
  switch (Wire.endTransmission()) // Slave response
  {
    case 0:
      if (mode == 1) Serial.println("Value stored!");
      else compare(data[0]);
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

int serial_input() // Gets integer and sets it to serial input
{
  int number = 0;
  while (true)
  {
    int received = Serial.read();
    if (received == -1) continue;
    if (received == '\n') break;
    number = number * 10 + (received - '0');
  }
  Serial.println(number);
  return number;
}

void select_mode()
{
  Serial.print(F("Choose 1 to store, 2 to compare: "));
  int mode = serial_input(); // Set mode to serial input
  if (mode == 1) transmit(1); // Perform transmit function with chosen mode
  else if (mode == 2) transmit(2); 
}

void loop()
{
  select_mode(); // Select mode
}
