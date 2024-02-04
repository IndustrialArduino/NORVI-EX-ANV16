/*Example code for SC-EX-ADV-4I /NORVI-EX-ANV16
 * ADC LTC2497 True Differential ADC
 * Reading 8 x Differential Channels / Max voltage 2.5V DC
 * 
 * 
 */
// LTC2497 I2C Address                 //  AD2       AD1       AD0
// #define LTC2497_I2C_ADDRESS 0x14    //  Low       Low       Low
// #define LTC2497_I2C_ADDRESS 0x15    //  Low       Low       Float
// #define LTC2497_I2C_ADDRESS 0x17    //  Low       Float     Low
// #define LTC2497_I2C_ADDRESS 0x24    //  Low       Float     Float
// #define LTC2497_I2C_ADDRESS 0x35    //  Float     Low       Low
// #define LTC2497_I2C_ADDRESS 0x36    //  Float     Low       Float
// #define LTC2497_I2C_ADDRESS 0x44    //  Float     Float     Low
// #define LTC2497_I2C_ADDRESS 0x45    //  Float     Float     Float


#include <Wire.h>

// LTC2497 I2C Address
const int LTC2497_ADDRESS = 0x45; // or the address that your device uses
uint32_t adc_value = 0;

// Function to read data from LTC2497
long readLTC2497(byte channel) {
  uint32_t data = 0;
  Wire.beginTransmission(LTC2497_ADDRESS);
  
  // Send configuration byte to select channel
  Wire.write(0xA0 | channel); // Example configuration for single-ended
  Wire.endTransmission();
  // Wait for conversion to complete
  delay(200);

  // Request 4 bytes of data
  Wire.requestFrom(LTC2497_ADDRESS, 4);

  if (Wire.available() == 4) {
    data = Wire.read();
    data = (data << 8) | Wire.read();
    data = (data << 8) | Wire.read();
    
  }
  
  return data;
}


void setup() {
  Wire.begin(16,17); // Join I2C bus
  Serial.begin(115200); // Start serial communication at 9600 baud
  uint32_t data = readLTC2497(0); // Read data from channel 0
  
}

void loop() {
  float voltage;
  unsigned int reading_channel=0;
  
  for (int i = 0; i <= 7; i++) {
      uint32_t data = readLTC2497(i); // Read data from channel 0
      delay(100);
      data = readLTC2497(i); // Read data from channel 0
      
      //Serial.println(data,HEX);
      adc_value =  data<<8;
      adc_value = adc_value>>2;
      adc_value = adc_value & 0x3FFFFFFF;
     // Serial.println(adc_value,HEX);
    
      adc_value -= 0x20000000;             //! 1) Converts offset binary to binary
      voltage=(float) adc_value;
      voltage = voltage / 536870912.0;    //! 2) This calculates the input as a fraction of the reference voltage (dimensionless)
      voltage = voltage * 5.0;           //! 3) Multiply fraction by Vref to get the actual voltage at the input (in volts)
      Serial.print(" A");  Serial.print(i); Serial.print(" : ");
      Serial.print(voltage); // Print the data
      delay(300);
  }
Serial.println("");

  
  delay(1000); // Delay for a second
}
