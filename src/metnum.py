#!/usr/bin/python2.7

from scripts.fabricate import *
from scripts.settings import *
from scripts.utils import listfiles
from sys import argv
import shutil

# Acciones
def build():
  compile()
  link()
  shutil.copyfile("./tp", "../experimentacion/tp")

def compile():
  for source in sources:
    run(compiler, '-c', '-std=c++11', '-O3', '-ffast-math', '-ggdb', source+'.cpp', '-o', source+'.o')

def link():
  objects = [s+'.o' for s in sources]
  run(compiler, '-o', executable, objects)

def clean():
  autoclean()

def test():
  build()
  import unittest
  unittest.main(module='scripts.tptests', exit=False, argv=argv[:1], verbosity=3)

main()
