
import os


def str_to_time(time_str):
    time_cost = time_str.split('=')[-1]
    time_cost = time_cost.replace('ms', '')
    return float(time_cost)


def stat():
    total = 0
    total_time = 0
    total_conn_time = 0
    total_req_time = 0
    log_dir = os.path.dirname(__file__)
    fp = open(log_dir + '/test.log', 'r')
    while True:
        line = fp.readline()
        if line:
            total += 1
            arr = line.split()
            conn_time_cost = str_to_time(arr[4])
            req_time_cost = str_to_time(arr[5])
            total_time_cost = str_to_time(arr[6])
            print(f'conn_time_cost={conn_time_cost}, req_time_cost={req_time_cost}, total_time_cost={total_time_cost}')
            total_conn_time += conn_time_cost
            total_req_time += req_time_cost
            total_time += total_time_cost
        else:
            break
    print(f'total_time_cost={round(total_time / total, 3)}')
    print(f'conn_time_cost={round(total_conn_time / total, 3)}ms')
    print(f'req_time_cost={round(total_req_time / total, 3)}ms')


if __name__ == '__main__':
    stat()
