#include <SoftwareSerial.h>

//#include <TinyGPS.h>

/* This sample code demonstrates the normal use of a TinyGPS object.
   It requires the use of SoftwareSerial, and aSerialumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD addreSerial to 0x27 for a 20 chars and 4 line display

//TinyGPS gps;
//SoftwareSerial Serial(4, 3);
//float lattitude,longitude;
//    float flat, flon;
//    unsigned long age;
    
SoftwareSerial mySerial(2, 3);
char msg;
char call;
char web[] = "www.google.com";


int gasSensor1 = A0;
int gasSensor2 = A1;
int flameSensor1 =5;
int flameSensor2 =6;
int relay1 =11;

bool gasdetected1 =false;
bool gasdetected2 =false;

bool flamedetected1 =false;
bool flamedetected2 =false;

char gas_alert[] = "Gas Detected!";
char fire_alert[] = "Fire Detected!";

bool gas_sms_sent = false;
bool flame_sms_sent = false;



bool gas_display = true;
bool flame_display = false;
bool location_display = false;

float sensorValue1;
float sensorValue2;
int sensorValue3;
int sensorValue4;

bool first_data_receive = true;
bool debug = false;
String cdata; // complete data, consisting of sensors values
void setup()
{
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("LCD OK");  
  delay(1000); 
  Serial.begin(9600);
  mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  lcd.setCursor(0,0);
  lcd.print("Serial OK");  

  delay(1000); 
  lcd.clear();

  lcd.setCursor(2,0);
  lcd.print("Gas & Fire"); 

  lcd.setCursor(3,1);
  lcd.print("Detector");

  delay(2000); 

  pinMode(flameSensor1,INPUT);
  pinMode(flameSensor2,INPUT);
  pinMode(relay1,OUTPUT);
  digitalWrite(relay1,LOW);
  
  SMS_Initial();
  SendInitial1();
 
  mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000); // Delay of 1000 milli seconds or 1 second

  SMS_Initial();
  SendInitial2();
  mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000); // Delay of 1000 milli seconds or 1 second

  
  SMS_Initial();
  SendInitial3();
  mySerial.println((char)26);// ASCII code of CTRL+Z
 // Serial.print(" test");
    lcd.clear();
   lcd.setCursor(0,1);
  lcd.print("INITIAL OK!");

  delay(2000); // Delay of 1000 milli seconds or 1 second
   lcd.clear();
}

void loop()
{


//  if (first_data_receive)
//  {
  // put your main code here, to run repeatedly:
   sensorValue1  = analogRead(gasSensor1);
   sensorValue2  = analogRead(gasSensor2);
   sensorValue3 = digitalRead(flameSensor1);
   sensorValue4 = digitalRead(flameSensor2);


      if( sensorValue1 > 100){ //Gas level

        digitalWrite(relay1,HIGH);
        SMS_Initial();
        SendMessageGas1();
    //    GPS_Location();

        SMS_Initial();
        SendMessageGas2();
        //GPS_Location();

        SMS_Initial();
        SendMessageGas3();
        //GPS_Location();

      }   
      else if( sensorValue2 > 100){ //Gas level
        digitalWrite(relay1,HIGH);
        SMS_Initial();
        SendMessageGas1();
        //GPS_Location();

        SMS_Initial();
        SendMessageGas2();
       // GPS_Location();

        SMS_Initial();
        SendMessageGas3();
       // GPS_Location();
      }
      else if( sensorValue3 == 0 ){
        digitalWrite(relay1,HIGH);
        SMS_Initial();
        SendMessageFlame1();
    //    GPS_Location();     

        SMS_Initial();
        SendMessageFlame2();
     //   GPS_Location();     

        SMS_Initial();
        SendMessageFlame3();
    //    GPS_Location();     

      }   
      else if( sensorValue4 == 0){
        digitalWrite(relay1,HIGH);
        SMS_Initial();
        SendMessageFlame1();
       // GPS_Location();

        SMS_Initial();
        SendMessageFlame2();
       // GPS_Location();     

        SMS_Initial();
        SendMessageFlame3();
       // GPS_Location();     
      }
      else{
        digitalWrite(relay1,LOW);
        
      }

      lcd.clear();    

    if(gas_display){
      lcd.setCursor(0,0);
      lcd.print("Gas[1]:");
      lcd.setCursor(7,0);
      lcd.print(sensorValue1,2); 
      lcd.setCursor(0,1);
      lcd.print("Gas[2]:"); 
      lcd.setCursor(7,1);
      lcd.print(sensorValue2,2);

      flame_display = true;
      gas_display = false;
     // location_display = false;
      delay(2000);
    }else if(flame_display){
      lcd.setCursor(0,0);
      lcd.print("FS1:");
      lcd.print(sensorValue3); 
      lcd.setCursor(0,1);
      lcd.print("FS2:"); 
      lcd.print(sensorValue4); 

      flame_display = false;
      gas_display = true;
      //location_display = true;
      delay(2000);
      
    }//else if(location_display){
    //  lcd.setCursor(0,0);
    //  lcd.print("LATTITUDE=");
    //  lcd.print(flat);
    //  lcd.setCursor(0,1);
    //  lcd.print("LONGITUDE=");
    //  lcd.print(flon);

   //   flame_display = false;
   //   gas_display = true;
    //  location_display = false;
  //  }

  if(debug){   
   Serial.print("sensorValue1= "); 
   Serial.print(sensorValue1);  
   Serial.print(",sensorValue2= ");
   Serial.print(sensorValue2);
   Serial.print(",sensorValue3= ");
   Serial.print(sensorValue3);
   Serial.print(",sensorValue4= ");
   Serial.println(sensorValue4); 
  }
      delay(2000);


//  }


//  cdata = cdata+sensorValue1+","+sensorValue2+","+sensorValue3+","+sensorValue4+'\n'; // comma will be used a delimeter
  cdata = cdata+sensorValue1+","+sensorValue2+","+sensorValue3+","+sensorValue4; // comma will be used a delimeter
  Serial.println(cdata); 
  cdata = ""; 
  delay(1000);

}

void SendMessageGas1()
{
  mySerial.println("AT+CMGS=\"+639352908039\"\r"); // Replace x with mobile number
//  mySerial.println("AT+CMGS=\"+639771930439\"\r"); // Replace x with mobile number
  delay(200);
  mySerial.println(gas_alert);// The SMS text you want to send    
  delay(200);
  mySerial.print("Gas[l]:");// The SMS text you want to send    
  delay(200);
  mySerial.println(sensorValue1);// The SMS text you want to send    
  delay(200);
  mySerial.print("Gas[2]:");// The SMS text you want to send    
  delay(200);
  mySerial.println(sensorValue2);// The SMS text you want to send    
  if(debug){   
  Serial.print("SMS Sent Gas Alert");    //Sets the GSM Module in Text Mode
  }
  delay(1000);
}
  
void SendMessageGas2()
{
  mySerial.println("AT+CMGS=\"+639512870668\"\r"); // Replace x with mobile number
  delay(200);
  mySerial.println(gas_alert);// The SMS text you want to send    
  delay(200);
  mySerial.print("Gas[l]:");// The SMS text you want to send    
  delay(200);
  mySerial.println(sensorValue1);// The SMS text you want to send    
  delay(200);
  mySerial.print("Gas[2]:");// The SMS text you want to send    
  delay(200);
  mySerial.println(sensorValue2);// The SMS text you want to send    
    if(debug){   
      Serial.print("SMS Sent Gas Alert");    //Sets the GSM Module in Text Mode
    }
  delay(1000);
}

void SendMessageGas3()
{
  mySerial.println("AT+CMGS=\"+639676347375\"\r"); // Replace x with mobile number
  delay(200);
  mySerial.println(gas_alert);// The SMS text you want to send    
  delay(200);
  mySerial.print("Gas[l]:");// The SMS text you want to send    
  delay(200);
  mySerial.println(sensorValue1);// The SMS text you want to send    
  delay(200);
  mySerial.print("Gas[2]:");// The SMS text you want to send    
  delay(200);
  mySerial.println(sensorValue2);// The SMS text you want to send    
    if(debug){   
      Serial.print("SMS Sent Gas Alert");    //Sets the GSM Module in Text Mode
    }
  delay(1000);
}


void SendMessageFlame1()
{
  mySerial.println("AT+CMGS=\"+639352908039\"\r"); // Replace x with mobile number
//  mySerial.println("AT+CMGS=\"+639771930439\"\r"); // Replace x with mobile number

  delay(1000);
  mySerial.println(fire_alert);// The SMS text you want to send   
    if(debug){   
      Serial.print("SMS Sent Fire Alert");    //Sets the GSM Module in Text Mode
    }
}

void SendMessageFlame2()
{
  mySerial.println("AT+CMGS=\"+639512870668\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println(fire_alert);// The SMS text you want to send   
    if(debug){   
      Serial.print("SMS Sent Fire Alert");    //Sets the GSM Module in Text Mode
    }
}
  

void SendMessageFlame3()
{
  mySerial.println("AT+CMGS=\"+639676347375\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println(fire_alert);// The SMS text you want to send   
    if(debug){   
      Serial.print("SMS Sent Fire Alert");    //Sets the GSM Module in Text Mode
    }
}
  

void SMS_Initial(void){
  mySerial.println("AT");           //Sets the GSM Module in Text Mode
  delay(1000);                      // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);                      // Delay of 1000 milli seconds or 1 second
}
/*
void GPS_Location(void){
  mySerial.print("LAT=");// The SMS text you want to send
  mySerial.print(flat);// The SMS text you want to send
  mySerial.print(",");// The SMS text you want to send
  mySerial.print("LON=");// The SMS text you want to send
  mySerial.println(flon);// The SMS text you want to send
  delay(100);
  mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);                    // Delay of 1000 milli seconds or 1 second
}
*/
void SendInitial1(void)
{
  mySerial.println("AT+CMGS=\"+639512870668\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("Gas & Flame Detector Initial OK");// The SMS text you want to send   
  //Serial.print("Initial Ok");    //Sets the GSM Module in Text Mode
  delay(1000);
}

void SendInitial2(void)
{
  mySerial.println("AT+CMGS=\"+639676347375\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("Gas & Flame Detector Initial OK");// The SMS text you want to send   
  //Serial.print("Initial Ok");    //Sets the GSM Module in Text Mode
  delay(1000);
}

void SendInitial3(void)
{
  mySerial.println("AT+CMGS=\"+639352908039\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("Gas & Flame Detector Initial OK");// The SMS text you want to send   
  //Serial.print("Initial Ok");    //Sets the GSM Module in Text Mode
  delay(1000);
}
