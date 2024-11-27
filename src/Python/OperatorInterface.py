import serial
import time
import tkinter as tk
import threading

#'COM 3' needs to be changed to the selected port
arduino = serial.Serial('COM4', 9600, timeout=1)  
time.sleep(2)  # Wait for Arduino to be ready

# Event to stop the thread
stop_event = threading.Event()

def send_msg(message):
    arduino.write(f"{message}\n".encode())  

def read_msg():
    try:
        if arduino.in_waiting > 0:  
            msg = arduino.readline().decode('utf-8').strip() 
            return msg
    except Exception as e:
        print(f"Errore nella lettura: {e}")
    return None

def update_fields():
    while not stop_event.is_set():
        message = read_msg()
        if message:
            if message.startswith("T"):
                root.after(0, update_temperature, message)
            elif message.startswith("F"):
                root.after(0, update_filling, message)

def update_filling(fill_msg):
    try:
        fill_label.config(text=f"{fill_msg}")
    except tk.TclError:
        pass  

def update_temperature(temperature_msg):
    try:
        temperature_label.config(text=f"{temperature_msg}")
    except tk.TclError:
        pass  

def send_command(command):
    send_msg(command)

def on_close():
    stop_event.set()  
    arduino.close()  
    root.destroy()  

root = tk.Tk()
root.title("Check Arduino")
root.geometry("500x450")
root.config(bg="#f4f4f4")  


temperature_label = tk.Label(root, text="Temperature: ", font=("Arial", 14), bg="#f4f4f4", fg="#000000")
temperature_label.pack(pady=20)

fill_label = tk.Label(root, text="Filling: --%", font=("Arial", 14), bg="#f4f4f4", fg="#000000")
fill_label.pack(pady=20)

result_label = tk.Label(root, text="Welcome to smart container waste control", font=("Arial", 14), bg="#f4f4f4", fg="#000000")
result_label.pack(pady=10)

btn_empty_container = tk.Button(root, text="Empty the container", font=("Arial", 12), bg="#007BFF", fg="black",
                                 command=lambda: send_command("EMPTY_CONTAINER"))
btn_empty_container.pack(pady=10)

btn_restore = tk.Button(root, text="Restore", font=("Arial", 12), bg="#28A745", fg="black",
                         command=lambda: send_command("RESTORE_PRESS"))
btn_restore.pack(pady=10)

root.protocol("WM_DELETE_WINDOW", on_close)

thread = threading.Thread(target=update_fields, daemon=True)
thread.start()

root.mainloop()

