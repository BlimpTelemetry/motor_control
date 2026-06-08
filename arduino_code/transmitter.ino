const int TX_PIN = 1; // Broche reliée au module émetteur 433MHz

void setup() {
  Serial.begin(115200);
  pinMode(TX_PIN, OUTPUT);
  digitalWrite(TX_PIN, LOW);
  
  Serial.println("--- Transmetteur Prêt ---");
  Serial.println("Commandes : Gxxxx (Gauche), Mxxxx (Milieu), Dxxxx (Droite), Txxxx (Tous)");
  Serial.println("Exemple : D1060 (Moteur Droite a 1060µs)");
}

void loop() {
  // Si on reçoit un message dans le moniteur série
  if (Serial.available() > 0) {
    String msg = Serial.readStringUntil('\n');
    msg.trim(); // Enlève les espaces ou retours à la ligne
    
    if (msg.length() > 1) {
      char moteur = msg.charAt(0);                // Récupère la lettre (G, M, D, T)
      int vitesse = msg.substring(1).toInt();     // Récupère la vitesse (ex: 1060)
      
      uint16_t id = 0;
      if (moteur == 'G' || moteur == 'g') id = 1;
      else if (moteur == 'M' || moteur == 'm') id = 2;
      else if (moteur == 'D' || moteur == 'd') id = 3;
      else if (moteur == 'T' || moteur == 't') id = 4;
      
      // Sécurité sur la saisie
      if (id > 0 && vitesse >= 800 && vitesse <= 1860) {
        // On combine l'ID (décalé de 12 bits) et la vitesse dans un seul nombre de 16 bits
        uint16_t trameRF = (id << 12) | vitesse;
        
        Serial.print("Envoi en cours -> Moteur ID: "); Serial.print(id);
        Serial.print(" | Vitesse: "); Serial.print(vitesse);
        Serial.print(" | Trame brute 16-bits: "); Serial.println(trameRF);
        
        envoyerTrameRF(trameRF);
      } else {
        Serial.println("Erreur: Vitesse hors limite (1000-2000) ou Moteur inconnu.");
      }
    }
  }
}

/**
 * Fonction pour générer le signal RF manuellement (Compatible avec le Receiver)
 */
void envoyerTrameRF(uint16_t data) {
  // 1. Séquence de START (Calibrée pour tromper le nec_read)
  digitalWrite(TX_PIN, HIGH);
  delayMicroseconds(9000); 
  digitalWrite(TX_PIN, LOW);
  delayMicroseconds(4500);

  // 2. Envoi des 16 bits
  for (int i = 0; i < 16; i++) {
    bool bitActuel = (data >> (15 - i)) & 1; // Lit le bit de gauche à droite
    
    digitalWrite(TX_PIN, HIGH);
    delayMicroseconds(560);
    
    digitalWrite(TX_PIN, LOW);
    if (bitActuel) {
      delayMicroseconds(1600); // Temps long pour un '1'
    } else {
      delayMicroseconds(600);  // Temps court pour un '0'
    }
  }

  // 3. Séquence de STOP
  digitalWrite(TX_PIN, HIGH);
  delayMicroseconds(560);
  digitalWrite(TX_PIN, LOW);
}