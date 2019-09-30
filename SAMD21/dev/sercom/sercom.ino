#include <Arduino.h>   // required before wiring_private.h
#include "wiring_private.h" // pinPeripheral() function

String inputString = "";
boolean USBstringComplete = false;
boolean ser2_stringComplete = false;

//SERCOM_RX_PAD_0 means RX on SERCOMn.0
//SERCOM_RX_PAD_1 means RX on SERCOMn.1
//SERCOM_RX_PAD_2 means RX on SERCOMn.2
//SERCOM_RX_PAD_3 means RX on SERCOMn.3
//and then for TX:
//
//UART_TX_PAD_0 means TX on SERCOMn.0
//UART_TX_PAD_2 means TX on SERCOMn.2


/// Serial 2 Configuration
//PA17  D13   SERCOM1.1 SERCOM3.1
//PA18  D10   SERCOM1.2 SERCOM3.2
Uart Serial2 (&sercom1, 13, 10, SERCOM_RX_PAD_1, UART_TX_PAD_2);
void SERCOM1_Handler()
{
  Serial2.IrqHandler();
}


/// Serial 3 Configuration

//PA19  D12   SERCOM1.3 SERCOM3.3
//PA16  D11   SERCOM1.0 SERCOM3.0

Uart Serial3 (&sercom3, 12, 11, SERCOM_RX_PAD_3, UART_TX_PAD_0);
void SERCOM3_Handler()
{
  Serial3.IrqHandler();
}


void setup() {
  // Open serial communications and wait for port to open:
  SerialUSB.begin(115200);

  // set the data rate for the SoftwareSerial port
  Serial2.begin(115200);
  Serial3.begin(115200);

    // Assign pins 10 & 11 SERCOM functionality
  pinPeripheral(13, PIO_SERCOM); //TX
  pinPeripheral(10, PIO_SERCOM); //RX

    // Assign pins 3 & 4 SERCOM functionality
  pinPeripheral(12, PIO_SERCOM_ALT); //TX
  pinPeripheral(11, PIO_SERCOM_ALT); //RX

  
}
void loop() {
  serialEvent();
  if (ser2_stringComplete) {
    SerialUSB.println(inputString);
    inputString = "";
    ser2_stringComplete = false;
  }
  
  Serial2.println("G2204 Z10 F5000");
  Serial3.println("G2204 X10 F5000");
  delay(3000);

  serialEvent();
  if (ser2_stringComplete) {
    SerialUSB.println(inputString);
    inputString = "";
    ser2_stringComplete = false;
  }
  

  Serial2.println("G2204 Z-10 F5000");
  Serial3.println("G2204 X-10 F5000");
  delay(3000);

}

void serialEvent() {
  while (SerialUSB.available()) {
    char inChar = (char)SerialUSB.read();
    inputString += inChar;
    if (inChar == '\n') {
      USBstringComplete = true;
    }
  }
  while (Serial2.available()) {
    char inChar = (char)Serial2.read();
    inputString += inChar;
    if (inChar == '\n') {
      ser2_stringComplete = true;
    }
  }
}
