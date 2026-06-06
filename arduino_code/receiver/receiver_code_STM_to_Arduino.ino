// --- Configuration pour Arduino MKR (1010 ou 1310) ---
const int RX_PIN = 1;     

// Exemple de broches pour tes moteurs (à adapter selon ton câblage)
const int MOTEUR_AVANT = 2;
const int MOTEUR_ARRIERE = 3;
const int MOTEUR_GAUCHE = 4;
const int MOTEUR_DROITE = 5;

uint8_t rf_code = 0;

void setup() {
  Serial.begin(115200);
  pinMode(RX_PIN, INPUT);
  
  // Initialisation des broches moteurs
  pinMode(MOTEUR_AVANT, OUTPUT);
  pinMode(MOTEUR_ARRIERE, OUTPUT);
  pinMode(MOTEUR_GAUCHE, OUTPUT);
  pinMode(MOTEUR_DROITE, OUTPUT);
  
  Serial.println("--- Récepteur Prêt ---");
}

void loop() {
  // Si un message RF est reçu et décodé
  if (nec_remote_read()) {
    
    // On éteint tous les moteurs par sécurité avant d'en allumer un nouveau
    digitalWrite(MOTEUR_AVANT, LOW);
    digitalWrite(MOTEUR_ARRIERE, LOW);
    digitalWrite(MOTEUR_GAUCHE, LOW);
    digitalWrite(MOTEUR_DROITE, LOW);

    // On active le bon moteur en fonction du code reçu
    switch (rf_code) {
      case 1:
        Serial.println("Action : AVANCER");
        digitalWrite(MOTEUR_AVANT, HIGH);
        break;
      case 2:
        Serial.println("Action : RECULER");
        digitalWrite(MOTEUR_ARRIERE, HIGH);
        break;
      case 3:
        Serial.println("Action : GAUCHE");
        digitalWrite(MOTEUR_GAUCHE, HIGH);
        break;
      case 4:
        Serial.println("Action : DROITE");
        digitalWrite(MOTEUR_DROITE, HIGH);
        break;
      default:
        Serial.println("Code inconnu reçu.");
        break;
    }
  }
}

/**
 * Fonction de lecture du protocole NEC
 */
bool nec_remote_read() {
  unsigned long start_time, count;
  
  start_time = micros();
  while (digitalRead(RX_PIN) == HIGH && (micros() - start_time < 12000));
  count = micros() - start_time;
  if (count > 11999 || count < 8500) return false;

  start_time = micros();
  while (digitalRead(RX_PIN) == LOW && (micros() - start_time < 5000));
  count = micros() - start_time;
  if (count > 4999 || count < 4000) return false;

  for (int i = 0; i < 8; i++) {
    start_time = micros();
    while (digitalRead(RX_PIN) == HIGH && (micros() - start_time < 660));
    count = micros() - start_time;
    if (count > 660 || count < 480) return false;

    start_time = micros();
    while (digitalRead(RX_PIN) == LOW && (micros() - start_time < 1900));
    count = micros() - start_time;
    if (count > 1899 || count < 400) return false;

    if (count > 1000) {  
      rf_code |= (1 << (7 - i));
    } else {             
      rf_code &= ~(1 << (7 - i));
    }
  }
  return true;
}