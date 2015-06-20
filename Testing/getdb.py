import sys
import re
import os
from os.path import basename
from numpy import mean
import shutil
import commands
import subprocess
import numpy as np
import Image
import math
import matplotlib.pyplot as pyplot

NEAREST = 0
BILINEAL = 1
CUBICO = 2
testsets = ['grid', 'pattern1', 'pattern2', 'sincos']
testimgsfoldersub = './testimgs/TESTSET-'
testimgsfoldname = 'TESTSET-'

outpdir = "./testresults/"

def main():
  if not os.path.exists(outpdir):
    os.makedirs(outpdir)
  for testset in testsets:
    processknn(testset)
    processbilinear(testset)

def trimmean(arr, percent):
    n = len(arr)
    k = int(round(n*(float(percent)/100)/2))
    return mean(arr[k+1:n-k])

def processknn(testset):
    print "testset:", testset ######################################3
    
    ou = open(outpdir + "data-knn-" + testset + ".csv", "w")
    ou.write("imgset, ancho, altura, tipo de zoom, k, tamaniobloque, tiempo de computo, errorECM, errorPSNR" + "\n")
    listfile = getfiles(testimgsfoldersub + testset + "/")

    origfname = getorig(testimgsfoldersub + testset + "/")
    origdata = origfname.split('-')
    origw = int(origdata[2])
    origh = int(origdata[3].split('.')[0])
    origset = origdata[1]
    origfile = testimgsfoldersub + testset + "/" + origfname

    
    print "origdata",origdata ######################################3
    
    outdirset = outpdir + testimgsfoldname + origset + "/"
    if not os.path.exists(outdirset):
      os.makedirs(outdirset)
    
    print "outdirset:", outdirset ######################################3
    
    #Para cada imagen de prueba
    for filen in listfile:
      imgdata = filen.split('-')
      print "    ", "filen:",filen, "imgdata:", imgdata, ######################################3

      imgw = int(imgdata[2])
      imgh = int(imgdata[3].split('.')[0])
      imgset = imgdata[1]

      if imgset != origset: continue
      kw = (origw - imgw) / imgw
      kh = (origh - imgh) / imgh
      if kw != kh: continue
      print "kw:", str(kw), "kh:" , str(kh), "OutName:", "testRes-knn-" + testset + "-" + str(imgw) + "-" + str(imgh) ######################################3

      outputfile = outdirset + "testRes-knn-" + testset + "-" + str(imgw) + "-" + str(imgh) 

      outputTP = subprocess.check_output(["./tp", testimgsfoldersub + testset + "/" + filen, outputfile + ".csv" ,  str(imgh), str(imgw), str(kw) , str(0)]).splitlines()
      time = outputTP[len(outputTP)-1].split('.')[1]
      err1 = errorECM(origfile, outputfile + ".csv", origh, origw)
      err2 = errorPSNR(origfile, outputfile + ".csv", origh, origw)
      #csv = np.genfromtxt (outputfile + ".csv", delimiter=",", dtype=np.uint8)
      #Image.fromarray(csv, 'L').save(outputfile + ".bmp")
      
      ou.write(imgset + ',' + str(imgw) + ',' + str(imgh) + ', knn,' + str(kw) + ', ,' + str(time) + ',' + str(err1) + ',' + str(err2) +"\n")

    ou.close()

def processbilinear(testset):
    print "testset:", testset ######################################3
    
    ou = open(outpdir + "data-bilineal-" + testset + ".csv", "w")
    ou.write("imgset, ancho, altura, tipo de zoom, k, tamaniobloque, tiempo de computo, errorECM, errorPSNR" + "\n")
    listfile = getfiles(testimgsfoldersub + testset + "/")

    origfname = getorig(testimgsfoldersub + testset + "/")
    origdata = origfname.split('-')
    origw = int(origdata[2])
    origh = int(origdata[3].split('.')[0])
    origset = origdata[1]
    origfile = testimgsfoldersub + testset + "/" + origfname

    print "origdata",origdata ######################################3
    
    outdirset = outpdir + testimgsfoldname + origset + "/"
    if not os.path.exists(outdirset):
      os.makedirs(outdirset)
    
    print "outdirset:", outdirset ######################################3
    
    #Para cada imagen de prueba
    for filen in listfile:
      imgdata = filen.split('-')
      print "    ", "filen:",filen, "imgdata:", imgdata, ######################################3

      imgw = int(imgdata[2])
      imgh = int(imgdata[3].split('.')[0])
      imgset = imgdata[1]

      if imgset != origset: continue
      kw = (origw - imgw) / imgw
      kh = (origh - imgh) / imgh
      if kw != kh: continue
      print "kw:", str(kw), "kh:" , str(kh), "OutName:", "testRes-bilineal-" + testset + "-" + str(imgw) + "-" + str(imgh) ######################################3

      outputfile = outdirset + "testRes-bilineal-" + testset + "-" + str(imgw) + "-" + str(imgh) 

      outputTP = subprocess.check_output(["./tp", testimgsfoldersub + testset + "/" + filen, outputfile + ".csv" ,  str(imgh), str(imgw), str(kw) , str(1)]).splitlines()
      time = outputTP[len(outputTP)-1].split('.')[1]
      err1 = errorECM(origfile, outputfile + ".csv", origh, origw)
      err2 = errorPSNR(origfile, outputfile + ".csv", origh, origw)
      #csv = np.genfromtxt (outputfile + ".csv", delimiter=",", dtype=np.uint8)
      #Image.fromarray(csv, 'L').save(outputfile + ".bmp")
      
      ou.write(imgset + ',' + str(imgw) + ',' + str(imgh) + ', bilineal,' + str(kw) + ', ,' + str(time) + ',' + str(err1) + ',' + str(err2) +"\n")
    ou.close()

def processcubic(testset):
    print "testset:", testset ######################################3
    
    ou = open(outpdir + "data-cubic-" + testset + ".csv", "w")
    ou.write("imgset, ancho, altura, tipo de zoom, k, tamaniobloque, tiempo de computo, errorECM, errorPSNR" + "\n")
    listfile = getfiles(testimgsfoldersub + testset + "/")

    origfname = getorig(testimgsfoldersub + testset + "/")
    origdata = origfname.split('-')
    origw = int(origdata[2])
    origh = int(origdata[3].split('.')[0])
    origset = origdata[1]
    origfile = testimgsfoldersub + testset + "/" + origfname

    print "origdata",origdata ######################################3
    
    outdirset = outpdir + testimgsfoldname + origset + "/"
    if not os.path.exists(outdirset):
      os.makedirs(outdirset)
    
    print "outdirset:", outdirset ######################################3
    
    #Para cada imagen de prueba
    for filen in listfile:
      imgdata = filen.split('-')
      print "    ", "filen:",filen, "imgdata:", imgdata, ######################################3

      imgw = int(imgdata[2])
      imgh = int(imgdata[3].split('.')[0])
      imgset = imgdata[1]

      if imgset != origset: continue
      kw = (origw - imgw) / imgw
      kh = (origh - imgh) / imgh
      if kw != kh: continue
      
      #Setear que tamanios de bloques se pueden usar
      for b in blocksizes:
        print "kw:", str(kw), "kh:" , str(kh), "OutName:", "testRes-cubic-" + testset + "-" + str(imgw) + "-" + str(imgh) ######################################3

        outputfile = outdirset + "testRes-cubic-" + testset + "-" + str(imgw) + "-" + str(imgh) + "-" + str(b)

        outputTP = subprocess.check_output(["./tp", testimgsfoldersub + testset + "/" + filen, outputfile + ".csv" ,  str(imgh), str(imgw), str(kw) , str(1), str(b)]).splitlines()
        time = outputTP[len(outputTP)-1].split('.')[1]
        err1 = errorECM(origfile, outputfile + ".csv", origh, origw)
        err2 = errorPSNR(origfile, outputfile + ".csv", origh, origw)
        #csv = np.genfromtxt (outputfile + ".csv", delimiter=",", dtype=np.uint8)
        #Image.fromarray(csv, 'L').save(outputfile + ".bmp")
        
        ou.write(imgset + ',' + str(imgw) + ',' + str(imgh) + ', cubic ,' + str(kw) + ',' + str(b) + ',' + str(time) + ',' + str(err1) + ',' + str(err2) +"\n")

    ou.close()

def getfiles(dir):
  res = []
  for filen in os.listdir(dir):
    match = re.search(r'^test-\w+-\d+-\d+\.csv$', filen)
    if match:
      res.append(filen)

  return sorted(res)  

def errorECM(origimg, processedimg, m, n):
  res = 0
  orig = np.genfromtxt(origimg, delimiter=",", dtype=np.uint8)
  new = np.genfromtxt(processedimg, delimiter=",", dtype=np.uint8)
  print orig.shape, new.shape, m, n
  for i in range(0,m):
    for j in range(0,n):
      res += math.pow(abs(orig[i][j]-new[i][j]),2)
  res /= m*n
  return res

def errorPSNR(origimg, processedimg, m, n):
  res = 10*(math.log10(((math.pow(255, 2))/errorECM(origimg, processedimg, m, n))))
  return res

def getorig(dir):
  for filen in os.listdir(dir):
    match = re.search(r'^ORIGtest-\w+-\d+-\d+\.csv$', filen)
    if match:
      res = filen
  return res 

if __name__ == '__main__':
  main()