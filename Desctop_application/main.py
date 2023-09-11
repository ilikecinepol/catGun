from flask import Flask, render_template, request
import asyncio
import bluetooth

app = Flask(__name__)

device_dict = {}
device_addr = ''
selected_device_name = ''
sock = ''

# Определение маршрута для обработки GET-запроса
@app.route('/')
def index():
    global device_dict
    device_dict = asyncio.run(discover_devices())
    print(f'devices: {device_dict}')
    return render_template('index.html', device_dict=device_dict)


# Определение маршрута для обработки POST-запроса
@app.route('/button_click', methods=['POST'])
def button_click():
    button_name = request.form.get('button_name')  # Получаем название кнопки из запроса
    print('Button Clicked:', button_name)  # Выводим название кнопки в консоль
    # Здесь вы можете выполнить нужные действия в зависимости от нажатой кнопки
    if button_name == '↑':
        sock.send("1")
    elif button_name == '↓':
        sock.send("2")
    elif button_name == '→':
        sock.send("3")
    elif button_name == '←':
        sock.send("4")
    elif button_name == 'X':
        sock.send("5")
    elif button_name == 'Y':
        sock.send("6")
    return 'Button Clicked: ' + button_name  # Отправляем ответ клиенту


@app.route('/connect')
def connect():
    global device_dict, device_addr, selected_device_name, sock
    selected_device = request.args.get('device')
    selected_device_name = device_dict.get(selected_device, 'N/A')  # Получаем имя устройства
    print(f'try to connect from {selected_device_name}')
    device_addr = list(device_dict.keys())[list(device_dict.values()).index(selected_device_name)]
    print(device_addr)
    try:
        # Попытка установить соединение с указанным устройством
        sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
        sock.connect((device_addr, 1))  # Порт RFCOMM может быть разным, в зависимости от вашего устройства

        print(f"Connected to device with address: {device_addr}")

        # Теперь вы можете отправлять и принимать данные через 'sock'
        # Пример отправки данных
        # sock.send("Hello, Bluetooth!")

        # Закрываем соединение
        # sock.close()
        # print("Connection closed")
        return "Connection Success", 200
    except Exception as e:
        # Если произошла ошибка, возвращаем HTTP-статус 500 (Внутренняя ошибка сервера)
        return "Connection Failed", 500


@app.route('/device_info')
def device_info():
    global selected_device_name
    try:
        selected_device = request.args.get('device')
        selected_device_name = device_dict.get(selected_device, 'N/A')  # Получаем имя устройства
        print(f'device_info: {device_info}')
        return "Connection Success", 200
    except Exception as e:
        # Если произошла ошибка, возвращаем HTTP-статус 500 (Внутренняя ошибка сервера)
        return "Connection Failed", 500


async def discover_devices():
    global device_dict
    print('discover')
    # device_dict = {}  # Создаем пустой словарь для сохранения устройств
    nearby_devices = await asyncio.to_thread(bluetooth.discover_devices, duration=8, lookup_names=True)

    if not nearby_devices:
        print("No Bluetooth devices found.")
        return {}

    for addr, name in nearby_devices:
        if name != '' or name != None:
            print(f"Device Name: {name}, Address: {addr}")
            device_dict[addr] = name  # Используем адрес как ключ и имя как значение

    return device_dict


if __name__ == '__main__':
    try:
        app.run(host='0.0.0.0', port=8080, debug=True)
    finally:
        # Закрытие соединения при завершении работы Flask-приложения
        print("Flask приложение завершено. Закрываем соединения.")

        try:
            asyncio.run(disconnect_device(device_addr))
        except Exception as e:
            print(f"Ошибка при отключении от устройства {device_address}: {str(e)}")
