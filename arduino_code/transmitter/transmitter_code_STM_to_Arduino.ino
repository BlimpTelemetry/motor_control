// --- Configuration pour Arduino MKR (1010 ou 1310) ---
const int TX_PIN = 1;     

void setup() {
  Serial.begin(115200);
  pinMode(TX_PIN, OUTPUT);
  digitalWrite(TX_PIN, LOW); 
  
  Serial.println("--- Transmetteur Clavier Prêt ---");
  Serial.println("Tapez W, A, S, ou D puis Entrée pour piloter les moteurs.");
}

void loop() {
  // On vérifie si un caractère a été envoyé via le clavier
  if (Serial.available() > 0) {
    
    // On lit le caractère
    char touche = Serial.read();
    
    // On convertit la majuscule en minuscule pour éviter les erreurs
    touche = toLowerCase(touche);
    
    // On associe la touche à un code RF et on l'envoie
    switch (touche) {
      case 'w': // Haut / Avancer
        Serial.println("Envoi : Moteur AVANT (W)");
        send_signal(1);
        break;
        
      case 's': // Bas / Reculer
        Serial.println("Envoi : Moteur ARRIÈRE (S)");
        send_signal(2);
        break;
        
      case 'a': // Gauche
        Serial.println("Envoi : Moteur GAUCHE (A)");
        send_signal(3);
        break;
        
      case 'd': // Droite
        Serial.println("Envoi : Moteur DROITE (D)");
        send_signal(4);
        break;
        
      case '\n': 
      case '\r':
        break;
        
      default:
        Serial.println("Touche non reconnue. Utilisez W, A, S, ou D.");
        break;
    }
    
    delay(100); // Petit délai pour éviter de spammer le signal
  }
}

/**
 * Envoi du signal RF (protocole NEC)
 */
void send_signal(uint8_t number) {
  digitalWrite(TX_PIN, HIGH);
  delay(9);
  digitalWrite(TX_PIN, LOW);
  delayMicroseconds(4500);
  
  for (int i = 0; i < 8; i++) {
    bool bit_is_high = (number >> (7 - i)) & 0x01;
    if (bit_is_high) {
      digitalWrite(TX_PIN, HIGH); delayMicroseconds(560);
      digitalWrite(TX_PIN, LOW); delayMicroseconds(1680);
    } else {
      digitalWrite(TX_PIN, HIGH); delayMicroseconds(560);
      digitalWrite(TX_PIN, LOW); delayMicroseconds(560);
    }
  }
  
  digitalWrite(TX_PIN, HIGH); delayMicroseconds(560);
  digitalWrite(TX_PIN, LOW); delayMicroseconds(560);
}