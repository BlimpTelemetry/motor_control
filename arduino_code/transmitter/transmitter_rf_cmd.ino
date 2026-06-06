#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

const int TX_PIN = 1; // Broche de données connectée à l'émetteur (DATA)

void setup() {
  Serial.begin(115200);
  
  // Initialisation de l'émetteur
  mySwitch.enableTransmit(TX_PIN);
  
  Serial.println("--- Transmetteur (rc-switch) Prêt ---");
}

void loop() {
  if (Serial.available() > 0) {
    char touche = Serial.read();
    touche = toLowerCase(touche);
    
    // On envoie des codes chiffrés sur 24 bits (standard rc-switch)
    switch (touche) {
      case 'w':
        Serial.println("Envoi : Moteur AVANT (W)");
        mySwitch.send(1001, 24); 
        break;
      case 's':
        Serial.println("Envoi : Moteur ARRIÈRE (S)");
        mySwitch.send(1002, 24);
        break;
      case 'a':
        Serial.println("Envoi : Moteur GAUCHE (A)");
        mySwitch.send(1003, 24);
        break;
      case 'd':
        Serial.println("Envoi : Moteur DROITE (D)");
        mySwitch.send(1004, 24);
        break;
      case '\n': 
      case '\r':
        break;
      default:
        Serial.println("Touche non reconnue.");
        break;
    }
    delay(50); 
  }
}