import socket
import threading
import struct
import time
import logging
from logging.config import dictConfig


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
            'wsgi': {
                'class': 'logging.StreamHandler',
                'stream': 'ext://flask.logging.wsgi_errors_stream',
                'formatter': 'default'
            },
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
            'handlers': ['wsgi', 'message']
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
        self.send_data(client, f'{self.name}: hello python')
        # self.send_data(client, f'{self.name}: so far so good!')
        client.close()

        end = time.time()
        cost = round(end - start, 3)
        logging.info(f'time cost={cost}s')
    
    def send_data(self, client, data):
        cmd = 1
        data_len = len(data)
        data = struct.pack(f'8sII{data_len}s', b'work', cmd, data_len, data.encode('utf-8'))
        client.send(data)  # 发送TCP数据
        info = client.recv(1024).decode()
        print(info)



if __name__ == '__main__':

    create_log("/root/yazi/client/python/bench.log")

    start = time.time()
    thread_list = list()
    for i in range(1024):
        thread = ClientThread(f'thread{i}')
        thread_list.append(thread)
        thread.start()
    
    for thread in thread_list:
        thread.join()
    print("thread finished , cost %s s" % (time.time() - start))
