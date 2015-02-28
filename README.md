GPU-Accelerated Painterly Rendering
===================================

Max Smolens  
UNC-Chapel Hill  
COMP 238: Advanced Image Generation  
Fall 2004  

## Overview

This project implements Aaron Hertzmann's [Painterly Rendering with Curved Brush Strokes of Multiple Sizes](http://www.mrl.nyu.edu/publications/painterly98/hertzmann-siggraph98.pdf)"  with the image processing operations performed on the GPU. The painterly rendering algorithm produces images that appear hand-painted. The algorithm builds the painting up in layers, with each layer corresponding to a single brush size. The algorithm performs blurring, edge detection, difference, and averaging operations on images when rendering each layer. All these steps are 2D convolution operations in which a kernel or multiple kernels are applied to each pixel in an image. Such repetitive calculations are suitable for GPU implementations, because of the hardware's massive parallelism.

See http://msmolens.github.io/painterly-rendering/ for complete implementation details, analysis, and results.

## Examples

### Colorist wash:
![Colorist wash rendering of tomatoes](/doc/imp2.png?raw=true)

### Impressionism:
![Impressionist rendering of billard table](/doc/wash1.png?raw=true)
