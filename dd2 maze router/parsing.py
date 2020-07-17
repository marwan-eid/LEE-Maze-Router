import re
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
i=0
macro_second=[]
dicts = {}
with open("file_elf.txt", "r") as lef_paragraphs_file:
     for lef_paragraph in lef_paragraphs_file:
        if re.match(r'MACRO', lef_paragraph):
           lef_macro = lef_paragraph.split(' ')[1]
           #dicts.update({i: lef_macro})
        if lef_paragraph.startswith(" ") or 'END' + " " + lef_macro not in lef_paragraph:
            temp= lef_paragraph.split()
            if 'PIN' in temp:
                lef_pin = lef_paragraph.split(' ')[3]
                dicts.update({lef_macro: lef_pin})
                #print(lef_pin)
        else:
            continue 
#print(dicts)
        macro_pattern = r'^(MACRO.*END FILL)'
        macro_second = re.search(macro_pattern, elf_x, re.MULTILINE | re.DOTALL)
        print(macro_second.group(0))
        i += 1
        print('hamooooot')


        


