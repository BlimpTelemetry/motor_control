const int pinESC = 9;  // Connecté au connecteur J3

// La période totale d'un cycle à 490 Hz est d'environ 2040 microsecondes
const unsigned long PERIODE_US = 2040;

unsigned long timerAffichage = 0;

// Vitesse du moteur par défaut
int valeurPot = 500; 

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10); 
  
  pinMode(pinESC, OUTPUT);

  Serial.println("Séquence d'armement stricte à 490Hz...");

  // --- Séquence d'armement manuel ---
  // On génère manuellement le signal pendant 3 secondes
  unsigned long debutArmement = millis();
  while (millis() - debutArmement < 3000) {
    digitalWrite(pinESC, HIGH);
    delayMicroseconds(1000); // 1000 µs = État HAUT (Inférieur à 1060 µs pour armer)
    digitalWrite(pinESC, LOW);
    delayMicroseconds(PERIODE_US - 1000); // 1040 µs = État BAS pour finir le cycle
  }
  
  Serial.println("ESC armé. Prêt à tourner !");
  Serial.println("Entrez une valeur entre 0 et 1023 dans le moniteur série.");
}

void loop() {
  
  if (Serial.available() > 0) {
    int nouvelleValeur = Serial.parseInt(); // Lit le nombre envoyé
    
    // Flush le buffer d'entrée
    while(Serial.available() > 0) {
      Serial.read();
    }

    // On sécurise pour s'assurer que la valeur est bien dans la plage prévue
    if (nouvelleValeur >= 0 && nouvelleValeur <= 1023) {
      valeurPot = nouvelleValeur;
      Serial.print(">>> Nouvelle consigne reçue : ");
      Serial.println(valeurPot);
    }
  }

  // Plage de la datasheet : 1060 (min) à 1860 (max)
  unsigned long consignePWM = map(valeurPot, 0, 1023, 1060, 1860);

  // --- GÉNÉRATION DU SIGNAL PWM ---
  digitalWrite(pinESC, HIGH);
  delayMicroseconds(consignePWM);           // Maintient le signal HAUT
  digitalWrite(pinESC, LOW);
  delayMicroseconds(PERIODE_US - consignePWM); // Maintient le signal BAS pour finir le cycle

  // Affichage (Uniquement toutes les 500ms pour ne pas ralentir le signal PWM)
  if (millis() - timerAffichage > 500) {
    Serial.print("Valeur actuelle: ");
    Serial.print(valeurPot);
    Serial.print(" | Signal (µs): ");
    Serial.println(consignePWM);
    timerAffichage = millis();
  }
}