#!/usr/bin/python

import os
import sys
import matplotlib.pyplot as plt


bin_path = '../output/demo'
plot_png = 'plot.png'


def usage(name):
    print('Usage: {} kP kI kD ref iters'.format(name) + '\r\n\
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
    data = tuple(zip(*tuple((float(f), float(r), float(h)) for f, r, h in map(lambda s: s.split('\t'), open(path).readlines()))))
    return data

def get_data_from_stream(path):
    stream = os.popen(path)
    data = tuple(zip(*tuple((float(f), float(r), float(h)) for f, r, h in map(lambda s: s.split('\t'), stream.readlines()))))
    return data


def main():
    if len(sys.argv) != 6:
        usage(sys.argv[0])
        sys.exit(2)

    for s in sys.argv[1:]:
        try:
            float(s)
        except:
            usage(sys.argv[0])
            sys.exit(2)

    bin_args = ' '.join(sys.argv[1:])
    reference = float(sys.argv[4])

    dirname = os.path.split(os.path.abspath(__file__))[0]
    data = get_data_from_stream(dirname + '/' + bin_path + ' ' + bin_args)

    fig, ax = plt.subplots()

    ax.axhline(reference, color='red', label='reference', linestyle='dashed', linewidth=.5)
    ax.plot(data[2], color='orange', label='input')
    ax.plot(data[0], color='blue', label='temperature')

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