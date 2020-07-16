import re
mybuffer = []
with open("file.txt", "r") as paragraphs_file: 
    i = 0
    for paragraph in paragraphs_file:
        if paragraph.startswith('PINS'):
            #print(paragraph)
            for word in paragraph.split():
                try:
                    a = int(word)
                    #print(a)
                except ValueError:
                    pass
        if "+ LAYER" in paragraph:
           # print(paragraph)
            res = paragraph.split(' ')[4]
            s = re.search(r"\d+(\.\d+)?", res)
        if "PLACED" in paragraph and paragraph.startswith(' '):
            #print(paragraph)
            test_list = [int(d) for d in re.findall(r'-?\d+', paragraph)]
            result = str(test_list)[1:-1]
            print('net' + str(i), '(', s.group(0), ',', result, ')' )
            i+=1

            #print(result)
Text = open("file.txt", "r")
x = Text.read()
pattern = r'^(NETS.*END NETS)'
second = re.search(pattern, x, re.MULTILINE | re.DOTALL)
print(second.group(0))

