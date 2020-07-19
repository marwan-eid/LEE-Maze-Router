import sys
import matplotlib.pyplot as plt
from matplotlib import colors 
from matplotlib.colors import NoNorm
import numpy as np

pins = []
nets = []
with open("output_file.txt") as fp:
        while True:
            line = fp.readline()
            nets = line.split("net")
            pin = []
            cells = 0
            if not line:
                break
            while True:
                cells += 1
                if line.find("(") == -1:
                    cells = 0
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
                pin.append([cell_layer,cell_x,cell_y])
            pins.append(pin)
input_pins=[]

with open("input_file.txt") as fp:
        while True:
            line = fp.readline()
            nets = line.split("net")
            pin = []
            cells = 0
            if not line:
                break
            while True:
                cells += 1
                if line.find("(") == -1:
                    cells = 0
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
                pin.append([cell_layer,cell_x,cell_y])
            input_pins.append(pin)
   
w=1000
h=1000
n_layers = int(input("please enter number of layers: \n"))
def initialize_layer(foo):
    foo[:] = [[[0 for k in range(h)] for j in range(h)] for i in range(n_layers)]

grids=[]
Matrix=[]
for k in range(n_layers):
    initialize_layer(grids)
for nets in input_pins:
    for pin in nets:
        layer= pin[0]
        cell_x = pin[1]
        cell_y = pin[2]
        grids[layer-1][cell_y-1][cell_x-1]=2

for nets in pins:
    first = nets[0]
    last = nets[-1]
    cell_lll =first[0]
    cell_xxx= first[1]
    cell_yyy= first[2]
    grids[cell_lll-1][cell_yyy-1][cell_xxx-1]=2
    cell_ll =last[0]
    cell_xx= last[1]
    cell_yy= last[2]
    grids[cell_ll-1][cell_yyy-1][cell_xxx-1]=2

    for pin in nets:
        layer= pin[0]
        cell_x = pin[1]
        cell_y = pin[2]
        if ( grids[layer-1][cell_y-1][cell_x-1] != 2):
            grids[layer-1][cell_y-1][cell_x-1]=1

for i in range(n_layers):
    cmap = colors.ListedColormap(['black','red','yellow'])
    plt.grid(False)
    plt.imshow(grids[i],cmap=cmap, vmin=0 , vmax=2)
    plt.show()
    
    for i in range(n_layers):
        text_file = "layer" + str(i) +".txt"
        with open(text_file, 'w') as file:
            file.writelines('\t'.join(str(j) for j in i) + '\n' for i in grids[i])
