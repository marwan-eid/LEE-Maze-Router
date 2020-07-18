import re
import itertools
import os
Text = open("file.txt", "r")
file1 = open("write.txt", "w")  
x = Text.read()
pattern = r'^(NETS.*END NETS)'
second = re.search(pattern, x, re.MULTILINE | re.DOTALL)
file1.write(second.group(0))

with open("write.txt", "r") as paragraphs_file:
    for paragraph in paragraphs_file:
        if paragraph.startswith('NETS'):
            for word in paragraph.split():
                try:
                    netNo = int(word)
                except ValueError:
                    pass
        if paragraph.startswith('NETS') or paragraph.startswith('END'):
            continue 
        else:
            string = paragraph.split()
        if string[0].startswith('('):
            del string[0]
        if len(string) >2:
            del string[2]
        if len(string) >2:
            del string[2]
        #string basically is a list that contains all the parsed nets with its corrosponding pins from def
        #print(string)
    

elf_Text = open("lef.txt", "r")
elf_file = open("file_elf.txt", "w")
elf_x = elf_Text.read()
elf_pattern = r'^(MACRO.+END LIBRARY)'
elf_second = re.search(elf_pattern, elf_x, re.MULTILINE | re.DOTALL)
elf_file.write(elf_second.group(0))
elf_file2 = open("file_elf.txt", "r")
elf_x2 = elf_file2.read()

i=0
macro_second=[]
macro_second2 = []
pins_name=[]
layer_name=[]
pin_positions=[]
b=[]
arg=[]
dicts = {}
with open("file_elf.txt", "r") as lef_paragraphs_file:
    for lef_paragraph in lef_paragraphs_file:
        if re.match(r'MACRO', lef_paragraph):
           lef_macro = lef_paragraph.split(' ')[1]
           lef_macro_final = 'END' + " " + lef_macro
           lef_macro_start = 'MACRO' + " " + lef_macro
           #print(lef_macro)
           macro_second.append(lef_macro)
           macro_second2.append(lef_macro_start)
           i+=1
           #dicts.update({i: lef_macro})
        else: 
            continue
        
z=0
#for x in range (len(macro_second)):
elf_file_macro = open("donia.txt", "r+")
content = elf_file_macro.read()
pin_count = sum(
    1 for match in re.finditer(r"\bPIN\b", content))
#macro_second.pop(0)
elf_pattern2 = r'^' + re.escape(macro_second2[0]) + \
    '.+' + re.escape(macro_second[0])
elf_second2 = re.search(elf_pattern2, elf_x2,
                    re.MULTILINE | re.DOTALL)
elf_file_macro.seek(0)
elf_file_macro.truncate()
elf_file_macro.write((elf_second2.group(0)))
elf_file_macro.close()
with open("donia.txt", "r+") as lef_macros_file:
    for lef_macros in lef_macros_file:
        if lef_macros.startswith(" ") or lef_macros.startswith("   "):
            temp_pin = lef_macros.split()
            #print(temp_pin)
            if 'PIN' in temp_pin:
                lef_pin = lef_macros.split(' ')[3]
                pins_name.append(lef_pin)
            if 'LAYER' in temp_pin:
                lef_layer = lef_macros.split(' ')[7]
                #print(lef_layer)
                layer_name.append(lef_layer)
            if 'RECT' in temp_pin:
                pin_positions.append(temp_pin)
                #print(lef_layer)

#for m in range(2):
    #conc = {lef_macro: pins_name[m], left_macro: pin_name[m+1]}
    #dicts.setdefault(macro_second, {})[pins_name] = 1
    dicts.setdefault(macro_second[0], []).append(pins_name)
    #dicts.update({lef_macro: pins_name[1]})
    '''IDs = dicts['FILL\n']
    Defaults = macro_second
    D = dict.fromkeys(IDs, Defaults)'''


def centroid(vertexes):
    _x_list = [vertex[0] for vertex in vertexes]
    _y_list = [vertex[1] for vertex in vertexes]
    _len = len(vertexes)
    _x = sum(_x_list) / _len
    _y = sum(_y_list) / _len
    return(_x, _y)
c=[]
final2=[]
new_position=[]
pin_position=[]
def ayhaga(list):
    for i in list:
        b.append(float(i))
    return b
def convert(list):
    return tuple(list)
R=[]
new_temp=[]
for m in range(pin_count):
    pin_positions[m].pop(-1)
    pin_positions[m].pop(0)
    pin_positions[m] = [float(i) for i in pin_positions[m]]
    for d in range(4):
        temp = pin_positions[m] 
        new_temp.append(temp[d]*100)
    final2 = centroid(
        convert(zip(new_temp[::2], new_temp[1::2])))  # (0.5, 0.5)
    pin_position.append(final2)
    '''#print(type(final2))
    out = list(itertools.chain(float)(*(final2)))
    print(out)
    final = dict(zip(pins_name, R))
    #print(final)'''

combined = dict(zip(pins_name, pin_position))
combined2 = dict(zip(pins_name, layer_name))

