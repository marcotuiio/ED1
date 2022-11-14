import os
import subprocess
import time
import datetime

# BED = Path BED (pasta de testes do Evandro):
BED = '/home/marcotuiio/Data_Structures/T4/inputs'

# BSD = Path de saída:
BSD = '/home/marcotuiio/Data_Structures/T4/outputs'

# src = Path da pasta SRC:
src = '/home/marcotuiio/Data_Structures/T4/src'

ini = datetime.datetime.now()

os.chdir(src)

# Ative a função abaixo caso tenha alguma coisa no Makefile que limpe todos os *.o
subprocess.call(["make", "clear"])  

# Observação, quem tiver o executável diferente de 'ted', mude no código todo para o nome certo
subprocess.call(["make", "t2"])

print("\n")

geo = [] 
files = []
subdir = []
for (dirpath, dirnames, filenames) in os.walk(BED):
    
    files.extend(filenames)
    subdir.extend(dirnames)
    break

subdir.sort()

geo = [ fi for fi in files if fi.endswith(".geo") ]
geo_no_qry = []

for i in range(len(geo)):
    if geo[i].split('.')[0] not in subdir:
        geo_no_qry.append(geo[i])

subqry = []

for i in subdir:
    subsubqry = []
    for (dirpath, dirnames, filenames) in os.walk(BED + "/" + i):
        subsubqry.extend(filenames)
        subqry.append(subsubqry)
        break

geo.sort()
geo_no_qry.sort()
subdir.sort()

for i in range(len(geo_no_qry)):
    geo.remove(geo_no_qry[i])

print("==========> Executando testes que possuem arquivos .geo e .qry <==========\n\n")

count = 0
for files_in_subqry in subqry:
    files_in_subqry.sort()
    for j in files_in_subqry:
        print(f"./t2 -e {BED} -o {BSD} -f {geo[count]} -q {subdir[count] + '/' + j}")
        print("\n")
        subprocess.call(["./t2", "-e", BED, "-o", BSD, "-f", geo[count], "-q", subdir[count] + "/" + j])
    count += 1 

print("==========> Executando testes que possuem apenas arquivos .geo <==========\n\n")

for f in geo_no_qry:
    print(f"./t2 -e {BED} -o {BSD} -f {f}")
    print("\n")
    subprocess.call(["./t2", "-e", BED, "-o", BSD, "-f", f])
    
fin = datetime.datetime.now()

print(f"Tempo para rodar:\n{str(fin - ini)}")