
#define ESP_TX1 17
#define ESP_RX1 18

HardwareSerial STM(1); // UART1



void setup() {
  Serial.begin(115200); // USB to PC
  STM.begin(115200, SERIAL_8N1, ESP_RX1, ESP_TX1); // UART1 to STM
  delay(5000);
  Serial.println("ESP32 -> STM32 TX bridge ready.");
  Serial.println("Type 1 and press Send to toggle STM led");
  
  //Serial.println("Type characters here: they go to STM32.");

}

void loop() {
//  Serial.println("hello from esp32");
//  delay(5000);
  // PC -> STM32
  if(Serial.available()){
    char c = Serial.read();
    STM.write(c);
  }
//
//  // STM32 -> PC
//  if(STM.available()){
//    char c = Serial.read();
//    Serial.write(c);
//  }
   

}
