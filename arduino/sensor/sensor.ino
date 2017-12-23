/*
  This is a library written for the AS726X Spectral Sensor (Visible or IR) with I2C firmware
  specially loaded. SparkFun sells these at its website: www.sparkfun.com

  Written by Nathan Seidle & Andrew England @ SparkFun Electronics, July 12th, 2017

  https://github.com/sparkfun/Qwiic_Spectral_Sensor_AS726X

  Do you like this library? Help support SparkFun. Buy a board!

  Development environment specifics:
  Arduino IDE 1.8.1

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.


  ---AVAILABLE FUNCTIONS---
  AS726X(TwoWire &wirePort = Wire, byte gain = 3, byte measurementMode = 3);
  void takeMeasurements();
  void takeMeasurementsWithBulb();
  void printMeasurements();
  byte getTemperature();
  float getTemperatureF();
  void setMeasurementMode(byte mode);
  boolean dataAvailable();
  void enableIndicator();
  void disableIndicator();
  void setIndicatorCurrent(byte current);
  void enableBulb();
  void disableBulb();
  void setBulbCurrent(byte current);
  void softReset();
  void setGain(byte gain);
  void setIntegrationTime(byte integrationValue);
  void enableInterrupt();
  void disableInterrupt();

  //Get the various color readings
  int getViolet();
  int getBlue();
  int getGreen();
  int getYellow();
  int getOrange();
  int getRed();

  //Get the various NIR readings
  int getR();
  int getS();
  int getT();
  int getU();
  int getV();
  int getW();

  //Returns the various calibration data
  float getCalibratedViolet();
  float getCalibratedBlue();
  float getCalibratedGreen();
  float getCalibratedYellow();
  float getCalibratedOrange();
  float getCalibratedRed();

  float getCalibratedR();
  float getCalibratedS();
  float getCalibratedT();
  float getCalibratedU();
  float getCalibratedV();
  float getCalibratedW();
*/

#include "AS726X.h"

AS726X sensor;

const int ledPin = 13;
String incoming = "";
String key = "";
String value = "";
String string = "";
boolean measureBulb = false;

unsigned long serialdata;
int inbyte;


void setup() {
  sensor.begin();
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  sensor.setIntegrationTime(255);
  sensor.setGain(0);
}

void loop() {
  readSerial();
  
  if (measureBulb == false){
   sensor.takeMeasurements();
  }
  if (measureBulb == true){
    sensor.takeMeasurementsWithBulb();
  }
  
   sensor.printMeasurements();
}


void readSerial(){

   while (Serial.available()) {          
      digitalWrite(ledPin, LOW); //LED on while receiving
      incoming = Serial.readStringUntil('\n');
      while (incoming.indexOf(":") >0){
          key = incoming.substring(0, incoming.indexOf(":"));
          value = incoming.substring(incoming.indexOf(":")+1, incoming.indexOf(";"));
          if (incoming.indexOf(";") >0) {
            incoming = incoming.substring(incoming.indexOf(";")+1, incoming.length());
          }
          else {
            incoming = "";
          }

      if (key=="bulb"){
        if (value=="on"){
           measureBulb = true;
        }
        if (value=="off"){
          measureBulb = false;
        }
      }

      if (key=="gain"){
         sensor.setGain(value.toInt());
      }
      
      if (key=="blueLED"){
        if (value=="on"){
            sensor.enableIndicator();
        }
        if (value=="off"){
            sensor.disableIndicator();
        }
      }
      if (key=="current"){
         sensor.setBulbCurrent(value.toInt());
      }
      
      if (key=="tc"){
         sensor.setIntegrationTime(value.toInt());
      }
      
      }
      
     }
  
  
      
  if (incoming == "withBulb"){
    measureBulb = true;
  }
  if (incoming == "noBulb"){
    measureBulb = false;
  }
  digitalWrite(ledPin, LOW); //LED off while not receiving
  }



