//* Code by EL Aydi ******
 
//*
//*************************************"**********************//

#include <TinyGPS.h>
#include <SoftwareSerial.h>

SoftwareSerial SIM900(7, 8);//Les broches 7 et 8 sont déclarées comme TX et RX
TinyGPS gps;  //Creer une nouvelle instance de tinyGPS 
             
void setup()
{
  Serial.begin(9600);//réglage taux de saut du module gsm 
  SIM900.begin(9600); //réglage taux de saut du moniteur série
}

void loop()
{
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  //Pendant une seconde, nous analysons les données GPS et rapportons certaines valeurs clés
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (Serial.available())
    {
      char c = Serial.read();
      if (gps.encode(c)) 
        newData = true;  
    }
  }

  if (newData)      //si newData est true
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);   
    SIM900.print("AT+CMGF=1\r"); 
    delay(400);
    SIM900.println("AT + CMGS = \"+212xxxxxxxxx\"");// numéro de mobile du destinataire avec le code du pays
    delay(300);
    SIM900.print("Latitude = ");
    SIM900.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    SIM900.print(" Longitude = ");
    SIM900.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    delay(200);
    SIM900.println((char)26); // Fin de la commande AT avec un ^ Z, ASCII code 26
    delay(200);
    SIM900.println();  
  }
  Serial.println(failed);
      }
