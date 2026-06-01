// --- Configuration des broches pour Arduino MKR WAN 1310 ---
const int TX_PIN = 1;     // Broche d'émission RF (PA0 sur STM32)

void setup() {
  // Initialisation du port série pour recevoir tes commandes de vitesse
  Serial.begin(115200);
  
  // Configuration de la broche d'émission
  pinMode(TX_PIN, OUTPUT);
  digitalWrite(TX_PIN, LOW); 
  
  Serial.println("--- Transmetteur Prêt ---");
  Serial.println("Entrez une vitesse entre 0 et 255 dans le moniteur de commande :");
}

void loop() {
  // Vérifie si une commande est reçue via le moniteur série
  if (Serial.available() > 0) {
    
    // Lit le nombre entier envoyé
    int vitesse saisie = Serial.parseInt();
    
    // Nettoie le buffer série des caractères restants (\n ou \r)
    while (Serial.available() > 0) {
      Serial.read();
    }
    
    // Le protocole NEC transmet 8 bits, donc la valeur doit être entre 0 et 255
    if (vitesse_saisie >= 0 && vitesse_saisie <= 255) {
      Serial.print("Commande validée. Envoi de la vitesse : ");
      Serial.println(vitesse_saisie);
      
      // Envoi du signal RF
      send_signal((uint8_t)vitesse_saisie);
      
      delay(200); // Court délai anti-engorgement
    } else {
      Serial.println("Erreur : La vitesse doit être un nombre entier entre 0 et 255.");
    }
  }
}

/**
 * Envoi du signal RF (protocole NEC)
 */
void send_signal(uint8_t number) {
  // 1. Impulsion de départ : 9ms HAUT, 4.5ms BAS
  digitalWrite(TX_PIN, HIGH);
  delay(9);
  digitalWrite(TX_PIN, LOW);
  delayMicroseconds(4500);
  
  // 2. Envoi des 8 bits de la vitesse
  for (int i = 0; i < 8; i++) {
    bool bit_is_high = (number >> (7 - i)) & 0x01;
    
    if (bit_is_high) {
      // Bit 1 : impulsion HAUT de 560us, BAS de 1680us
      digitalWrite(TX_PIN, HIGH);
      delayMicroseconds(560);
      digitalWrite(TX_PIN, LOW);
      delayMicroseconds(1680);
    } else {
      // Bit 0 : impulsion HAUT de 560us, BAS de 560us
      digitalWrite(TX_PIN, HIGH);
      delayMicroseconds(560);
      digitalWrite(TX_PIN, LOW);
      delayMicroseconds(560);
    }
  }
  
  // 3. Bit de fin
  digitalWrite(TX_PIN, HIGH);
  delayMicroseconds(560);
  digitalWrite(TX_PIN, LOW);
  delayMicroseconds(560);
}