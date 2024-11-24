import serial
import time
import tkinter as tk
import threading

# Configura la porta seriale (modifica con la tua porta)
arduino = serial.Serial('COM3', 9600, timeout=1)  # Cambia 'COM3' con la tua porta seriale
time.sleep(2)  # Attendi che Arduino sia pronto

# Funzione per inviare un messaggio ad Arduino
def send_msg(message):
    arduino.write(f"{message}\n".encode())  # Aggiungi newline per segnare la fine del messaggio

# Funzione per leggere un messaggio dalla seriale
def read_msg():
    if arduino.in_waiting > 0:  # Se ci sono dati disponibili
        msg = arduino.readline().decode('utf-8').strip()  # Leggi la linea e rimuovi il newline
        return msg
    return None

# Funzione per aggiornare continuamente la percentuale di riempimento
def update_fill_percentage():
    while True:
        time.sleep(1)  # Pausa di 1 secondo
        fill_msg = read_msg()
        if fill_msg:
            try:
                # Aggiorna il testo dell'etichetta con il messaggio ricevuto
                fill_label.config(text=f"Riempimento: {fill_msg}")
            except tk.TclError:
                # Esci dal ciclo se la finestra viene chiusa
                break

# Funzione per inviare un comando specifico
def send_command(command):
    send_msg(command)

# Configurazione dell'interfaccia grafica
root = tk.Tk()
root.title("Controllo Arduino")
root.geometry("500x450")
root.config(bg="#f4f4f4")  # Colore di sfondo

# Etichetta per visualizzare il riempimento
fill_label = tk.Label(root, text="Riempimento: --%", font=("Arial", 14), bg="#f4f4f4", fg="#000000")
fill_label.pack(pady=20)

# Etichetta per i risultati
result_label = tk.Label(root, text="Benvenuto nel controllo di Arduino", font=("Arial", 14), bg="#f4f4f4", fg="#000000")
result_label.pack(pady=10)

# Bottoni per inviare comandi
btn_empty_container = tk.Button(root, text="Emptying the Container", font=("Arial", 12), bg="#007BFF", fg="white",
                                 command=lambda: send_command("EMPTY_CONTAINER"))
btn_empty_container.pack(pady=10)

btn_restore = tk.Button(root, text="Restore", font=("Arial", 12), bg="#28A745", fg="white",
                         command=lambda: send_command("RESTORE_PRESS"))
btn_restore.pack(pady=10)

# Avvia il thread per aggiornare il riempimento
thread = threading.Thread(target=update_fill_percentage, daemon=True)
thread.start()

# Avvia il loop principale della GUI
root.mainloop()

# Chiudi la connessione seriale quando la finestra viene chiusa
arduino.close()
