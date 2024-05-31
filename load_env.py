import os
import re
from sys import argv
from os import path, listdir

base_path = os.getcwd()
files = [path.join(base_path, file) for file in (argv[1::] if len(argv) > 1 else ['.env'])]

file_paths: list[str] = []
def add_file(file_path:str, deep = 4):
  if deep <= 0:
    return
  elif path.isdir(file_path):
    for file in listdir(file_path):
      add_file(path.join(file_path, file), deep - 1)
  else:
    file_paths.append(file_path)
  
for file_path in files:
  add_file(file_path)

env = {}
for file_path in file_paths:
  try:
    test_str = re.compile('\s*(\"[^\n]*\")')
    
    with open(file_path) as file:
      for line in file:
        if not line.startswith('#') and not line.startswith(';'):
          nline = line.replace('\n', '')
          if len(nline) > 0:
            var = nline.split('=')
            key, value = var[0], var[1]
            is_str = bool(test_str.match(value))
            env[key] = f"'{value}'" if  is_str >= 1 else value
            
  except:
    pass
  
for var in env.keys():
  print(f'-D {var}={env[var]}')