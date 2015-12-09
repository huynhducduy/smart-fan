#include <dht.h>
dht DHT;
#define DHT11_PIN 6
void setup()
{
  Serial.begin(9600);
}
void loop()
{
  int chk = DHT.read11(DHT11_PIN);
  /* switch (chk)
  {
    case DHTLIB_OK:  
      Serial.print("OK,\t"); 
      break;
    case DHTLIB_ERROR_CHECKSUM: 
      Serial.print("Checksum error,\t"); 
      break;
    case DHTLIB_ERROR_TIMEOUT: 
      Serial.print("Time out error,\t"); 
      break;
    case DHTLIB_ERROR_CONNECT:
      Serial.print("Connect error,\t");
      break;
    case DHTLIB_ERROR_ACK_L:
      Serial.print("Ack Low error,\t");
      break;
    case DHTLIB_ERROR_ACK_H:
      Serial.print("Ack High error,\t");
      break;
    default: 
      Serial.print("Unknown error,\t"); 
      break;
  } */
  Serial.print(DHT.humidity,1);
  Serial.print("%");
  Serial.print(",\t");
  Serial.print(DHT.temperature,1);
  Serial.println("(C)");
  delay(1000);
}
