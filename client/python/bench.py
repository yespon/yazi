import socket
import threading
import struct
import time
import logging
from logging.config import dictConfig
import os


def create_log(log_file):
    dictConfig({
        'version': 1,
        'formatters': {
            'default': {
                'format': '%(asctime)s %(levelname)s [%(filename)s:%(lineno)s]: %(message)s',
                'datefmt': '%Y-%m-%d %H:%M:%S'
            }
        },
        'handlers': {
            'message': {
                'class': 'logging.handlers.RotatingFileHandler',
                'formatter': 'default',
                'filename': log_file,
                'maxBytes': 1024 * 1024,
                'backupCount': 10,
                'encoding': 'utf-8'
            }
        },
        'root': {
            'level': 'INFO',
            'handlers': ['message']
        }
    })


class ClientThread(threading.Thread):

    def __init__(self, name):
        super().__init__(name=name)
        self.name = name

    def run(self):
        start = time.time()

        host = '127.0.0.1'
        port = 8080

        client = socket.socket()  # 创建TCP/IP套接字
        client.connect((host, port))  # 主动初始化TCP服务器连接
        self.send_data_1(client, f'{self.name}: hello python')
        # self.send_data(client, f'{self.name}: so far so good!')
        client.close()

        end = time.time()
        cost = round((end - start) * 1000, 3)
        logging.info(f'time cost={cost}ms')
    
    def send_data_1(self, client, data):
        cmd = 1
        data_len = len(data)
        data = struct.pack(f'8sII{data_len}s', b'work', cmd, data_len, data.encode('utf-8'))
        client.send(data)  # 发送TCP数据
        info = client.recv(1024).decode()
        print(info)

    def send_data_2(self, client, data):
        client.send(data.encode('utf-8'))  # 发送TCP数据
        info = client.recv(1024).decode()
        print(info)


if __name__ == '__main__':
    log_dir = os.path.dirname(__file__)
    create_log(log_dir + "/test.log")

    threads = 100000

    start = time.time()
    thread_list = list()
    for i in range(threads):
        thread = ClientThread(f'thread{i}')
        thread_list.append(thread)
        thread.start()
    
    for thread in thread_list:
        thread.join()
    print("thread finished , cost %s s" % (time.time() - start))

