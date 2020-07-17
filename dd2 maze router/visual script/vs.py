#text = open("output_file.txt","r")
#y = text.readline()
#nets = y.split("net")
import sys
import matplotlib.pyplot as plt
from matplotlib import colors 
from matplotlib.colors import NoNorm
import numpy as np
pins = []
nets = []
#nets.remove(nets[0])
with open("output_file1.txt") as fp:
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
                #print(pin) #reflects a pin
            #pin.clear()
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
                #print(pin) #reflects a pin
            #pin.clear()
            input_pins.append(pin)
   
#print(pins[3])
w=1000
h=1000
n_layers = int(input("please enter number of layers: \n"))
def initialize_layer(foo):
    foo[:] = [[[0 for k in range(h)] for j in range(h)] for i in range(n_layers)]

   #foo[:].pop(0)
   #print (foo[999][10])
   #print (foo[10][10])
'''
print('\n'.join([''.join(['{:4}'.format(item) for item in row]) 
for row in foo]))
'''


grids=[]
Matrix=[]
for k in range(n_layers):
    initialize_layer(grids)
    #grids[:].append(Matrix)
for nets in input_pins:
    for pin in nets:
        layer= pin[0]
        cell_x = pin[1]
        cell_y = pin[2]
        grids[layer-1][cell_x-1][cell_y-1]=2

#print(grids[0])
for nets in pins:
    #print (nets)
    first = nets[0]
    last = nets[-1]
    cell_lll =first[0]
    cell_xxx= first[1]
    cell_yyy= first[2]
    grids[cell_lll-1][cell_xxx-1][cell_yyy-1]=2
    cell_ll =last[0]
    cell_xx= last[1]
    cell_yy= last[2]
    grids[cell_ll-1][cell_xx-1][cell_yy-1]=2
    '''
    print(Matrix[cell_xxx][cell_yyy])
    print("cell_xx_first is" + str(cell_xx))
    print("cell_yy_first is" + str(cell_yy))
    print("cell_xx_last is" + str(cell_xxx))
    print("cell_yy_last is" + str(cell_yyy))
   '''
    
    #print(Matrix[cell_xx][cell_yy])
    for pin in nets:
        #print (pin)
        layer= pin[0]
        cell_x = pin[1]
        cell_y = pin[2]
        if ( grids[layer-1][cell_x-1][cell_y-1] != 2):
            grids[layer-1][cell_x-1][cell_y-1]=1




'''
n = np.array(Matrix).astype(np.float) 
cmap = colors.ListedColormap(['White','Blue','red'])
plt.figure(figsize=(100,100))
plt.pcolor(n[::-1],cmap=cmap,edgecolors='k', linewidths=3)
plt.xticks(np.arange(100,100,step=1))
plt.yticks(np.arange(100,100,step=1))
'''
#plt.show()
for i in range(n_layers):
    cmap = colors.ListedColormap(['black','red','yellow'])
    plt.grid(False)
    plt.imshow(grids[i],cmap=cmap, vmin=0 , vmax=2)
    plt.show()

#for x in Matrix:
    #print(*x, sep='')
    
    for i in range(n_layers):
        text_file = "layer" + str(i) +".txt"
        with open(text_file, 'w') as file:
            file.writelines('\t'.join(str(j) for j in i) + '\n' for i in grids[i])
    
    

'''
initialize_layer(Matrix)
while not pins:
    for x in w:
        for y in h:
            #Matrix[x][y]=pins[]
#Matrix [5][5]=3 #assign value for layer here
#print(Matrix[10][10])

'''
#while not pins:

    
#print (nets)
#pins = nets[0].split()
#print (pins)


'''
sources = []
targets = []

with open("output_file.txt") as fp:
        while True:
            line = fp.readline()
            x = []
            y = []
            pin = []
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
                pin.append([cell_layer,cell_x,cell_y])
              
                #print(pin)
                x.append(cell_x)
                y.append(cell_y)
                pins.append(pin)
                if(cells==1):
                    layer.append(cell_layer)
                if cells == 1:
                    sources.append([cell_x,cell_y,cell_layer])

print (pins)          

'''
'''
 if "motorcycle" in all_text or 'bike' in all_text or 'cycle' in all_text or 'dirtbike' in all_text:
print 'found one of em'
'''


   