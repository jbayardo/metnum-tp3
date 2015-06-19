import numpy as np
import Image
import matplotlib.pyplot as pyplot
import os
import sys
import re
from os.path import basename
from numpy import mean
import shutil
import commands
import subprocess

origfolder = "./origs/"

def main():
	imgs = getfiles(origfolder)
	print imgs
	for item in imgs:
		imageFile = origfolder + item
		testsetname = item.split('.')[0]
		directory = "./testimgs/TESTSET-" + testsetname + "/"
		if not os.path.exists(directory):
		    os.makedirs(directory)
		img = Image.open(imageFile).convert('L')
		print img.size
		x0, y0 = img.size
		x = x0
		y = y0
		img.save(directory + "ORIGtest-" + testsetname + "-" + str(x) + "-" + str(y) + ".bmp")
		np.savetxt(directory + "ORIGtest-" + testsetname + "-" + str(x) + "-" + str(y) + ".csv" , np.array(img,np.uint8), delimiter=",", fmt="%u")
		while min(x,y) > min(x0 * 0.1, y0 * 0.1):
			x /= 2
			y /= 2
			filename = directory + "test-" + testsetname + "-" + str(x) + "-" + str(y)
			np.savetxt(filename + ".csv" , np.array(img.resize((x,y),Image.NEAREST),np.uint8), delimiter=",", fmt="%u")

			csv = np.genfromtxt (filename + ".csv", delimiter=",", dtype=np.uint8)
			Image.fromarray(csv, 'L').save(filename + ".bmp")

def getfiles(dir):
	res = []
	for filen in os.listdir(dir):
		print filen
		match = re.search(r'^\w+\.tif$', filen)
		if match:
			res.append(filen)

	return sorted(res)  

if __name__ == '__main__':
	main()