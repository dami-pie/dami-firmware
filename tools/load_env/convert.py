
from base64 import b32decode, b32encode, b64decode
from functools import reduce
import re
from sys import argv

secret = argv[1].upper()


for match in re.findall("[^A-Z\d=]", secret):
  secret = secret.replace(match, "")

pad = len(secret) 
pad = pad % 8 if pad >= 8 else 1

for i in range(1 if pad <= 0 else pad):
  secret += "=" 

print(f'Converting secret "{secret}": ')
print("{", end=" ")
for b in b32decode(secret):
  print(f"{hex(b)},", end=" ")
print("}")
