import bluetooth

# Адрес Bluetooth-устройства HC-06
device_address = '98:DA:60:04:56:19'

try:
    # Создаем сокет для Bluetooth
    sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)

    # Подключаемся к устройству
    sock.connect((device_address, 1))  # Номер порта может отличаться

    # Теперь вы можете отправлять и принимать данные через 'sock'

    # Пример отправки данных
    sock.send('1')

    # Пример приема данных
    received_data = sock.recv(1024)

    print("Received data:", received_data)

    # Закрываем соединение
    sock.close()

except bluetooth.BluetoothError as e:
    print("Bluetooth Error:", e)

