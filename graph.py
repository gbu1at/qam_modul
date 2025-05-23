import sys
import os
import numpy as np
import matplotlib.pyplot as plt


os.system("make clean")
os.system("make")



modules = ["QPSK", "QAM16", "QAM64"]

for mod in modules:
    X = np.linspace(0, 2, 100)
    y = []

    for x in X:
        os.system(f"./main {x} {mod} input.txt")
        diff = float(open("input.txt").read())
        y.append(diff)

    plt.plot(X, y)
    plt.savefig(f"graph_{mod}.png")
    plt.close()

os.system("rm -f input.txt")
os.system("make clean")
