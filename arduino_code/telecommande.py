import serial
import time
from pynput import keyboard

# --- Configuration du port série ---
PORT = 'COM3' # TODO !! Modifier si nécessaire
BAUDRATE = 115200

try:
    print(f"Connexion à l'Arduino sur {PORT}...")
    arduino = serial.Serial(PORT, BAUDRATE, timeout=1)
    time.sleep(2) 
    
    print(" Connecté avec succès !")
    print("-------------------------------------------------")
    print("  CONTRÔLES ACTIFS : Appuyez sur W, A, S, D.")
    print("  Appuyez sur la touche 'Echap' pour quitter.")
    print("-------------------------------------------------")

except Exception as e:
    print(f" Erreur : Impossible de se connecter au port {PORT}.")
    print(f"Détails de l'erreur : {e}")
    exit()

# --- NOUVEAU : On crée un "set" pour mémoriser les touches enfoncées ---
touches_enfoncees = set()

def on_press(key):
    try:
        touche = key.char.lower()
        
        # Si la touche est W, A, S, ou D ET qu'elle n'est pas DÉJÀ enfoncée
        if touche in ['w', 'a', 's', 'd', 'z', 'q'] and touche not in touches_enfoncees:
            
            # 1. On l'ajoute à notre mémoire
            touches_enfoncees.add(touche)
            
            # 2. On envoie la trame UNE SEULE FOIS
            arduino.write(touche.encode())
            print(f"Trame envoyée : {touche.upper()}")
            
    except AttributeError:
        # Ignore les touches spéciales (Maj, Alt...)
        pass

def on_release(key):
    try:
        touche = key.char.lower()
        
        # Quand on relâche la touche, on la supprime de la mémoire
        if touche in touches_enfoncees:
            touches_enfoncees.remove(touche)
            print(f"Touche relâchée : {touche.upper()}")
            
    except AttributeError:
        pass

    if key == keyboard.Key.esc:
        print("Fermeture du programme...")
        return False

with keyboard.Listener(on_press=on_press, on_release=on_release) as listener:
    listener.join()

arduino.close()
print("Déconnecté de l'Arduino.")