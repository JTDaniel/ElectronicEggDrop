#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>
#include <SD.h>

// Used for software SPI
#define LIS3DH_CLK 2
#define LIS3DH_MISO 3
#define LIS3DH_MOSI 4
// Used for hardware & software SPI
#define LIS3DH_CS 5
#define CLICKTHRESHHOLD 5

const int CrackLEDR = 6;
const int CrackLEDG = 7;
const int DataIndicatorPin = 8; //Blue of RGB LED
float ResultantAcceleration = 0;
float NewResultantAcceleration = 0;
const int DropThreshold = 4;
const int CrackThreshold = 8;

File myFile;

// software SPI
Adafruit_LIS3DH lis = Adafruit_LIS3DH(LIS3DH_CS, LIS3DH_MOSI, LIS3DH_MISO, LIS3DH_CLK);
// hardware SPI
//Adafruit_LIS3DH lis = Adafruit_LIS3DH(LIS3DH_CS);
// I2C
//Adafruit_LIS3DH lis = Adafruit_LIS3DH();


void WriteToCard();


void setup(void) {

  //-------------------------------------------------
  //Setting up Indicator lights
  pinMode(CrackLEDR, OUTPUT);
  pinMode(CrackLEDG, OUTPUT);
  pinMode(DataIndicatorPin, OUTPUT);

  digitalWrite(CrackLEDR, LOW);
  digitalWrite(CrackLEDG, HIGH);
  digitalWrite(DataIndicatorPin, LOW);
  //-------------------------------------------------

  Serial.begin(9600);
  while (!Serial) delay(10);     // will pause Zero, Leonardo, etc until serial console opens

  if (! lis.begin(0x18)) {   // change this to 0x19 for alternative i2c address
    Serial.println("Couldnt start");
    while (1) yield();
  }


  //-----------------------------------------------------------------------------------
  //Detect SD Card
  Serial.print("Initializing SD card...");
  //SD CD on pin 10
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
  //-----------------------------------------------------------------------------------


  //Detect Accelerometer
  Serial.println("LIS3DH found!");

  // lis.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G!

  Serial.print("Range = "); Serial.print(2 << lis.getRange());
  Serial.println("G");

  // lis.setDataRate(LIS3DH_DATARATE_50_HZ);
  Serial.print("Data rate set to: ");
  switch (lis.getDataRate()) {
    case LIS3DH_DATARATE_1_HZ: Serial.println("1 Hz"); break;
    case LIS3DH_DATARATE_10_HZ: Serial.println("10 Hz"); break;
    case LIS3DH_DATARATE_25_HZ: Serial.println("25 Hz"); break;
    case LIS3DH_DATARATE_50_HZ: Serial.println("50 Hz"); break;
    case LIS3DH_DATARATE_100_HZ: Serial.println("100 Hz"); break;
    case LIS3DH_DATARATE_200_HZ: Serial.println("200 Hz"); break;
    case LIS3DH_DATARATE_400_HZ: Serial.println("400 Hz"); break;

    case LIS3DH_DATARATE_POWERDOWN: Serial.println("Powered Down"); break;
    case LIS3DH_DATARATE_LOWPOWER_5KHZ: Serial.println("5 Khz Low Power"); break;
    case LIS3DH_DATARATE_LOWPOWER_1K6HZ: Serial.println("16 Khz Low Power"); break;
  }


   lis.setClick(2, CLICKTHRESHHOLD);

}




  void loop() {

    while (true) {
      uint8_t click = lis.getClick();
      if (click == 0) return;
      if (! (click & 0x30)) return;
      Serial.print("Click detected (0x"); Serial.print(click, HEX); Serial.print("): ");
      if (click & 0x10){   
            
      myFile = SD.open("Data.txt", FILE_WRITE);
       digitalWrite(DataIndicatorPin, HIGH);
       
        break;
      }
      myFile.println("New Data Set");
      myFile.println("-------------------------------------");
    }
    while (true) {
      sensors_event_t event;
      lis.getEvent(&event);
      ResultantAcceleration = sqrt(sq(abs(event.acceleration.x)) + sq(abs(event.acceleration.y)) + sq(abs(event.acceleration.z)));
    WriteToCard();
    NewResultantAcceleration = sqrt(sq(abs(event.acceleration.x)) + sq(abs(event.acceleration.y)) + sq(abs(event.acceleration.z)));
    if (abs(ResultantAcceleration - NewResultantAcceleration) > DropThreshold) break;
    if (abs(ResultantAcceleration - NewResultantAcceleration) > CrackThreshold){
      digitalWrite(CrackLEDG, LOW);
      digitalWrite(CrackLEDR, HIGH);
      break;
    }
    }
    for (int i=0;i<25;i++){
        WriteToCard();
      }
    myFile.println("-------------------------------------");
    myFile.close();
    digitalWrite(DataIndicatorPin, LOW);
delay (5000);
  

}

void WriteToCard(){
        sensors_event_t event;
      lis.getEvent(&event);

      ResultantAcceleration = sqrt(sq(abs(event.acceleration.x)) + sq(abs(event.acceleration.y)) + sq(abs(event.acceleration.z)));
      /* Display the results (acceleration is measured in m/s^2) */
      Serial.print("\t\tX: "); Serial.print(event.acceleration.x);
      Serial.print(" \tY: "); Serial.print(event.acceleration.y);
      Serial.print(" \tZ: "); Serial.print(event.acceleration.z);
      Serial.println(" m/s^2 ");
      Serial.println("Total Acceleration"); Serial.print(ResultantAcceleration);
      Serial.println();

      myFile.print(event.acceleration.x); myFile.print(" ");
      myFile.print(event.acceleration.y); myFile.print(" ");
      myFile.print(event.acceleration.z); myFile.print(" ");
      myFile.print(ResultantAcceleration); myFile.print(" ");
      myFile.println(" ");

      delay(200);

      
}
