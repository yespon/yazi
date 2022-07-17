import socket
import threading
import struct
import time
import logging
from logging.config import dictConfig
import os
import configparser


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
                'maxBytes': 1024 * 1024 * 10,
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
        start_time = time.time()

        host = '127.0.0.1'
        port = 8080

        client = socket.socket()  # 创建TCP/IP套接字
        client.connect((host, port))  # 主动初始化TCP服务器连接

        # 建立连接消耗的时间
        conn_time = time.time()

        self.send_data_1(client, f'{self.name}: hello python')
        # self.send_data(client, f'{self.name}: so far so good!')

        # 请求响应消耗的时间
        resp_time = time.time()

        client.close()

        close_time = time.time()
        conn_time_cost = round((conn_time - start_time) * 1000, 3)
        req_time_cost = round((resp_time - conn_time) * 1000, 3)
        total_time_cost = round((close_time - start_time) * 1000, 3)
        logging.info(f'conn_time={conn_time_cost}ms req_time_cost={req_time_cost}ms total_time_cost={total_time_cost}ms')
    
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

    # 删除旧的日志文件
    os.remove(log_dir + '/test.log')

    create_log(log_dir + '/test.log')

    base_path = os.path.dirname(os.path.dirname(log_dir))
    config = configparser.ConfigParser()
    config.read(base_path + '/config/main.ini')

    threads = config['bench']['threads']
    threads = int(threads)

    start = time.time()
    thread_list = list()
    for i in range(threads):
        thread = ClientThread(f'thread{i}')
        thread_list.append(thread)
        thread.start()
    
    for thread in thread_list:
        thread.join()
    total_time = round((time.time() - start), 3)
    print(f'thread finished, total time cost: {total_time}s')

