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
                'maxBytes': 1024 * 1024 * 100,
                'backupCount': 100,
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
        # print(info)

    def send_data_2(self, client, data):
        client.send(data.encode('utf-8'))  # 发送TCP数据
        info = client.recv(1024).decode()
        # print(info)


def str_to_time(time_str):
    time_cost = time_str.split('=')[-1]
    time_cost = time_cost.replace('ms', '')
    return float(time_cost)


def stat(log_file):
    total = 0
    total_time = 0
    total_conn_time = 0
    total_req_time = 0
    fp = open(log_file, 'r')
    while True:
        line = fp.readline()
        if line:
            total += 1
            arr = line.split()
            conn_time_cost = str_to_time(arr[4])
            req_time_cost = str_to_time(arr[5])
            total_time_cost = str_to_time(arr[6])
            # print(f'conn_time_cost={conn_time_cost}, req_time_cost={req_time_cost}, total_time_cost={total_time_cost}')
            total_conn_time += conn_time_cost
            total_req_time += req_time_cost
            total_time += total_time_cost
        else:
            break
    print(f'连接耗时={round(total_conn_time / total, 3)}ms, 请求耗时={round(total_req_time / total, 3)}ms, 总共耗时={round(total_time / total, 3)}ms')


if __name__ == '__main__':
    log_dir = os.path.dirname(__file__)
    log_file = log_dir + '/test.log'
    if os.path.exists(log_file):
        # 删除旧的日志文件
        os.remove(log_file)

    create_log(log_file)

    base_path = os.path.dirname(os.path.dirname(log_dir))
    base_path = os.path.abspath(base_path)
    config = configparser.ConfigParser()
    config.read(base_path + '/config/main.ini')

    threads = config['client']['threads']
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

    # 统计每个请求的平均耗时
    stat(log_file)
