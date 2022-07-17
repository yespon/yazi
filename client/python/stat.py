
import os


def stat():
    total = 0
    total_time = 0
    log_dir = os.path.dirname(__file__)
    fp = open(log_dir + '/test.log', 'r')
    while True:
        line = fp.readline()
        if line:
            total += 1
            arr = line.split()
            txt = arr[-1]
            time_cost = txt.split('=')[-1]
            time_cost = time_cost.replace('ms', '')
            total_time += float(time_cost)
        else:
            break
    print(f'total={total}')
    print(f'total time={round(total_time, 3)}ms')
    print(f'average time={round(total_time / total, 3)}ms')


if __name__ == '__main__':
    stat()
