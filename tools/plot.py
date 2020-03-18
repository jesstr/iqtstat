#!/usr/bin/python

import os
import sys
import matplotlib.pyplot as plt


bin_path = '../output/demo'
plot_png = 'plot.png'


def usage(name):
    print('Usage: {} type [kP kI kD ref] iters'.format(name) + '\r\n\
    type - {pid|tres} - Algorithm type\r\n\
    kP - Proportional (P) component\r\n\
    kI - Integrative (I) component\r\n\
    kD - Derivative (D) component\r\n\
    ref - Reference temperature value\r\n\
    iters - Max number of PID iterations')

def movavg(raw, window=30):
    mean = 71500
    extended = ((mean, ) * (window - 1)) + raw
    return tuple((sum(extended[i:i + window]) / window) for i in range(0, len(raw)))

def get_data_from_file(path):
    data = tuple(zip(*tuple((float(m), float(e), float(r), float(i), float(f), float(h)) \
        for m, e, r, i, f, h in map(lambda s: s.split('\t'), open(path).readlines()))))
    return data

def get_data_from_stream(path):
    stream = os.popen(path)
    data = tuple(zip(*tuple((float(m), float(e), float(r), float(i), float(f), float(h)) \
        for  m, e, r, i, f, h in map(lambda s: s.split('\t'), stream.readlines()))))
    return data


def main():
    for s in sys.argv[2:]:
        try:
            float(s)
        except:
            usage(sys.argv[0])
            sys.exit(2)

    if sys.argv[1] == "pid":
        if len(sys.argv) != 7:
            usage(sys.argv[0])
            sys.exit(2)

    if sys.argv[1] == "tres":
        if len(sys.argv) != 3:
            usage(sys.argv[0])
            sys.exit(2)

    bin_args = ' '.join(sys.argv[1:])

    dirname = os.path.split(os.path.abspath(__file__))[0]
    data = get_data_from_stream(dirname + '/' + bin_path + ' ' + bin_args)

    fig, ax = plt.subplots()

    ax.plot(data[0], color='blue', label='Temperature')
    ax.plot(data[1], color='red', label='Tmin/Err')
    ax.plot(data[2], color='green', label='Tmax/Ref')
    ax.plot(data[3], color='orange', label='Input')
    ax.plot(data[4], color='yellow', label='Fan')
    ax.plot(data[5], color='purple', label='Heat')

    ax.set(xlabel='Time', ylabel='Temperature',
        title='PID demo')

    ax.legend()
    ax.grid()

    fig.set_size_inches(10, 5)
    fig.savefig(plot_png)

    try:
        plt.show()
    except:
        sys.exit(2)


if __name__ == '__main__':
    main()