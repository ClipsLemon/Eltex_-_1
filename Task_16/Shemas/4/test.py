import threading
import subprocess
import time
import signal
import sys

# Команда для запуска сервера и клиента
server_command = './server'
client_command = './client'

# Количество клиентских потоков
num_client_threads = 1000

# Таймаут для проверки состояния сервера
server_check_interval = 5

# Флаг для остановки потоков, если сервер не отвечает
stop_threads = False

def run_server():
    global server_process
    server_process = subprocess.Popen(server_command, shell=True)

def run_client():
    global stop_threads
    while not stop_threads:
        client_process = subprocess.Popen(client_command, shell=True)
        client_process.wait()
        if stop_threads:
            break
        time.sleep(1)

def check_server():
    global stop_threads
    while not stop_threads:
        try:
            server_process.poll()
            if server_process.returncode is not None:
                print("Server process has stopped.")
                stop_threads = True
                break
        except Exception as e:
            print(f"Error checking server: {e}")
            stop_threads = True
            break
        time.sleep(server_check_interval)

def signal_handler(sig, frame):
    global stop_threads
    stop_threads = True
    if server_process.poll() is None:
        server_process.terminate()
    sys.exit(0)

# Установка обработчика сигнала для SIGINT
signal.signal(signal.SIGINT, signal_handler)

# Запуск сервера
server_thread = threading.Thread(target=run_server)
server_thread.start()

# Ждем, пока сервер запустится
time.sleep(2)

# Создание и запуск клиентских потоков
client_threads = []
for i in range(num_client_threads):
    t = threading.Thread(target=run_client)
    t.start()
    client_threads.append(t)

# Создание и запуск потока проверки состояния сервера
server_check_thread = threading.Thread(target=check_server)
server_check_thread.start()

# Ожидание завершения всех потоков
for t in client_threads:
    t.join()

# Остановка сервера, если он еще работает
if server_process.poll() is None:
    server_process.terminate()

print("All threads stopped. Exiting program.")
