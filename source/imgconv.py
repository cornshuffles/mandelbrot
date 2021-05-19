#!/bin/python3

from PIL import Image

im = Image.open("./mandelbrot.ppm")
im.save("brot.png")
