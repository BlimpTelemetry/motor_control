# Motor Control

Ce dépôt contient les firmwares et codes de test liés au contrôle moteur du projet **BlimpTelemetry**.

Il regroupe les programmes utilisés pour le prototypage, la génération de commandes moteur, la transmission/réception de commandes et les firmwares ESC utilisés sur le dirigeable.

---

## Structure du dépôt

```text
motor_control/
├── arduino_code/           # Prototypes Arduino et transmission/réception de commandes
├── esc_driver_firmware/    # Firmware ESC principal
├── esc_driver_no_foc/      # Variante de driver sans FOC
└── README.md
```

---

## Contenu

### `arduino_code/`

Ce dossier contient les codes Arduino utilisés pour le prototypage et les tests liés au contrôle moteur du dirigeable.

Il inclut notamment des programmes pour :

* générer des signaux de commande PWM pour les ESC ;
* tester différentes consignes moteur ;
* transmettre et recevoir des commandes ;
* valider certains comportements avant intégration sur le système embarqué principal.

Ces codes servent principalement aux essais rapides et à la validation fonctionnelle.

---

### `esc_driver_firmware/`

Ce dossier contient le firmware principal destiné au contrôle des ESC.

Il inclut la version avec **FOC** (*Field-Oriented Control*), qui a été implémentée et utilisée sur le dirigeable.

---

### `esc_driver_no_foc/`

Ce dossier contient une variante du driver sans **FOC**.

Cette version peut être utilisée pour des tests plus simples, pour comparer le comportement avec la version FOC, ou pour diagnostiquer certains problèmes liés au contrôle moteur.

---

## Objectifs du dépôt

Ce dépôt sert à :

* regrouper les codes liés au contrôle moteur ;
* tester les signaux de commande envoyés aux ESC ;
* développer et modifier les firmwares ESC ;
* comparer différentes stratégies de contrôle moteur ;
* documenter les paramètres utilisés pour les moteurs du dirigeable.

---

## Compatibilité moteur

Les drivers sont configurés pour les moteurs :

```text
A2212/13T
```

Les paramètres moteur doivent être vérifiés avant les essais, notamment :

* tension d’alimentation ;
* courant maximal ;
* nombre de pôles ;
* plage PWM ;
* paramètres de démarrage ;
* limites de vitesse.

---

## Prérequis

Selon la partie du dépôt utilisée, les outils suivants peuvent être nécessaires :

* Arduino IDE pour les fichiers du dossier `arduino_code/` ;
* STM32CubeIDE ;
* STM32CubeMX ;
* MC Workbench ;
* un compilateur C/C++ adapté à la cible ;
* `make` ;
* `cmake`.

---

## Compilation

### Avec Make

Depuis le dossier contenant le Makefile :

```bash
make
```

### Avec CMake

```bash
mkdir build
cd build
cmake ..
make
```

### Code Arduino

Pour utiliser un programme Arduino :

1. ouvrir le fichier `.ino` dans l’IDE Arduino ;
2. sélectionner la carte cible ;
3. sélectionner le port série ;
4. téléverser le programme ;
5. utiliser le moniteur série si le programme le prévoit.

---

## Tests moteur

Lors des premiers essais :

* retirer les hélices ;
* vérifier la polarité de l’alimentation ;
* utiliser une alimentation limitée en courant ;
* vérifier que la masse de la carte de commande est commune avec celle de l’ESC ;
* commencer avec des consignes faibles ;
* vérifier le signal PWM à l’oscilloscope si possible.

Les moteurs brushless peuvent démarrer brusquement si la commande ou la configuration de l’ESC est incorrecte.

---

## Intégration sur le dirigeable

La version avec **FOC** a été utilisée sur le dirigeable.

Les codes Arduino servent principalement aux essais, à la transmission/réception de commandes et à la validation de la chaîne de commande avant ou pendant l’intégration.

---

## Documentation des modifications

Lorsqu’un paramètre moteur ou ESC est modifié, il est recommandé de documenter :

* le moteur utilisé ;
* la tension d’alimentation ;
* la limite de courant ;
* la plage de commande PWM ;
* les paramètres de démarrage ;
* le comportement observé pendant les tests ;
* les problèmes rencontrés.

---

## Projet

Ce dépôt fait partie du projet **BlimpTelemetry**.
