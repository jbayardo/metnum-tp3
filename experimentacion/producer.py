import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import os
import re
import pandas as pd
import math

def fromSTRToNumpy(data):
    output = []

    for line in data.strip().split('\n'):
        output += map(lambda x: float(x.strip()), line.strip().split(','))

    return np.array(output)

def ecm(Iorig,Inew):
    mn = len(Iorig)
    res = 0.0
    for ij in range(0,mn):
            res += math.pow(Iorig[ij] - Inew[ij],2)
    res /= mn
    return res

def psnr(Iorig,Inew):
    return 10.0*math.log10(math.pow(255,2)/ecm(Iorig,Inew))

# Este script larga output.csv, con el formato que buscamos para que lo tome
# el software para graficar, Tableau.

dataset = []

files = map(lambda x: 'splines/'+x, os.listdir('splines')) + map(lambda x:'bilineal/'+x, os.listdir('bilineal')) + map(lambda x:'knn/'+x, os.listdir('knn'))
contraste = map(lambda x: 'originales/'+x, os.listdir('originales'))
originales = {}

for fname in contraste:
    if fname[-3:] != 'csv':
        continue

    matches = re.match(r"^(originales)\/(.*)\.csv$", fname)
    name = matches.group(2).strip()

    with open(fname, 'r') as handle:
        data = handle.read()

    originales[name] = fromSTRToNumpy(data)

for fname in files:
    if fname[-3:] != 'csv':
        continue

    matches = re.match(r"^(splines|bilineal|knn)\/(.*)_test_([0-9]+)_?([0-9]+)?\.csv$", fname)

    method = matches.group(1).strip()
    name = matches.group(2).strip()

    k = int(matches.group(3))

    try:
        bloque = int(matches.group(4))
    except:
        bloque = -1

    if name not in originales:
        print 'La imagen', name, 'no esta dentro de los archivos para contrastar'
        continue

    with open(fname, 'r') as handle:
        data = handle.read()

    matriz = fromSTRToNumpy(data)

    ecmR = ecm(originales[name], matriz)
    psnrR = psnr(originales[name], matriz)

    output = {
        'name': name,
        'method': method,
        'k': k,
        'bloques': bloque,
        'ecm': ecmR,
        'psnr': psnrR
    }

    if os.path.isfile(fname + '.stats'):
        with open(fname + '.stats', 'r') as handle:
            stats = handle.read()

        for line in stats.split('\n'):
            if line == '':
                continue

            line = line.strip().split('\t\t\t')
            measures = np.fromstring(line[1].strip(), sep=' ')

            output[line[0] + ' Mean'] = np.mean(measures)
            output[line[0] + ' Std'] = np.std(measures)
            output[line[0]] = str(list(measures))[1:-1]

    dataset.append(output)

df = pd.DataFrame(dataset)
df.to_csv('output.csv', index=False)
