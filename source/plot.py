import matplotlib.pyplot as plt
import numpy as np
import sys


values = [float(i) for i in sys.argv[3:]]

plt.plot(np.arange(-10, 10, float(sys.argv[2])), values)

plt.savefig('debug/' + sys.argv[1] + '.png')
