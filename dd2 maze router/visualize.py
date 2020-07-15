'''
from mpl_toolkits.mplot3d import axes3d
import matplotlib.pyplot as plt
import numpy as np

fig = plt.figure()
ax = fig.add_subplot(1,1,1, projection='3d')
X, Y, Z = [10,11,12],[20,20,20],[1,1,1]
ax.plot(X, Y, Z)
ax.set_xlabel('x-axis')
ax.set_ylabel('y-axis')
ax.set_zlabel('z-axis')
plt.show()
ax2 = fig.add_subplot(1,1,1, projection='3d')
X, Y, Z = [100,100,100],[200,201,202],[2,2,2]
ax2.plot(X, Y, Z)
plt.show()
'''
from mpl_toolkits.mplot3d.axes3d import Axes3D
import matplotlib.pyplot as plt

def plot_exp():
    #fig, ax = plt.subplots(subplot_kw={'projection': '3d'})
    #datasets = [{"x":[1,2,3], "y":[1,4,9], "z":[0,0,0], "colour": "red"} for _ in range(6)]
    #for dataset in datasets:
        #ax.plot(dataset["x"], dataset["y"], dataset["z"], color=dataset["colour"])
    #plt.show()
    x = []
    y = []
    z = []
    w = []
    x.append(1)
    x.append(2)
    x.append(3)
    y.append(1)
    y.append(4)
    y.append(9)
    z.append(0)
    z.append(0)
    z.append(0)
    w.append(1)
    w.append(1)
    w.append(1)
    color = "red"
    fig, ax = plt.subplots(subplot_kw={'projection': '3d'})
    d2 = {"x":x, "y":y, "z":w, "colour": color}
    datasets = []
    datasets.append(d2)
    datasets = [{'x': [10, 11, 12], 'y': [20, 20, 20], 'z': [1, 1, 1], 'colour': 'red'}, {'x': [10, 10, 10], 'y': [20, 21, 22], 'z': [2, 2, 2], 'colour': 'red'}]
    for dataset in datasets:
        ax.plot(dataset["x"], dataset["y"], dataset["z"], color=dataset["colour"])
    plt.show()


def read_and_plot():
    no_layers = 5
    color = "red"
    nets = []
    sources = []
    targets = []
    vias = []
    fig, ax = plt.subplots(subplot_kw={'projection': '3d'})
    file1 = open('output_file.txt', 'r')
    with open("output_file.txt") as fp:
        while True:
            line = fp.readline()
            x = []
            y = []
            layer = []
            cells = 0
            if not line:
                break
            while True:
                cells += 1
                if line.find("(") == -1:
                    cells = 0
                    targets.append([cell_x,cell_y,cell_layer])
                    break
                temp_pos = line.find("(") + 1
                comma_pos = line.find(",")
                if cells > 1:
                    prev_cell_layer = cell_layer
                cell_layer = int(line[temp_pos: comma_pos])
                line = line[comma_pos + 2:]
                comma_pos = line.find(",")
                cell_x = int(line[0: comma_pos])
                line = line[comma_pos + 2:]
                comma_pos = line.find(")")
                cell_y = int(line[0: comma_pos])
                x.append(cell_x)
                y.append(cell_y)
                layer.append(cell_layer)
                if cells == 1:
                    sources.append([cell_x,cell_y,cell_layer])
                if cells > 1 and prev_cell_layer != cell_layer:
                    vias.append([cell_x,cell_y,cell_layer])
            if layer == 2:
                color = "red"
            net = {"x":x, "y":y, "z":layer, "colour": color}
            nets.append(net)
    ax.set_xlim3d(1000)
    ax.set_ylim3d(1000)
    ax.set_zlim3d(no_layers)
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_zlabel('layer')
    for i, s in enumerate(sources):
        ax.text(s[0], s[1], s[2], '%s' % ("S"), size=20, zorder=1, color='k')
    for i, t in enumerate(sources):
        ax.text(t[0], t[1], t[2], '%s' % ("T"), size=20, zorder=1, color='k')
    for i, v in enumerate(sources):
        ax.text(v[0], v[1], v[2], '%s' % ("V"), size=20, zorder=1, color='k')
    for n in nets:
        ax.plot(n["x"], n["y"], n["z"], color=n["colour"])
    plt.show()


def main():
    read_and_plot()
    #plot_exp()

main()


'''
                    elif layer == 3:
                        color = "green"
                    elif layer == 4:
                        color = "blue"
                    elif layer == 5:
                        color = "yellow"
                    elif layer == 6:
                        color = "orange"

'''