import re
import sys

mapping = {
  'String' : '[str]',
  'int' : '[int, float]'}


def matchFunction(string):
  return re.search('public [0-9a-zA-Z ]+ (\w+)\(([a-zA-Z_, ]*)\)$', string)

def matchClass(string):
  return re.search('^class (\w+)$', string)

def writeClass(input, output):
  c = None
  for i in input:
    pattern = matchClass(i)
    if pattern:
      c = pattern.group(1)
      output.write('class ' + c + '(GameObject):\n')
      continue
    pattern = matchFunction(i)
    if pattern:
      n = pattern.group(1)
      args = pattern.group(2) #get the argument list
      args = args.split(',') #split it by each argument
      args = [i.strip().split(' ') for i in args] #split into type, name pairs
      args = [i for i in args if len(i) == 2] # get rid of non-pairs (empty argument lists)
      
      output.write('  def ' + n + '(self')
      for i,j in enumerate(args):
        output.write(', ')
        output.write(j[1])
      output.write('):\n')
      output.write('    self.validify()\n')
      
      for i in args:
        output.write('    if ' + i[1] + '.__class__ not in ')
        if i[0] in mapping:
          output.write(mapping[i[0]] + ':\n      raise TypeError(\'' + i[1] + ' should be of ' + mapping[i[0]] + '\')\n')
        else:
          output.write('[' + i[0] + ']:\n      raise TypeError(\'' + i[1] + ' should be of [' + i[0] + ']\')\n')
          output.write('    ' + i[1] + '.validify()\n')
      
      output.write('    return library.' + c.replace(c[0], c[0].lower(), 1) + n.replace(n[0], n[0].upper(), 1) + '(self.ptr')
      for i,j in enumerate(args):
        output.write(', ')
        if j[0] in mapping:
          output.write(j[1])
        else:
            output.write(j[1] + '.ptr')
      output.write(')\n\n')

writeClass(sys.stdin, sys.stdout)
