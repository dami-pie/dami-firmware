from base64 import b32encode, b64encode
from sys import argv, stderr
from pick import pick
from getpass import getpass
import os
import time
import json


def get_key_bytes(key, encode=None):

    if encode == 'b64' or encode == 'B64':
        byte_key = b64encode(bytearray(key, encoding='utf-8'))
    elif encode == 'b32' or encode == 'B32':
        byte_key = b32encode(bytearray(key, encoding='utf-8'))
    else:
        byte_key = bytearray(key, encoding='UTF-8')

    encoded_key = byte_key.decode(encoding="utf-8")

    print(f'Secret key: {key}')

    print(f'Encoded key: {byte_key.decode(encoding="utf-8")}')

    print(f'Key bytes: {[hex(byte) for byte in byte_key]}')

    return (key, encoded_key, byte_key)


def main():

    wifi_config = dict(
        user=os.getenv("WIFI_USER", default=""),
        ssid=os.getenv("WIFI_SSID", default=""),
        type=os.getenv("WIFI_METHOD", default=""),
        password=os.getenv("WIFI_PASSWORD", default="")
    )

    broker_config = dict(
        address=os.getenv("BROKER_ADDRESS", default=""),
        port=os.getenv("BROKER_PORT", default=""),
        user=os.getenv("BROKER_USER", default=""),
        password=os.getenv("BROKER_PASSWORD", default="")
    )

    secure_key = dict(
        encode_type="",
        key="",
        encoded_key=""
    )

    device_config = dict(
        id=os.getenv('DEVICE_ID', default=""),
        wifi=wifi_config,
        broker=broker_config,
        secure_key=secure_key
    )

    while True:
        if not device_config['id']:
            time.sleep(0.1)
            print('Chose a ID from your device:')
            device_config['id'] = input()

        if not wifi_config['ssid']:
            time.sleep(0.1)
            print('Insert the WiFi SSID:')
            wifi_config['ssid'] = input()

        if not wifi_config['type']:
            time.sleep(0.1)
            wifi_config['type'] = pick(['WPA2', 'PEAP'], 'Please choose wifi auth method: ', indicator='>')[0]

        if wifi_config['type'] == 'PEAP' and not wifi_config['user']:
            time.sleep(0.1)
            print('Type the login user:')
            wifi_config['user'] = input()

        if not wifi_config['password']:
            time.sleep(0.1)
            print('Type the password')
            # wifi_config['password'] = getpass()
            wifi_config['password'] = input()

        if not broker_config['address']:
            time.sleep(0.1)
            print('Insert broker address or domain:')
            broker_config['address'] = input()

        if not broker_config['port'].isdecimal():
            time.sleep(0.1)
            print('Insert broker address port:')
            while not broker_config['port'].isdecimal():
                broker_config['port'] = input()
                if not broker_config['port'].isdecimal():
                    print('Chose a number!', file=stderr)

        if not broker_config['user']:
            time.sleep(0.1)
            print('Insert broker user:')
            broker_config['user'] = input()

        if not broker_config['password']:
            time.sleep(0.1)
            print('Insert broker user password:')
            # broker_config['user'] = getpass()
            broker_config['password'] = input()

        if not secure_key['encode_type']:
            time.sleep(0.1)
            secure_key['encode_type'] = pick(
                ['b32', 'b64', 'raw'],
                'Choose a secure key encode: ', indicator='>')[0]

        if not secure_key['key']:
            time.sleep(0.1)
            print('Chose a secure key:')
            secure_key['key'], encoded_key, _bytes = get_key_bytes(input(), secure_key['encode_type'])
            secure_key['encoded_key'] = encoded_key

        device_config['broker'] = broker_config
        device_config['secure_key'] = secure_key
        device_config['wifi'] = wifi_config

        time.sleep(0.1)
        config_print = json.dumps(device_config, indent=4, sort_keys=True)
        should_ = pick(
            ['Continue', 'Edit', 'Cancel'],
            f'Please confirm the device configuration before dump: \n{config_print}', indicator='>')[0]

        if should_ == 'Cancel':
            exit(1)
        elif should_ == 'Continue':
            break
        else:
            edit_field = pick([key for key in device_config.keys()],
                              f'Select a fileld to edit: \n{device_config}', indicator='>')[0]
            edit_dict = device_config[edit_field]
            edit_field = pick([key for key in edit_dict.keys()],
                              f'Select a fileld to edit: \n{device_config}', indicator='>')[0]
            edit_dict[edit_field] = ""

    default_dir = os.path.join(os.path.dirname(os.path.abspath(argv[0])), '..', 'data/configs/env')
    dump_config_path = os.getenv('DIST_PATH', default=default_dir)
    if not os.path.exists(dump_config_path):
        print('Creating env folder:')
        print(dump_config_path)
        os.makedirs(dump_config_path)

    time.sleep(0.2)
    print('Starting dump data', end='...')
    # try:

    with open(os.path.join(dump_config_path, 'id'), '+bw') as key_file:
        key_file.write(bytearray(device_config['id'], encoding="ascii"))

    with open(os.path.join(dump_config_path, 'otp.config.bin'), '+bw') as key_file:
        key_file.write(_bytes)

    with open(os.path.join(dump_config_path, 'wifi.config'), '+bw') as wifi_file:
        if wifi_config["type"] == 'PEAP':
            config_content = f'0;{wifi_config["ssid"]};{wifi_config["user"]};{wifi_config["password"]}'
        else:
            config_content = f'1;{wifi_config["ssid"]};{wifi_config["password"]}'

        wifi_file.write(bytearray(config_content, encoding='ascii'))

    with open(os.path.join(dump_config_path, 'broker.config'), '+bw') as broker_files:
        file_content = f'{broker_config["address"]};'
        file_content = file_content + f'{broker_config["port"]};'
        file_content = file_content + f'{broker_config["user"]};'
        file_content = file_content + f'{broker_config["password"]}'
        broker_files.write(bytearray(file_content, encoding='ascii'))

        print('Done!')
        exit(0)
    # except:
    #   print('Fail!')


if __name__ == '__main__':
    main()
