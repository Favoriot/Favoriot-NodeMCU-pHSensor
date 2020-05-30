#include <SPI.h>
#include <ESP8266WiFi.h>
#include<Adafruit_GFX.h>
#include<Adafruit_SSD1306.h>

char ssid[] = "favoriot@unifibiz";      
char pass[] = "fav0r10t2017";  
const String myDevice = "NodePHSens@dex2813"; 
char server[] = "apiv2.favoriot.com";

int status = WL_IDLE_STATUS;
WiFiClient client;

unsigned long int avgValue;  //Store the average value of the sensor feedback
float b;
int buf[10],temp;
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH,SCREEN_HEIGHT,&Wire,OLED_RESET);

void setup() {
  Serial.begin(115200);
  WiFi.disconnect();
  Serial.println("Connecting...");
  WiFi.begin(ssid,pass);
  
  while((!(WiFi.status() == WL_CONNECTED))){
    delay(300);
    Serial.print(".");
  }
  
  Serial.println(WiFi.status());
  Serial.println("Connected!");
  Serial.println("");
   if(!display.begin(SSD1306_SWITCHCAPVCC,0x3c))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);//Dont proceed 
  }
display.display();
delay(2);
display.clearDisplay();
display.clearDisplay();
display.setTextColor(WHITE);
display.setTextSize(2);
display.setCursor(0,5);
display.print("PH Sensor");
display.display();
delay(3000);
}

void loop() {
  //String myData = String(analogRead(A0));
    for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(A0);
    delay(10);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
    avgValue+=buf[i];
  float phValue=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
  phValue=3.5*phValue;  //convert the millivolt into pH value
  phValue=phValue-4;
  //String myData = String(analogRead(A0))
  Serial.print("    pH:");  
  Serial.print(phValue,2);
  Serial.println(" ");
  //=============================oled display======================================
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(20,0);
  display.print("PH Value");

  display.setTextSize(3);
  display.setCursor(30,30);
  display.print(phValue);
  display.display();

  //=========================json favoriot data send ==================================================
  String json = "{\"device_developer_id\":\""+myDevice+"\",\"data\":{\"PH Value:\":\""+phValue+"\"}}";
  Serial.println(json);
  if (client.connect(server, 80)) {
    client.println("POST /v2/streams HTTP/1.1");
    client.println("Host: apiv2.favoriot.com");

    client.println(F("apikey:eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VybmFtZSI6ImRleDI4MTMiLCJyZWFkX3dyaXRlIjp0cnVlLCJpYXQiOjE1ODM4MjcyNjB9.CmQYQSH-rg1GHWOnJJx9yj3z9hAr_ZPL1xg0AA3XIz8"));  // change it!
    
    client.println("Content-Type: application/json");
    client.println("cache-control: no-cache");
    client.print("Content-Length: ");
    int thisLength = json.length();
    client.println(thisLength);
    client.println("Connection: close");
    client.println();
    client.println(json);
  }
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }
  if (!client.connected()) {
    client.stop();
  }
  delay(5000);
}
