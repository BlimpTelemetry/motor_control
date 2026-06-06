#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

// --- CONFIGURATION DES BROCHES ---
const int RX_PIN = 6;      // Broche Data
const int PIN_ESC_1 = 2;   // Moteur 1
const int PIN_ESC_2 = 3;   // Moteur 2
const int PIN_ESC_3 = 4;   // Moteur 3

// Vitesse utilisée
const int MOTOR_ON = 800;
const int MOTOR_TURN = 300;
const int MOTOR_LOW = 0;

// --- PARAMÈTRES DU SIGNAL ESC ---
const unsigned long PERIODE_US = 2040; // Cycle pour 490 Hz
unsigned long timerAffichage = 0;

// Vitesses actuelles
int vitM1 = 0; 
int vitM2 = 0; 
int vitM3 = 0; 

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(10); 
  
  pinMode(PIN_ESC_1, OUTPUT);
  pinMode(PIN_ESC_2, OUTPUT);
  pinMode(PIN_ESC_3, OUTPUT);
  
  // Initialisation du récepteur RF
  mySwitch.enableReceive(digitalPinToInterrupt(RX_PIN));

  Serial.println("--- Séquence d'armement stricte (3 moteurs) ---");

  // --- Séquence d'armement manuel pour les 3 ESC en parallèle ---
  unsigned long debutArmement = millis();
  while (millis() - debutArmement < 3000) {
    unsigned long startArmCycle = micros();
    
    digitalWrite(PIN_ESC_1, HIGH);
    digitalWrite(PIN_ESC_2, HIGH);
    digitalWrite(PIN_ESC_3, HIGH);
    
    while(micros() - startArmCycle < 1000); // 1000 µs = État HAUT pour armer
    
    digitalWrite(PIN_ESC_1, LOW);
    digitalWrite(PIN_ESC_2, LOW);
    digitalWrite(PIN_ESC_3, LOW);
    
    while(micros() - startArmCycle < PERIODE_US); // Fin du cycle de 2040 µs
  }
  
  Serial.println("ESC armés. Prêts à tourner !");
  Serial.println("Entrez une valeur (0-1023) dans le port Série OU utilisez la télécommande RF.");
}

void loop() {
  // ---------------------------------------------------------
  // 1. ÉCOUTE DE LA COMMANDE SÉRIE (Pour tests manuels)
  // ---------------------------------------------------------
  if (Serial.available() > 0) {
    int nouvelleValeur = Serial.parseInt(); 
    while(Serial.available() > 0) Serial.read(); // Vide le buffer

    if (nouvelleValeur >= 0 && nouvelleValeur <= 1023) {
      vitM1 = nouvelleValeur;
      vitM2 = nouvelleValeur;
      vitM3 = nouvelleValeur;
      Serial.print(">>> Commande SÉRIE reçue. Tous les moteurs à : ");
      Serial.println(nouvelleValeur);
    }
  }

  // ---------------------------------------------------------
  // 2. ÉCOUTE DE LA COMMANDE RF (La télécommande Python)
  // ---------------------------------------------------------
  if (mySwitch.available()) {
    long value = mySwitch.getReceivedValue();
    
	// Il faudra sûrement ajouter encore une touche pour descendre
    if (value != 0) {
      switch (value) {
        case 1001: // Touche W (Avancer) TODO c'est un exemple
          vitM1 = MOTOR_ON; vitM2 = MOTOR_ON; vitM3 = MOTOR_ON;
          Serial.println("RF: Touche W -> AVANCER");
          break;
        case 1002: // Touche S (Stop) TODO c'est un exemple
          vitM1 = MOTOR_LOW; vitM2 = MOTOR_LOW; vitM3 = MOTOR_LOW;
          Serial.println("RF: Touche S -> STOP");
          break;
        case 1003: // Touche A (Tourner à gauche) TODO c'est un exemple
          vitM1 = MOTOR_TURN; vitM2 = MOTOR_ON; vitM3 = MOTOR_ON;
          Serial.println("RF: Touche A -> GAUCHE");
          break;
        case 1004: // Touche D (Tourner à droite) TODO c'est un exemple
          vitM1 = MOTOR_ON; vitM2 = MOTOR_TURN; vitM3 = MOTOR_TURN;
          Serial.println("RF: Touche D -> DROITE");
          break;
      }
    }
    mySwitch.resetAvailable(); // Réarme le récepteur RF
  }

  // ---------------------------------------------------------
  // 3. GÉNÉRATION DU SIGNAL PWM PARALLÈLE
  // ---------------------------------------------------------
  // Conversion des consignes (0-1023) en microsecondes (1060-1860)
  unsigned long consigne1 = map(vitM1, 0, 1023, 1060, 1860);
  unsigned long consigne2 = map(vitM2, 0, 1023, 1060, 1860);
  unsigned long consigne3 = map(vitM3, 0, 1023, 1060, 1860);

  unsigned long startCycle = micros();
  
  // On allume tout le monde au début du cycle
  digitalWrite(PIN_ESC_1, HIGH);
  digitalWrite(PIN_ESC_2, HIGH);
  digitalWrite(PIN_ESC_3, HIGH);

  bool m1_on = true, m2_on = true, m3_on = true;

  // On écoute le chronomètre tourner jusqu'à la fin de la période (2040µs)
  while(micros() - startCycle < PERIODE_US) {
    unsigned long current = micros() - startCycle;
    
    // Dès que le temps attribué à un moteur est écoulé, on éteint sa broche
    if (m1_on && current >= consigne1) { digitalWrite(PIN_ESC_1, LOW); m1_on = false; }
    if (m2_on && current >= consigne2) { digitalWrite(PIN_ESC_2, LOW); m2_on = false; }
    if (m3_on && current >= consigne3) { digitalWrite(PIN_ESC_3, LOW); m3_on = false; }
  }
  
  // On s'assure que tout est éteint à la sortie de la boucle
  digitalWrite(PIN_ESC_1, LOW);
  digitalWrite(PIN_ESC_2, LOW);
  digitalWrite(PIN_ESC_3, LOW);

  // ---------------------------------------------------------
  // 4. AFFICHAGE (Non bloquant)
  // ---------------------------------------------------------
  if (millis() - timerAffichage > 500) {
    Serial.print("Signaux µs -> M1:");
    Serial.print(consigne1);
    Serial.print(" | M2:");
    Serial.print(consigne2);
    Serial.print(" | M3:");
    Serial.println(consigne3);
    timerAffichage = millis();
  }
}