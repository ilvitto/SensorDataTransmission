#include <ESP8266WiFi.h>
#include <Wire.h>

const char* ssid = "SSID";
const char* password = "password";

const int MPU=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

IPAddress ip(192,168,43,222);  //Node static IP
IPAddress gateway(192,168,43,1);
IPAddress subnet(255,255,255,0);

WiFiServer server(80);
WiFiClient serverClient;
int j;
void setup() {
  Wire.begin(4, 5); // sda, scl
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

  j = 0;
  
  Serial.begin(115200);
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
  Serial.print("\nConnecting to "); Serial.println(ssid);
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 20) 
    delay(500);
  if(i == 21){
    Serial.print("Could not connect to"); 
    Serial.println(ssid);
    while(1) 
      delay(500);
  }
  server.begin();
  server.setNoDelay(true);
  
  Serial.print("Connect to ");
  Serial.println(WiFi.localIP());
}

void loop() {
  uint8_t i;
  //check if there are any new clients
  if (server.hasClient()){
      if (!serverClient || !serverClient.connected()){
        serverClient = server.available();
        Serial.println("New client");
      }
    //WiFiClient serverClient2 = server.available();
    //serverClient2.stop();
  }
  /*//input data from device
  for(i = 0; i < MAX_SRV_CLIENTS; i++){
    if (serverClients[i] && serverClients[i].connected()){
      if(serverClients[i].available()){
        //get data from the telnet client and push it to the UART
        while(serverClients[i].available()) Serial.write(serverClients[i].read());
      }
    }
  }*/
    //GET AND PUSH DATA FROM MPU
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU,14,true);  // request a total of 14 registers
    AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
    AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

    j++;
    Serial.print(j);
    Serial.print(AcX); 
    Serial.print(" ");
    Serial.print(AcY);
    Serial.print(" ");
    Serial.print(AcZ);
    Serial.print(" ");
    Serial.print(GyX);
    Serial.print(" ");
    Serial.print(GyY);
    Serial.print(" ");
    Serial.println(GyZ);
    size_t len = 12;
    uint8_t buff[len];
    buff[1] = AcX;
    buff[0] = AcX >> 8;
    buff[3] = AcY;
    buff[2] = AcY >> 8;
    buff[5] = AcZ;
    buff[4] = AcZ >> 8;
    buff[7] = GyX;
    buff[6] = GyX >> 8;
    buff[9] = GyY;
    buff[8] = GyY >> 8;
    buff[11] = GyZ;
    buff[10] = GyZ >> 8;
    
    /*size_t len = Serial.available();
    uint8_t sbuf[len];
    Serial.readBytes(sbuf, len);
    
    for(int i =0; i < len; i++)
      Serial.println(sbuf[i]);
      */
    if (serverClient && serverClient.connected()){
      serverClient.write(buff, len);
      delay(1);
    }
  }

