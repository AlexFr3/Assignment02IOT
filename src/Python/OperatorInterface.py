import serial
import time
import tkinter as tk
import threading

# Configura la porta seriale (modifica con la tua porta)
arduino = serial.Serial('COM3', 9600, timeout=1)  # Cambia 'COM3' con la tua porta seriale
time.sleep(2)  # Attendi che Arduino sia pronto

# Event per fermare il thread
stop_event = threading.Event()

# Funzione per inviare un messaggio ad Arduino
def send_msg(message):
    arduino.write(f"{message}\n".encode())  # Aggiungi newline per segnare la fine del messaggio

# Funzione per leggere un messaggio dalla seriale
def read_msg():
    try:
        if arduino.in_waiting > 0:  # Se ci sono dati disponibili
            msg = arduino.readline().decode('utf-8').strip()  # Leggi la linea e rimuovi newline
            return msg
    except Exception as e:
        print(f"Errore nella lettura: {e}")
    return None

# Funzione per aggiornare continuamente la percentuale di riempimento
def update_fill_percentage():
    while not stop_event.is_set():
        fill_msg = read_msg()
        if fill_msg:
            # Usa root.after() per aggiornare l'etichetta nel thread principale
            root.after(0, update_label, fill_msg)

# Funzione per aggiornare l'etichetta con il messaggio ricevuto
def update_label(fill_msg):
    try:
        # Aggiorna il testo dell'etichetta con il messaggio ricevuto
        fill_label.config(text=f"{fill_msg}")
    except tk.TclError:
        pass  # Evita errori se la finestra è stata chiusa

# Funzione per inviare un comando specifico
def send_command(command):
    send_msg(command)

# Funzione per chiudere in modo sicuro l'applicazione
def on_close():
    stop_event.set()  # Imposta l'evento per fermare il thread
    thread.join()  # Attendi che il thread termini
    arduino.close()  # Chiudi la connessione seriale
    root.quit()  # Termina il loop principale

# Configurazione dell'interfaccia grafica
root = tk.Tk()
root.title("Check Arduino")
root.geometry("500x450")
root.config(bg="#f4f4f4")  # Colore di sfondo

fill_label = tk.Label(root, text="Filling: --%", font=("Arial", 14), bg="#f4f4f4", fg="#000000")
fill_label.pack(pady=20)

result_label = tk.Label(root, text="Welcome to smart container waste control", font=("Arial", 14), bg="#f4f4f4", fg="#000000")
result_label.pack(pady=10)

btn_empty_container = tk.Button(root, text="Emptying the Container", font=("Arial", 12), bg="#007BFF", fg="white",
                                 command=lambda: send_command("EMPTY_CONTAINER"))
btn_empty_container.pack(pady=10)

btn_restore = tk.Button(root, text="Restore", font=("Arial", 12), bg="#28A745", fg="white",
                         command=lambda: send_command("RESTORE_PRESS"))
btn_restore.pack(pady=10)

# Associa l'evento di chiusura alla funzione `on_close`
root.protocol("WM_DELETE_WINDOW", on_close)

# Avvia il thread per aggiornare il riempimento
thread = threading.Thread(target=update_fill_percentage, daemon=True)
thread.start()

# Avvia il loop principale della GUI
root.mainloop()

# Dopo la chiusura della finestra, il thread è stato fermato e la connessione seriale è stata chiusa
