// --- Configuration Récepteur avec Servo.h ---
#include <Servo.h>

const int RX_PIN = 1;     

// Déclaration des moteurs
Servo esc1; // Gauche (G)
Servo esc2; // Milieu (M)
Servo esc3; // Droite (D)

const int pinESC1 = 2; 
const int pinESC2 = 3; 
const int pinESC3 = 4; 

// Variables pour stocker les vitesses actuelles
int consignePWM1 = 1000;
int consignePWM2 = 1000;
int consignePWM3 = 1000;

// Variables générales
unsigned long timerAffichage = 0;
uint16_t rf_code = 0; // Trame de 16 bits

void setup() {
  Serial.begin(115200);
  pinMode(RX_PIN, INPUT);
  
  // Attachement des broches au timer matériel
  esc1.attach(pinESC1);
  esc2.attach(pinESC2);
  esc3.attach(pinESC3);

  Serial.println("--- Séquence d'armement matérielle (3 Moteurs) ---");

  // On envoie le signal minimum pour armer les ESC
  esc1.writeMicroseconds(1000);
  esc2.writeMicroseconds(1000);
  esc3.writeMicroseconds(1000);
  
  // On laisse le Timer générer le signal tout seul pendant 3 secondes
  delay(3000);
  
  Serial.println("--- ESC armés et Récepteur Prêt ! ---");
}

void loop() {
  
  // ---------------------------------------------------------
  // LECTURE DU SIGNAL RF
  // ---------------------------------------------------------
  if (digitalRead(RX_PIN) == HIGH) {
    
    // Si on a reçu et décodé une trame valide
    if (nec_remote_read()) {
      
      // Décodage de la trame 16 bits : ID (4 bits) et Vitesse (12 bits)
      uint8_t idMoteur = (rf_code >> 12) & 0x0F;
      uint16_t vitesseRecue = rf_code & 0x0FFF;

      // Affectation et mise à jour des ESC matériels
      if (idMoteur == 1) { 
        consignePWM1 = vitesseRecue; 
        esc1.writeMicroseconds(consignePWM1);
        Serial.println("Action : Moteur GAUCHE mis à jour"); 
      }
      else if (idMoteur == 2) { 
        consignePWM2 = vitesseRecue; 
        esc2.writeMicroseconds(consignePWM2);
        Serial.println("Action : Moteur MILIEU mis à jour"); 
      }
      else if (idMoteur == 3) { 
        consignePWM3 = vitesseRecue; 
        esc3.writeMicroseconds(consignePWM3);
        Serial.println("Action : Moteur DROITE mis à jour"); 
      }
      else if (idMoteur == 4) { 
        consignePWM1 = vitesseRecue; 
        consignePWM2 = vitesseRecue; 
        consignePWM3 = vitesseRecue; 
        esc1.writeMicroseconds(consignePWM1);
        esc2.writeMicroseconds(consignePWM2);
        esc3.writeMicroseconds(consignePWM3);
        Serial.println("Action : TOUS les moteurs mis à jour"); 
      }
    }
  }

  // ---------------------------------------------------------
  // AFFICHAGE DE DÉBOGAGE
  // ---------------------------------------------------------
  if (millis() - timerAffichage > 500) {
    Serial.print("Vitesses ESC -> G: "); Serial.print(consignePWM1);
    Serial.print(" | M: "); Serial.print(consignePWM2);
    Serial.print(" | D: "); Serial.println(consignePWM3);
    timerAffichage = millis();
  }
}

/**
 * Fonction de lecture du protocole RF 16 BITS
 * Cette fonction a été traduite d'une utilisation ultérieur sur STM32
 */
bool nec_remote_read() {
  unsigned long start_time, count;
  rf_code = 0; 
  
  start_time = micros();
  while (digitalRead(RX_PIN) == HIGH && (micros() - start_time < 12000));
  count = micros() - start_time;
  if (count > 11999 || count < 8500) return false;

  start_time = micros();
  while (digitalRead(RX_PIN) == LOW && (micros() - start_time < 5000));
  count = micros() - start_time;
  if (count > 4999 || count < 4000) return false;

  for (int i = 0; i < 16; i++) {
    start_time = micros();
    while (digitalRead(RX_PIN) == HIGH && (micros() - start_time < 660));
    count = micros() - start_time;
    if (count > 660 || count < 480) return false;

    start_time = micros();
    while (digitalRead(RX_PIN) == LOW && (micros() - start_time < 1900));
    count = micros() - start_time;
    if (count > 1899 || count < 400) return false;

    if (count > 1000) {  
      rf_code |= (1 << (15 - i));
    } else {             
      rf_code &= ~(1 << (15 - i));
    }
  }
  return true;
}