import sys
import re
import os
from os.path import basename
from numpy import mean
import shutil
import commands
import subprocess

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

def trimmean(arr, percent):
    n = len(arr)
    k = int(round(n*(float(percent)/100)/2))
    return mean(arr[k+1:n-k])

def processblur(testset):
    ou = open(outpdir + "blur-"+ testset + ".csv", "w")
    listfile = getfiles(testimgsfoldersub + testset + "/")
    done = 0
    #Para cada imagen de prueba
    for filen in listfile:
      done++
      imgdata = filen.split('-')

      imgw = imgdata[2]
      imgh = imgdata[3].split('.')[0]
      imgset = imgdata[1]



      time = int(subprocess.check_output(["./tp", testimgsfoldersub + testset + "/" + filen, "n", str(0)]))
      
      ou.write(imgset + ',' + imgw + ',' imgh + ', knn' + ',' + time + "\n")

      print("knn[]: TESTSET=" + testset + " IMG=" + filen)
    ou.close()

def getfiles(dir):
  res = []
  for filen in os.listdir(dir):
    match = re.search(r'^test-\w+-\d+-\d+\.csv$', filen)
    if match:
      res.append(filen)

  return sorted(res)  

def getorig(dir):
  for filen in os.listdir(dir):
    match = re.search(r'^ORIGtest-\w+-\d+-\d+\.csv$', filen)
    if match:
      res = filen
  return res 

if __name__ == '__main__':
  main()