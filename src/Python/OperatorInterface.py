import serial
import time

# Configura la porta seriale (modifica con la tua porta)
arduino = serial.Serial('COM3', 9600, timeout=1)  # Cambia 'COM3' con la tua porta seriale

# Attendere che Arduino sia pronto
time.sleep(2)

# Funzione per inviare un messaggio ad Arduino
def send_msg(message):
    arduino.write(f"{message}\n".encode())  # Aggiungi newline per segnare la fine del messaggio

# Funzione per leggere un messaggio dalla seriale
def read_msg():
    if arduino.in_waiting > 0:  # Se ci sono dati disponibili
        msg = arduino.readline().decode('utf-8').strip()  # Leggi la linea e rimuovi il newline
        return msg
    return None

# Esegui il ciclo principale
try:
    while True:
        # Invia un messaggio di esempio
        send_msg("LED_ON")
        print("Messaggio inviato: LED_ON")

        # Leggi la risposta da Arduino
        response = read_msg()
        if response:
            print(f"Risposta da Arduino: {response}")
        
        time.sleep(1)  # Pausa di 1 secondo
except KeyboardInterrupt:
    print("Programma interrotto dall'utente.")

finally:
    # Chiudi la porta seriale quando il programma termina
    arduino.close()
