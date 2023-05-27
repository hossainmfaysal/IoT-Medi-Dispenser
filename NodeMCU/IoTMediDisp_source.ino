#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <Wire.h> 
#include <RtcDS3231.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

const char* ssid = "***";
const char* password = "***";
const char* firebaseHost = "***";
const char* firebaseAuth = "***";
const char* locatF;
char dose[17];
char doseAll[42][17];
char currTime[17]; 
int n, i, k, t, x, stackno, btnC, pos;

byte dT = 0;
byte address = 0x0010;

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
RtcDS3231<TwoWire> rtcObject(Wire);
Servo grabber;
Servo releaser;

FirebaseData fbdo; 


void setup() 
{ 
  Wire.begin();
  rtcObject.Begin();
  lcd.begin (16,2);
  WiFi.begin(ssid, password);
  lcd.backlight();

  lcd.clear();
  lcd.setCursor(1 , 0);
  lcd.print("W E L C O M E");  
  delay(2000);

  RtcDateTime currentTime = RtcDateTime(23, 04, 26, 23, 40, 00); //yy, mm, dd, hh, mm, ss
  rtcObject.SetDateTime(currentTime);


  while (WiFi.status() != WL_CONNECTED) 
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Connecting to");  
    lcd.setCursor(0, 1);
    lcd.print("WiFi...");
    delay(1000);
  }

  pinMode (3, INPUT_PULLUP);  // button
  pinMode (1, OUTPUT);  // buzzer
  pinMode (0, OUTPUT);  //A0 demux
  pinMode (14, OUTPUT);  //A1 demux
  pinMode (12, OUTPUT);  //A2 demux
  pinMode (16, OUTPUT);  //IN1
  pinMode (2, OUTPUT);  //IN2

  grabber.attach(13, 1500, 3500); 
  releaser.attach(15, 1500, 3500);
  Firebase.begin(firebaseHost, firebaseAuth);

  dataFetch();
}


void dataFetch()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Fetching data");  
  lcd.setCursor(0, 1);
  lcd.print("from database..");  

  for(n=1; n<43; n++)
  {
    switch (n)
    {
      case 1:
      locatF = "MedicineDoses/1/Time";
      break;
      case 2:
      locatF = "MedicineDoses/2/Time";
      break;
      case 3:
      locatF = "MedicineDoses/3/Time";
      break;
      case 4:
      locatF = "MedicineDoses/4/Time";
      break;
      case 5:
      locatF = "MedicineDoses/5/Time";
      break;
      case 6:
      locatF = "MedicineDoses/6/Time";
      break;
      case 7:
      locatF = "MedicineDoses/7/Time";
      break;
      case 8:
      locatF = "MedicineDoses/8/Time";
      break;
      case 9:
      locatF = "MedicineDoses/9/Time";
      break;
      case 10:
      locatF = "MedicineDoses/10/Time";
      break;
      case 11:
      locatF = "MedicineDoses/11/Time";
      break;
      case 12:
      locatF = "MedicineDoses/12/Time";
      break;
      case 13:
      locatF = "MedicineDoses/13/Time";
      break;
      case 14:
      locatF = "MedicineDoses/14/Time";
      break;
      case 15:
      locatF = "MedicineDoses/15/Time";
      break;
      case 16:
      locatF = "MedicineDoses/16/Time";
      break;
      case 17:
      locatF = "MedicineDoses/17/Time";
      break;
      case 18:
      locatF = "MedicineDoses/18/Time";
      break;
      case 19:
      locatF = "MedicineDoses/19/Time";
      break;
      case 20:
      locatF = "MedicineDoses/20/Time";
      break;
      case 21:
      locatF = "MedicineDoses/21/Time";
      break;
      case 22:
      locatF = "MedicineDoses/22/Time";
      break;
      case 23:
      locatF = "MedicineDoses/23/Time";
      break;
      case 24:
      locatF = "MedicineDoses/24/Time";
      break;
      case 25:
      locatF = "MedicineDoses/25/Time";
      break;
      case 26:
      locatF = "MedicineDoses/26/Time";
      break;
      case 27:
      locatF = "MedicineDoses/27/Time";
      break;
      case 28:
      locatF = "MedicineDoses/28/Time";
      break;
      case 29:
      locatF = "MedicineDoses/29/Time";
      break;
      case 30:
      locatF = "MedicineDoses/30/Time";
      break;
      case 31:
      locatF = "MedicineDoses/31/Time";
      break;
      case 32:
      locatF = "MedicineDoses/32/Time";
      break;
      case 33:
      locatF = "MedicineDoses/33/Time";
      break;
      case 34:
      locatF = "MedicineDoses/34/Time";
      break;
      case 35:
      locatF = "MedicineDoses/35/Time";
      break;
      case 36:
      locatF = "MedicineDoses/36/Time";
      break;
      case 37:
      locatF = "MedicineDoses/37/Time";
      break;
      case 38:
      locatF = "MedicineDoses/38/Time";
      break;
      case 39:
      locatF = "MedicineDoses/39/Time";
      break;
      case 40:
      locatF = "MedicineDoses/40/Time";
      break;
      case 41:
      locatF = "MedicineDoses/41/Time";
      break;
      case 42:
      locatF = "MedicineDoses/42/Time";
      break;
    }

    Firebase.getString(fbdo, locatF);
    String doseTime = fbdo.stringData();
    doseTime.toCharArray(dose, 17);

    for(i=0; i<17; i++)
    {
      doseAll[n-1][i] = dose[i];
    }  
  }
}


void idleAct2()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Next Dose:");  
  lcd.setCursor(11, 0);
  lcd.print(dT+1);
  lcd.setCursor(0, 1);
  lcd.print(doseAll[dT]);  
  delay(60000); // 1 min
}


void takenDose()
{
  t=dT+1;

   switch(t) 
  {
    case 1:
    Firebase.setString(fbdo, "MedicineDoses/1/mStatus", "TAKEN"); 
    break;
    case 2:
    Firebase.setString(fbdo, "MedicineDoses/2/mStatus", "TAKEN"); 
    break;
    case 3:
    Firebase.setString(fbdo, "MedicineDoses/3/mStatus", "TAKEN"); 
    break;
    case 4:
    Firebase.setString(fbdo, "MedicineDoses/4/mStatus", "TAKEN"); 
    break;
    case 5:
    Firebase.setString(fbdo, "MedicineDoses/5/mStatus", "TAKEN"); 
    break;
    case 6:
    Firebase.setString(fbdo, "MedicineDoses/6/mStatus", "TAKEN"); 
    break;
    case 7:
    Firebase.setString(fbdo, "MedicineDoses/7/mStatus", "TAKEN"); 
    break;
    case 8:
    Firebase.setString(fbdo, "MedicineDoses/8/mStatus", "TAKEN"); 
    break;
    case 9:
    Firebase.setString(fbdo, "MedicineDoses/9/mStatus", "TAKEN"); 
    break;
    case 10:
    Firebase.setString(fbdo, "MedicineDoses/10/mStatus", "TAKEN"); 
    break;
    case 11:
    Firebase.setString(fbdo, "MedicineDoses/11/mStatus", "TAKEN"); 
    break;
    case 12:
    Firebase.setString(fbdo, "MedicineDoses/12/mStatus", "TAKEN"); 
    break;
    case 13:
    Firebase.setString(fbdo, "MedicineDoses/13/mStatus", "TAKEN"); 
    break;
    case 14:
    Firebase.setString(fbdo, "MedicineDoses/14/mStatus", "TAKEN"); 
    break;
    case 15:
    Firebase.setString(fbdo, "MedicineDoses/15/mStatus", "TAKEN"); 
    break;
    case 16:
    Firebase.setString(fbdo, "MedicineDoses/16/mStatus", "TAKEN"); 
    break;
    case 17:
    Firebase.setString(fbdo, "MedicineDoses/17/mStatus", "TAKEN"); 
    break;
    case 18:
    Firebase.setString(fbdo, "MedicineDoses/18/mStatus", "TAKEN"); 
    break;
    case 19:
    Firebase.setString(fbdo, "MedicineDoses/19/mStatus", "TAKEN"); 
    break;
    case 20:
    Firebase.setString(fbdo, "MedicineDoses/20/mStatus", "TAKEN"); 
    break;
    case 21:
    Firebase.setString(fbdo, "MedicineDoses/21/mStatus", "TAKEN"); 
    break;
    case 22:
    Firebase.setString(fbdo, "MedicineDoses/22/mStatus", "TAKEN"); 
    break;
    case 23:
    Firebase.setString(fbdo, "MedicineDoses/23/mStatus", "TAKEN"); 
    break;
    case 24:
    Firebase.setString(fbdo, "MedicineDoses/24/mStatus", "TAKEN"); 
    break;
    case 25:
    Firebase.setString(fbdo, "MedicineDoses/25/mStatus", "TAKEN"); 
    break;
    case 26:
    Firebase.setString(fbdo, "MedicineDoses/26/mStatus", "TAKEN"); 
    break;
    case 27:
    Firebase.setString(fbdo, "MedicineDoses/27/mStatus", "TAKEN"); 
    break;
    case 28:
    Firebase.setString(fbdo, "MedicineDoses/28/mStatus", "TAKEN"); 
    break;
    case 29:
    Firebase.setString(fbdo, "MedicineDoses/29/mStatus", "TAKEN"); 
    break;
    case 30:
    Firebase.setString(fbdo, "MedicineDoses/30/mStatus", "TAKEN"); 
    break;
    case 31:
    Firebase.setString(fbdo, "MedicineDoses/31/mStatus", "TAKEN"); 
    break;
    case 32:
    Firebase.setString(fbdo, "MedicineDoses/32/mStatus", "TAKEN"); 
    break;
    case 33:
    Firebase.setString(fbdo, "MedicineDoses/33/mStatus", "TAKEN"); 
    break;
    case 34:
    Firebase.setString(fbdo, "MedicineDoses/34/mStatus", "TAKEN"); 
    break;
    case 35:
    Firebase.setString(fbdo, "MedicineDoses/35/mStatus", "TAKEN"); 
    break;
    case 36:
    Firebase.setString(fbdo, "MedicineDoses/36/mStatus", "TAKEN"); 
    break;
    case 37:
    Firebase.setString(fbdo, "MedicineDoses/37/mStatus", "TAKEN"); 
    break;
    case 38:
    Firebase.setString(fbdo, "MedicineDoses/38/mStatus", "TAKEN"); 
    break;
    case 39:
    Firebase.setString(fbdo, "MedicineDoses/39/mStatus", "TAKEN"); 
    break;
    case 40:
    Firebase.setString(fbdo, "MedicineDoses/40/mStatus", "TAKEN"); 
    break;
    case 41:
    Firebase.setString(fbdo, "MedicineDoses/41/mStatus", "TAKEN"); 
    break;
    case 42:
    Firebase.setString(fbdo, "MedicineDoses/42/mStatus", "TAKEN"); 
    break;
  }

  dT++;

  Wire.beginTransmission(0x57);  // start transmission to AT24C32 EEPROM (address 0x57)
  Wire.write(highByte(address)); 
  Wire.write(lowByte(address)); 
  Wire.write(dT);              
  Wire.endTransmission();      

  idleAct2();
}


void dispense()
{
  if (dT>=0 && dT<=6) 
    stackno = 1;
  else if (dT>=7 && dT<=13)
    stackno = 2;
  else if (dT>=14 && dT<=20)
      stackno = 3;
  else if (dT>=21 && dT<=27)
    stackno = 4;
  else if (dT>=28 && dT<=34)
    stackno = 5;
  else if (dT>=35 && dT<=41)
    stackno = 6;   

  switch (stackno)
  {
    case 1:
    digitalWrite(0, LOW); //A0
    digitalWrite(14, LOW); //A1
    digitalWrite(12, LOW); //A2
    break;
    case 2:
    digitalWrite(0, HIGH);
    digitalWrite(14, LOW);
    digitalWrite(12, LOW);
    break;
    case 3:
    digitalWrite(0, LOW);
    digitalWrite(14, HIGH);
    digitalWrite(12, LOW);
    break;  
    case 4:
    digitalWrite(0, HIGH);
    digitalWrite(14, HIGH);
    digitalWrite(12, LOW);
    break;
    case 5:
    digitalWrite(0, LOW);
    digitalWrite(14, LOW);
    digitalWrite(12, HIGH);
    break;
    case 6:
    digitalWrite(0, HIGH);
    digitalWrite(14, LOW);
    digitalWrite(12, HIGH);
    break;  
  } 

   for(pos=0; pos<=40; pos+=5)
  {
    grabber.write(pos);
    delay(15);
  }
  delay(500);
  for(pos=0; pos<=120; pos+=5)
  {
    releaser.write(pos);
    delay(15);
  }
  delay(500);
  for(pos=120; pos>=0; pos-=5)
  {
    releaser.write(pos);
    delay(15);
  }
  delay(500);
  for(pos=40; pos>=0; pos-=5)
  {
    grabber.write(pos);
    delay(15);
  }  

  digitalWrite(16, HIGH); 
  digitalWrite(2, LOW);
  delay(10000);
  digitalWrite(16, LOW); 
  digitalWrite(2, LOW);
  delay(10);
  
  takenDose();
}


void missedDose()
{
  k=dT+1;

   switch(k) 
  {
    case 1:
    Firebase.setString(fbdo, "MedicineDoses/1/mStatus", "MISSED"); 
    break;
    case 2:
    Firebase.setString(fbdo, "MedicineDoses/2/mStatus", "MISSED"); 
    break;
    case 3:
    Firebase.setString(fbdo, "MedicineDoses/3/mStatus", "MISSED"); 
    break;
    case 4:
    Firebase.setString(fbdo, "MedicineDoses/4/mStatus", "MISSED"); 
    break;
    case 5:
    Firebase.setString(fbdo, "MedicineDoses/5/mStatus", "MISSED"); 
    break;
    case 6:
    Firebase.setString(fbdo, "MedicineDoses/6/mStatus", "MISSED"); 
    break;
    case 7:
    Firebase.setString(fbdo, "MedicineDoses/7/mStatus", "MISSED"); 
    break;
    case 8:
    Firebase.setString(fbdo, "MedicineDoses/8/mStatus", "MISSED"); 
    break;
    case 9:
    Firebase.setString(fbdo, "MedicineDoses/9/mStatus", "MISSED"); 
    break;
    case 10:
    Firebase.setString(fbdo, "MedicineDoses/10/mStatus", "MISSED"); 
    break;
    case 11:
    Firebase.setString(fbdo, "MedicineDoses/11/mStatus", "MISSED"); 
    break;
    case 12:
    Firebase.setString(fbdo, "MedicineDoses/12/mStatus", "MISSED"); 
    break;
    case 13:
    Firebase.setString(fbdo, "MedicineDoses/13/mStatus", "MISSED"); 
    break;
    case 14:
    Firebase.setString(fbdo, "MedicineDoses/14/mStatus", "MISSED"); 
    break;
    case 15:
    Firebase.setString(fbdo, "MedicineDoses/15/mStatus", "MISSED"); 
    break;
    case 16:
    Firebase.setString(fbdo, "MedicineDoses/16/mStatus", "MISSED"); 
    break;
    case 17:
    Firebase.setString(fbdo, "MedicineDoses/17/mStatus", "MISSED"); 
    break;
    case 18:
    Firebase.setString(fbdo, "MedicineDoses/18/mStatus", "MISSED"); 
    break;
    case 19:
    Firebase.setString(fbdo, "MedicineDoses/19/mStatus", "MISSED"); 
    break;
    case 20:
    Firebase.setString(fbdo, "MedicineDoses/20/mStatus", "MISSED"); 
    break;
    case 21:
    Firebase.setString(fbdo, "MedicineDoses/21/mStatus", "MISSED"); 
    break;
    case 22:
    Firebase.setString(fbdo, "MedicineDoses/22/mStatus", "MISSED"); 
    break;
    case 23:
    Firebase.setString(fbdo, "MedicineDoses/23/mStatus", "MISSED"); 
    break;
    case 24:
    Firebase.setString(fbdo, "MedicineDoses/24/mStatus", "MISSED"); 
    break;
    case 25:
    Firebase.setString(fbdo, "MedicineDoses/25/mStatus", "MISSED"); 
    break;
    case 26:
    Firebase.setString(fbdo, "MedicineDoses/26/mStatus", "MISSED"); 
    break;
    case 27:
    Firebase.setString(fbdo, "MedicineDoses/27/mStatus", "MISSED"); 
    break;
    case 28:
    Firebase.setString(fbdo, "MedicineDoses/28/mStatus", "MISSED"); 
    break;
    case 29:
    Firebase.setString(fbdo, "MedicineDoses/29/mStatus", "MISSED"); 
    break;
    case 30:
    Firebase.setString(fbdo, "MedicineDoses/30/mStatus", "MISSED"); 
    break;
    case 31:
    Firebase.setString(fbdo, "MedicineDoses/31/mStatus", "MISSED"); 
    break;
    case 32:
    Firebase.setString(fbdo, "MedicineDoses/32/mStatus", "MISSED"); 
    break;
    case 33:
    Firebase.setString(fbdo, "MedicineDoses/33/mStatus", "MISSED"); 
    break;
    case 34:
    Firebase.setString(fbdo, "MedicineDoses/34/mStatus", "MISSED"); 
    break;
    case 35:
    Firebase.setString(fbdo, "MedicineDoses/35/mStatus", "MISSED"); 
    break;
    case 36:
    Firebase.setString(fbdo, "MedicineDoses/36/mStatus", "MISSED"); 
    break;
    case 37:
    Firebase.setString(fbdo, "MedicineDoses/37/mStatus", "MISSED"); 
    break;
    case 38:
    Firebase.setString(fbdo, "MedicineDoses/38/mStatus", "MISSED"); 
    break;
    case 39:
    Firebase.setString(fbdo, "MedicineDoses/39/mStatus", "MISSED"); 
    break;
    case 40:
    Firebase.setString(fbdo, "MedicineDoses/40/mStatus", "MISSED"); 
    break;
    case 41:
    Firebase.setString(fbdo, "MedicineDoses/41/mStatus", "MISSED"); 
    break;
    case 42:
    Firebase.setString(fbdo, "MedicineDoses/42/mStatus", "MISSED"); 
    break;
  }

  dT++;

  Wire.beginTransmission(0x57);  // start transmission to AT24C32 EEPROM (address 0x57)
  Wire.write(highByte(address)); 
  Wire.write(lowByte(address));  
  Wire.write(dT);            
  Wire.endTransmission();  
}


void missedAct()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dose");
  lcd.setCursor(5, 0);
  lcd.print(dT+1);
  lcd.setCursor(8, 0);
  lcd.print("Missed!");
  delay(1000); 

  if (dT>=0 && dT<=6) 
    stackno = 1;
  else if (dT>=7 && dT<=13)
    stackno = 2;
  else if (dT>=14 && dT<=20)
      stackno = 3;
  else if (dT>=21 && dT<=27)
    stackno = 4;
  else if (dT>=28 && dT<=34)
    stackno = 5;
  else if (dT>=35 && dT<=41)
    stackno = 6;   

  switch (stackno)
  {
    case 1:
    digitalWrite(0, LOW); //A0, D3
    digitalWrite(14, LOW); //A1, D5
    digitalWrite(12, LOW); //A2, D6
    break;
    case 2:
    digitalWrite(0, HIGH);
    digitalWrite(14, LOW);
    digitalWrite(12, LOW);
    break;
    case 3:
    digitalWrite(0, LOW);
    digitalWrite(14, HIGH);
    digitalWrite(12, LOW);
    break;  
    case 4:
    digitalWrite(0, HIGH);
    digitalWrite(14, HIGH);
    digitalWrite(12, LOW);
    break;
    case 5:
    digitalWrite(0, LOW);
    digitalWrite(14, LOW);
    digitalWrite(12, HIGH);
    break;
    case 6:
    digitalWrite(0, HIGH);
    digitalWrite(14, LOW);
    digitalWrite(12, HIGH);
    break;  
  } 

  for(pos=0; pos<=40; pos+=5)
  {
    grabber.write(pos);
    delay(15);
  }
  delay(500);
  for(pos=0; pos<=120; pos+=5)
  {
    releaser.write(pos);
    delay(15);
  }
  delay(500);
  for(pos=120; pos>=0; pos-=5)
  {
    releaser.write(pos);
    delay(15);
  }
  delay(500);
  for(pos=40; pos>=0; pos-=5)
  {
    grabber.write(pos);
    delay(15);
  }  

  digitalWrite(16, LOW); 
  digitalWrite(2, HIGH);
  delay(10000);
  digitalWrite(16, LOW); 
  digitalWrite(2, LOW);
  delay(10);
  
  missedDose();
}


void dispenseAct()
{
  unsigned long timerBegin = millis();
  while(millis()-timerBegin <= 60000) // 1 min
  {
    tone(1, 2351.97); // Send G
    delay(200);  
    tone(1, 1977.768); // Send E
    delay(200);  
    tone(1, 1569.78); // Send C
    delay(200);       
    noTone(1);  
    delay(50);  
    tone(1, 1569.78); // Send C
    delay(200); 
    tone(1, 1977.768); // Send E
    delay(200); 
    tone(1, 2351.97); // Send G
    delay(200);
    noTone(1);       
  
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("MEDICINE TIME!");  
    delay(2000);
    lcd.clear();
    delay(500);
    lcd.setCursor(0, 0);
    lcd.print("PRESS & HOLD to"); 
    lcd.setCursor(4, 1);
    lcd.print("DISPENSE..");
    delay(2000);

    if (!digitalRead(3)) 
    {
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("DISPENSING..");  
      lcd.setCursor(3, 1);
      lcd.print("Box:");  
      lcd.setCursor(8, 1);
      lcd.print(dT+1); 
      delay(1000);
      dispense();
      btnC=1;
    }
  }

  if ((millis()-timerBegin > 60000) && btnC == 0) // edit waiting time
  {
    missedAct();
  }
  btnC=0;


}

void idleAct()
{
  lcd.clear();
  lcd.setCursor(0, 0); 
  lcd.print("Next Dose:");  
  lcd.setCursor(11, 0);
  lcd.print(dT+1);
  lcd.setCursor(0, 1);
  lcd.print(doseAll[dT]);    
  delay(1000);
}

void loop() 
{
  for(i=1; i<43; i++) 
  {
    RtcDateTime currentTime = rtcObject.GetDateTime(); 
    int min = currentTime.Minute();
    char minC[3];
    if (min<10)
      sprintf(minC, "0%d", min);
    else
      sprintf(minC, "%d", min);
    int hh = currentTime.Hour();
    char hhC[3];
    if (hh<10)
      sprintf(hhC, "0%d", hh);
    else
      sprintf(hhC, "%d", hh);
    int mm = currentTime.Month();
    char mmC[3];
    if (mm<10)
      sprintf(mmC, "0%d", mm);
    else
      sprintf(mmC, "%d", mm);
    int dd = currentTime.Day();
    char ddC[3];
    if (dd<10)
      sprintf(ddC, "0%d", dd);
    else
      sprintf(ddC, "%d", dd);
    int yy = currentTime.Year();
    char yyC[5];
    sprintf(yyC, "%d", yy);
    sprintf(currTime, "%s-%s-%sT%s:%s", yyC, mmC, ddC, hhC, minC);

    Wire.beginTransmission(0x57);  // start transmission to AT24C32 EEPROM (address 0x50)
    Wire.write(highByte(address)); 
    Wire.write(lowByte(address));  
    Wire.endTransmission();     
    Wire.requestFrom(0x57, 1); 
    if (Wire.available()) {
      dT = Wire.read();   
    }

    x = strcmp(doseAll[i-1], currTime);
    if(x == 0)
    {
      dispenseAct();
    }
    else
    {
      idleAct();
    }

    if (!digitalRead(3)) 
    {
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Device REFILL");  
      delay(1000);
      dT=0;
      Wire.beginTransmission(0x57);  // start transmission to AT24C32 EEPROM (address 0x57)
      Wire.write(highByte(address)); 
      Wire.write(lowByte(address));  
      Wire.write(dT);          
      Wire.endTransmission();        
    }
    const char* blnk = "";
    int y = strcmp(doseAll[dT], blnk);
    
    while(y == 0)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Cartrige Empty");  
      lcd.setCursor(0, 1);
      lcd.print("Please REFILL");  
      delay(1000);
      if (!digitalRead(3)) 
      {
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("Device REFILL");  
        delay(1000);
        dT=0;
        Wire.beginTransmission(0x57);  // start transmission to AT24C32 EEPROM (address 0x57)
        Wire.write(highByte(address)); 
        Wire.write(lowByte(address)); 
        Wire.write(dT);           
        Wire.endTransmission();       
      }
    }

    while(dT>41)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Cartrige Empty");  
      lcd.setCursor(0, 1);
      lcd.print("Please REFILL");  
      delay(1000);
      if (!digitalRead(3)) 
      {
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("Device REFILL");  
        delay(1000);
        dT=0;
        Wire.beginTransmission(0x57);  // start transmission to AT24C32 EEPROM (address 0x57)
        Wire.write(highByte(address));
        Wire.write(lowByte(address)); 
        Wire.write(dT);           
        Wire.endTransmission();     
      }
    }
  }
}