#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

const int RX_PIN = 1; // Broche connectée à DATA du MX-RM-5V

// Broches moteurs
const int MOTEUR_AVANT = 2;
const int MOTEUR_ARRIERE = 3;
const int MOTEUR_GAUCHE = 4;
const int MOTEUR_DROITE = 5;

void setup() {
  Serial.begin(115200);
  
  pinMode(MOTEUR_AVANT, OUTPUT);
  pinMode(MOTEUR_ARRIERE, OUTPUT);
  pinMode(MOTEUR_GAUCHE, OUTPUT);
  pinMode(MOTEUR_DROITE, OUTPUT);
  
  // Initialisation du récepteur sur la broche RX_PIN
  mySwitch.enableReceive(digitalPinToInterrupt(RX_PIN));
  
  Serial.println("--- Récepteur (rc-switch) Prêt ---");
}

void loop() {
  // Si un signal valide est reçu
  if (mySwitch.available()) {
    
    // On récupère la valeur décodée
    long value = mySwitch.getReceivedValue();
    
    if (value == 0) {
      Serial.println("Erreur d'encodage inconnu");
    } else {
      
      // On éteint tout avant de changer de direction
      digitalWrite(MOTEUR_AVANT, LOW);
      digitalWrite(MOTEUR_ARRIERE, LOW);
      digitalWrite(MOTEUR_GAUCHE, LOW);
      digitalWrite(MOTEUR_DROITE, LOW);

      // On déclenche l'action correspondante
      switch (value) {
        case 1001: // Correspond à W
          Serial.println("Action : AVANCER");
          digitalWrite(MOTEUR_AVANT, HIGH);
          break;
        case 1002: // Correspond à S
          Serial.println("Action : RECULER");
          digitalWrite(MOTEUR_ARRIERE, HIGH);
          break;
        case 1003: // Correspond à A
          Serial.println("Action : GAUCHE");
          digitalWrite(MOTEUR_GAUCHE, HIGH);
          break;
        case 1004: // Correspond à D
          Serial.println("Action : DROITE");
          digitalWrite(MOTEUR_DROITE, HIGH);
          break;
      }
    }
    
    mySwitch.resetAvailable();
  }
}