import numpy as np
import os
import re
import math
import csv
import pickle


def fromSTRToNumpy(data):
    output = []

    for line in data.strip().split('\n'):
        output += map(lambda x: float(x.strip()), line.strip().split(','))

    return np.array(output)

def ecm(Iorig, Inew, width, height):
    ecmR = 0.0

    for i in xrange(0, height):
        for j in xrange(0, width):
            ecmR += math.pow(Iorig[i*width+j] - Inew[i*width+j], 2)
            #c /= float(width * height)
            #row.append(c)

        #m.append(row)

    ecmR /= float(width * height)
    return ecmR

# Este script larga output.csv, con el formato que buscamos para que lo tome
# el software para graficar, Tableau.

dataset = []

files = map(lambda x: 'splines/'+x, os.listdir('splines')) + map(lambda x:'bilineal/'+x, os.listdir('bilineal')) + map(lambda x:'knn/'+x, os.listdir('knn'))
contraste = map(lambda x: 'originales/'+x, os.listdir('originales'))
originales = {}

for fname in contraste:
    if fname[-3:] != 'csv' or fname[-8:] == '_ecm.csv':
        continue

    matches = re.match(r"^(originales)\/(.*)\.csv$", fname)
    name = matches.group(2).strip()

    with open(fname, 'r') as handle:
        data = handle.read()

    originales[name] = fromSTRToNumpy(data)

now = 0
total = len(files)
for fname in files:
    print now*100/total , "% | ",now, " of ", total
    now += 1
    if fname[-3:] != 'csv' or fname[-8:] == '_ecm.csv':
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

    #with open(fname, 'r') as handle:
     #   data = handle.read()

    #matriz = fromSTRToNumpy(data)

    ecmR = 0
    #ecm(originales[name], matriz, 2049, 2049)

    output = {
        'name': name,
        'method': method,
        'k': k,
        'bloques': bloque,
        'ecm': ecmR,
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

    #matches = re.match(r"(.*)\.csv$", fname)

    # with open(matches.group(1) + '_ecm.csv', 'w+') as handle:
    #     print >> handle, "X Y Value"
    #     for (x, y), value in np.ndenumerate(ecmM):
    #         print >> handle, x, y, '%.3e' % value


# myfile = open("aDataJoy-DATASET-splines.csv", 'wb')
# wr = csv.writer(myfile, quoting=csv.QUOTE_ALL)
# wr.writerow(dataset)
# df = pd.DataFrame(dataset)
# df.to_csv('output-bigimage.txt', index=False)
tt = open("dataset.dat", "w")
pickle.dump(dataset, tt)
tt.close()