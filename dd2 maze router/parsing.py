import re
import itertools
import os
Text = open("file.txt", "r")
file1 = open("write.txt", "w")
x = Text.read()
pattern = r'^(NETS.*END NETS)'
second = re.search(pattern, x, re.MULTILINE | re.DOTALL)
file1.write(second.group(0))

'''
This function basically build the struct for storing each macro with its corrosponding 
pins and each pin with it corrosponding pin positions and maetal layer
'''
def getstruct(inputmacroname, inputpinname, macrounit):
    file1 = open('file_elf.txt', 'r')
    lines = file1.readlines()
    macroname = ''
    pinname = []
    layer = ''
    xindex = yindex = 0
    xlist = []
    ylist = []
    flag2 = False
    flag = False
    '''
    this loops over every macro and start extract the pin information from each one
    '''
    for line in lines:
        words = line.split()
        if(len(words) == 0):
            flag = False
        if(len(words) == 2):
            if(words[0] == 'MACRO'):
                if(words[1] == inputmacroname):
                    flag = True
        if(len(words) == 2):
            if(words[0] == 'PIN'):
                if(words[1] == inputpinname):
                    flag2 = True
        #This for making sure that we have reached the edn of the given pin
        if(flag2 and len(words) == 2 and words[0] == 'END' and words[1] == inputpinname):
            flag2 = False
            _x_list = [float(x) for x in xlist]
            _y_list = [float(y) for y in ylist]
            _len = len(xlist)
            _x = sum(_x_list) / _len
            _y = sum(_y_list) / _len
            return(_x*macrounit, _y*macrounit)
        if(flag2):
        #this ensures that we have already parsing inside a specific pin
            if(len(words) > 0 and words[0] == 'LAYER'):
                layer = words[1][5:]
        #this to get the coordinates of each pin
            if(len(words) > 0 and words[0] == 'RECT'):
                xlist.append(words[1])
                xlist.append(words[3])
                ylist.append(words[2])
                ylist.append(words[4])
    '''
    it basically gets the centroid of the RECT coordinates 
    '''
    _x_list = xlist
    _y_list = ylist
    _len = len(xlist)
    _x = sum(_x_list) / _len
    _y = sum(_y_list) / _len
    return(_x, _y)

'''
This function basically gets all the required information fron the router input file
it does so by looping over all the nets inside the def file and extracting all the corosponidng macros 
and pins and then search inside the lef file for the pin positions
'''
def getall(macrosize):
    file1 = open('file_elf.txt', 'r')
    lines = file1.readlines()
    macroname = ''
    pinname = []
    layer = ''
    xindex = yindex = 0
    xlist = []
    ylist = []
    flag2 = False
    flag = False
    currentmacro = ''
    currentpin = ''
    allmacros = {}
    currentmacropins = {}
    for line in lines:
        words = line.split()
        if(len(words) == 0):
            flag = False
            allmacros[currentmacro] = currentmacropins.copy()
            currentmacropins.clear()
            currentmacro = ''
        if(len(words) == 2):
            if(words[0] == 'MACRO'):
                flag = True
                currentmacro = words[1]
        if(len(words) == 2):
            if(words[0] == 'PIN'):
                flag2 = True
                currentpin = words[1]
        if(flag2 and len(words) == 2 and words[0] == 'END' and words[1] == currentpin):
            flag2 = False
            _x_list = [float(x)*macrosize for x in xlist]
            _y_list = [float(y)*macrosize for y in ylist]
            _len = len(xlist)
            _x = round(sum(_x_list) / _len)
            _y = round(sum(_y_list) / _len)
            currentmacropins[currentpin] = (layer, _x, _y)
        if(flag2):
            if(len(words) > 0 and words[0] == 'LAYER'):
                layer = words[1][5:]
            if(len(words) > 0 and words[0] == 'RECT'):
                xlist.append(words[1])
                xlist.append(words[3])
                ylist.append(words[2])
                ylist.append(words[4])
    return allmacros

#it here assumes that the macron unit inside the def file is 100
don = getall(100)
#This to get the centroid of the coordinates
def centroid(xlist, ylist):
    _x_list = xlist
    _y_list = ylist
    _len = len(xlist)
    _x = sum(_x_list) / _len
    _y = sum(_y_list) / _len
    return(_x, _y)
#This for parsing the def file
with open("write.txt", "r") as paragraphs_file:
    allnets = {}
    currentdict = {}
    currentnet = ''
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
        if len(string) > 2:
            del string[2]
        if len(string) > 2:
            del string[2]
        # string basically is a list that contains all the parsed nets with its corrosponding pins from def
        if(string[0] == '-'):
            currentnet = string[1]
            currentdict.clear()
            allnets[currentnet] = []
        else:
            if(string[0].find('_') != -1):
                currentdict[string[0][:string[0].find('_')]] = string[1]
            else:
                currentdict[string[0]] = string[1]
            allnets[currentnet] = currentdict.copy()
        print(string)
Text.close()
Text = open("file.txt", "r")
#starting from here, we parse the def file to know the micron unit used to calculating the 
#centroid of the pins
getmacrosize = Text.readlines()
macronsize = 0
for line in getmacrosize:
    words = line.split()
    if len(words) > 0 and words[0] == 'UNITS':
        macronsize = int(words[3])
netcoo = {}
for net in allnets:
    if not net.startswith('select') and not net.startswith('grant') and not net.startswith('active'):
        netcoo[net] = []
        for key in allnets[net]:
            alll = getall(macronsize)
            if(key in alll):
                dataa = alll[key][allnets[net][key]]
            netcoo[net].append(
                ' (' + str(dataa[0]) + ', ' + str(dataa[1]) + ', ' + str(dataa[2])+')')
#Here for writing the output from this parsing and then be the input file for the router
f = open('output.txt', 'w')
netcount = 0
for net in netcoo:
    stringgg = 'net'+str(netcount) + ' '.join(netcoo[net])
    print(stringgg)
    f.write(stringgg + '\n')
    netcount += 1
#to erase the last line
with open('output.txt', 'rb+') as filehandle:
    filehandle.seek(-1, os.SEEK_END)
    filehandle.truncate()
f.close()
pass
