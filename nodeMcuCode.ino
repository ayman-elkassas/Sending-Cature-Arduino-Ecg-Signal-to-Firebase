#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <FirebaseObject.h>

// Set these to run example.
#define FIREBASE_HOST "ecg-checker.firebaseio.com"
#define FIREBASE_AUTH "JDoi1oBmga66MdN24avq8svmdMKhpyPrXJQNOa6p"
#define WIFI_SSID "ezayk23moraaa"
#define WIFI_PASSWORD "salama18"

static int ecgValues[1000];
static int counter=0;
static int limit=0;

char res[10];
int n=0;

void setup() {
  Serial.begin(9600);
  counter=0;
  n=0;
  limit=0;
  // connect to wifi.
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("connecting");
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(500);
    }
    Serial.println();
    Serial.print("connected: ");
    Serial.println(WiFi.localIP());
    
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

    pinMode(D2,OUTPUT);
}

void loop() {
  
  if(digitalRead(D3)==HIGH)
  {
    Firebase.remove("patient/G6B7stSxsXbKOMGYBMGrnbkCNNy1/signal");
    digitalWrite(D2,HIGH);
  }
  else
  {
    int flag=digitalRead(D5);
    if(flag==LOW && digitalRead(D4)==LOW)
    {
        int val=digitalRead(D6);
        Serial.print(val);
        delay(1000);
        ecgValues[counter]=val;
        counter++;
    }
    if(flag==HIGH)
    {
      Serial.println();
      Serial.println("ok");
      ecgValues[counter]=2;
      counter++;
      delay(1000);
    }
    
    if(digitalRead(D4)==HIGH)
    {
      if(n<counter)
      {
        itoa(n,res,10);
        Firebase.push("patient/G6B7stSxsXbKOMGYBMGrnbkCNNy1/signal",ecgValues[n]);
        //handle error
        if (Firebase.failed()) {
            Serial.print("setting /number failed:");
            Serial.println(Firebase.error());  
            return;
        }
        n++;
      }
      else
      {
        counter=0;
        n=0;
      }
    }
  }
  
}

int ConcatinateInt(int x,int y)
{
  int POW=10;
  while(y>=POW)
  {
    POW*=10;
  }
  return x*POW+y;
}

/* Function to convert a binary number to decimal number */
long binaryToDecimal(long n) {
 int remainder; 
    long decimal = 0, i=0;
    while(n != 0) {
        remainder = n%10;
        n = n/10;
        decimal = decimal + (remainder*pow(2,i));
        ++i;
    }
    return decimal;
}
