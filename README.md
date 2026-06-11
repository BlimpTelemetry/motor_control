# Motor Control

Ce dépôt regroupe les firmwares et outils logiciels dédiés au contrôle moteur du projet **BlimpTelemetry**. Il contient plusieurs variantes de drivers et de firmwares embarqués destinés à piloter les moteurs du dirigeable avec précision, efficacité et fiabilité.

## Vue d’ensemble

Le projet s’articule autour de trois composants principaux :

- **arduino_code/** : code Arduino pour le prototypage et les essais rapides
- **esc_driver_firmware/** : firmware principal pour le contrôle des ESC
- **esc_driver_no_foc/** : variante de driver sans **FOC** (Field-Oriented Control)

La version **FOC** a déjà été implémentée et utilisée sur le dirigeable.

## Objectifs du projet

Ce dépôt vise à :

- piloter les moteurs du dirigeable de manière stable et réactive
- fournir des firmwares embarqués adaptés à des contraintes temps réel
- proposer des implémentations optimisées en C avec un minimum d’assemblage pour les sections critiques
- faciliter les tests, le calibrage et l’évolution des drivers moteurs

## Caractéristiques techniques

- **Langage principal** : C
- **Optimisations bas niveau** : Assembly pour les parties critiques
- **Outils de build** : Makefile, CMake
- **Cible** : systèmes embarqués et contrôle moteur

## Compatibilité moteur

Les drivers sont calibrés pour les moteurs **A2212/13T**.

## Structure du dépôt

```text
motor_control/
├── arduino_code/           # Code Arduino et prototypes
├── esc_driver_firmware/    # Firmware ESC principal
├── esc_driver_no_foc/      # Variante de driver sans FOC
└── README.md
```

## Détails des modules

### `arduino_code/`
Code destiné aux cartes Arduino pour les tests, la validation fonctionnelle et le développement rapide.

### `esc_driver_firmware/`
Firmware principal du contrôle moteur et des ESC, avec une approche orientée performance et fiabilité.

### `esc_driver_no_foc/`
Version simplifiée du driver, sans contrôle FOC, utile pour des configurations plus directes ou des besoins spécifiques.

## Démarrage rapide

### Prérequis

- un compilateur C embarqué adapté à la cible
- `make`
- éventuellement `cmake`
- l’environnement Arduino pour les éléments du dossier `arduino_code/`

### Compilation

#### Avec Make
```bash
make
```

#### Avec CMake
```bash
mkdir build
cd build
cmake ..
make
```

#### Partie Arduino
Ouvrir les fichiers du dossier `arduino_code/` dans l’IDE Arduino et téléverser sur la carte cible.

## Intégration sur le dirigeable

La variante **FOC** a été intégrée avec succès sur le dirigeable, ce qui confirme l’usage du dépôt dans un contexte réel de motorisation embarquée.

## Bonnes pratiques de contribution

Toute contribution est la bienvenue. Merci de :

- conserver un code lisible et commenté
- vérifier les changements sur le matériel cible
- documenter les paramètres de calibration
- expliciter toute modification liée au contrôle moteur ou aux performances

## Licence

Ajouter ici les informations de licence si elles sont disponibles.

## Projet

Développé dans le cadre de **BlimpTelemetry**.
