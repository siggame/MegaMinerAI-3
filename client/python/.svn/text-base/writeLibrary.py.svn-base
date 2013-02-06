import re
import sys

mapping = {
  'String' : 'c_char_p',
  'int' : 'c_int',
  'float' : 'c_float',
  'boolean' : 'c_int',
  'Pointer' : 'c_void_p',
  'void' : 'None'}


def matchFunction(string):
  return re.search("(\w+) (\w+)\(([a-zA-Z_, ]*)\);$", string)

def writeLibrary(input, output):
  for i in input:
    pattern = matchFunction(i)
    if not pattern:
      continue
    type = None
    name = None
    args = []
    
    type = mapping[pattern.group(1)]
    name = pattern.group(2)
    #print pattern.group(3)
    for i in pattern.group(3).split(','):
      if not i:
        continue
      args.append(mapping[(i.strip().split(' '))[0]])
    
    output.write('library.' + name + '.restype = ' + type + '\n')
    output.write('library.' + name + '.argtypes = ' + str(args).replace("'", '') + '\n\n')
    #output.write(type + ' ' + name + ' ' + str(args))

writeLibrary(sys.stdin, sys.stdout)