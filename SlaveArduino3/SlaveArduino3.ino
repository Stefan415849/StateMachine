#include <Wire.h>
#include <EEPROM.h>

void setup()
{
  Serial.begin(9600); // Open serial port and set data rate to 9600 bps
  Wire.begin(42); // Join I2C bus with slave address 42
  Wire.onReceive(receiveEvent); // Perform function when bytes received
  Wire.onRequest(requestEvent); // Perform function when bytes requested
}

void loop()
{
}

byte var_a; // Variable A
byte var_b; // Variable B

void receiveEvent()
{
  byte counter = 0; // Count received bytes
  byte address; // Memory address
  byte value; // Stored value
  byte mode; // Chosen mode
  while (counter < 3) // Iterate 3 times
  {
    byte received = Wire.read();
    if (counter == 0 && received == 'S') // Store mode chosen
    {
      Serial.println("Value stored! ");
      mode = 1;
    }
    else if (counter == 0 && received == 'C') // Compare mode chosen
    {
      Serial.println("Comparing values!");
      mode = 2;
    }
    else
    {
      switch (mode) // Store incoming byte to address or set variable A and B to incoming bytes
      {
        case 1:
          if (counter == 1) address = received;
          else
          {
            value = received;
            EEPROM.write(address, value);
            Serial.print("Memory address: ");
            Serial.println(address);
            Serial.print("Value: ");
            Serial.println(value);
            Serial.println();
          }
          break;

        case 2:
          if (counter == 1) var_a = EEPROM.read(received);
          else
          {
            var_b = EEPROM.read(received);
            Serial.print("Value of variable A: ");
            Serial.println(var_a);
            Serial.print("Value of variable B: ");
            Serial.println(var_b);
          }
          break;
      }
    }
    counter++;
  }
}

void requestEvent()
{
  byte maximum = (var_a > var_b) ? var_a : var_b; // Calculate maximum value
  byte minimum = (var_a > var_b) ? var_b : var_a; // Calculate minimum value
  Serial.print("Maximum value: ");
  Serial.println(maximum);
  Serial.print("Minimum value: ");
  Serial.println(minimum);
  Serial.println();
  Wire.write(maximum); // Write maximum value to master
  Wire.write(minimum); // Write minimum value to master
}
