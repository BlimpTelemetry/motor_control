// --- Configuration des broches pour Arduino MKR WAN 1310 ---
const int RX_PIN = 1;     // Broche de réception RF (PA1 sur STM32)

uint8_t rf_code = 0;      // Variable globale stockant la vitesse reçue

void setup() {
  // Initialisation du port série pour l'affichage de la vitesse
  Serial.begin(115200);
  
  // Configuration de la broche de réception
  pinMode(RX_PIN, INPUT);
  
  Serial.println("--- Récepteur Prêt ---");
  Serial.println("En attente du signal de vitesse...");
}

void loop() {
  // Si un message RF est décodé avec succès
  if (nec_remote_read()) {
    
    // Affichage de la vitesse reçue
    Serial.print("Vitesse du moteur reçue : ");
    Serial.print(rf_code);
    Serial.println(" tr/min (ou %)");
    
  }
}

/**
 * Fonction de lecture du protocole NEC
 * Remplit la variable globale 'rf_code' et renvoie true en cas de succès.
 */
bool nec_remote_read() {
  unsigned long start_time, count;
  
  // 1. Vérification de l'impulsion haute de 9ms
  start_time = micros();
  while (digitalRead(RX_PIN) == HIGH && (micros() - start_time < 12000));
  count = micros() - start_time;
  if (count > 11999 || count < 8500) return false;

  // 2. Vérification de l'espace bas de 4.5ms
  start_time = micros();
  while (digitalRead(RX_PIN) == LOW && (micros() - start_time < 5000));
  count = micros() - start_time;
  if (count > 4999 || count < 4000) return false;

  // 3. Lecture des 8 bits de données
  for (int i = 0; i < 8; i++) {
    // Impulsion haute (env. 560us)
    start_time = micros();
    while (digitalRead(RX_PIN) == HIGH && (micros() - start_time < 660));
    count = micros() - start_time;
    if (count > 660 || count < 480) return false;

    // Espace bas (détermine le bit 0 ou 1)
    start_time = micros();
    while (digitalRead(RX_PIN) == LOW && (micros() - start_time < 1900));
    count = micros() - start_time;
    if (count > 1899 || count < 400) return false;

    // Reconstruction de la valeur binaire de la vitesse
    if (count > 1000) {  
      rf_code |= (1 << (7 - i));  // Bit à 1
    } else {             
      rf_code &= ~(1 << (7 - i)); // Bit à 0
    }
  }
  
  return true;
}