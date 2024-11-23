import serial
import time

# Configura la porta seriale
arduino = serial.Serial(port='/dev/ttyUSB0', baudrate=9600, timeout=1)  # Cambia il port se sei su Windows ('COM3', 'COM4', ecc.)

# Attendi che la connessione sia stabile
time.sleep(2)

# Legge i dati dalla seriale
while True:
    try:
        if arduino.in_waiting > 0:  # Se ci sono dati disponibili
            data = arduino.readline().decode('utf-8').strip()  # Legge la linea
            print(f"Dati ricevuti: {data}")
    except KeyboardInterrupt:
        print("Connessione interrotta dall'utente.")
        break

arduino.close()
