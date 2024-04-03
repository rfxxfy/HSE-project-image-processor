## Image Filters (image_processor)

This project implements a console application that allows applying various filters to images, 
similar to filters found in popular graphic editors. 

## Supported Image Format

Input and output image files must be in the [BMP](http://en.wikipedia.org/wiki/BMP_file_format) format.

The BMP format supports various variations, but this task will use
a 24-bit BMP without compression and without a color table. The type of used  `DIB header` - `BITMAPINFOHEADER`.

## Command Line Arguments Format 

Description of the command line arguments format: 

`{program name} {input file path} {output file path}
[-{filter name 1} [filter parameter 1] [filter parameter 2] ...]
[-{filter name 2} [filter parameter 1] [filter parameter 2] ...] ...`

When executed without arguments, the program displays the help message.

### Example
`./image_processor input.bmp /tmp/output.bmp -crop 800 600 -gs -blur 0.5`

In this example:
1. An image is loaded from the file `input.bmp `
2. It is cropped to an image starting from the top left corner with dimensions of 800x600 pixels 
3. Converted to grayscale 
4. A blur with sigma 0.5 is applied 
5. The resulting image is saved to the file `/tmp/output.bmp`

The list of filters can be empty, in which case the image is saved as is.
Filters are applied in the order they are listed in the command line arguments. 

## Filters

In the formulas below, each color component 
is represented by a floating-point number from 0 to 1. Pixel colors 
are represented by triples `(R, G, B)`. Thus, `(0, 0, 0)` represents black, 
and `(1, 1, 1)` represents white. 
 
If a filter is specified by a matrix, it means that the value of each color is determined by a weighted sum
of the values of that color in neighboring pixels according to the matrix. The target pixel 
corresponds to the central element of the matrix. 
 
For example, for a filter specified by the matrix: 

![encoding](https://latex.codecogs.com/svg.image?%5Cbegin%7Bbmatrix%7D1%20&%202%20&%203%20%5C%5C4%20&%205%20&%206%20%5C%5C7%20&%208%20&%209%20%5C%5C%5Cend%7Bbmatrix%7D)

The value of each color of the target pixel `C[x][y]` is determined by the formula: 
```
C[x][y] =
  min(1, max(0,
   1*C[x-1][y-1] + 2*C[x][y-1] + 3*C[x+1][y-1] +
   4*C[x-1][y]   + 5*C[x][y]   + 6*C[x+1][y]   +
   7*C[x-1][y+1] + 8*C[x][y+1] + 9*C[x+1][y+1]
))
```

When processing pixels close to the edge of the image, part of the matrix may extend beyond the image boundary. 
In this case, the value of the pixel that extends beyond the boundary is taken as the value of the 
nearest pixel of the image. 

### List of Implemented Filters

#### Crop (-crop width height)
Crops the image to the specified width and height. Uses the upper left part of the image. 
 
If the requested width or height exceeds the dimensions of the original image, the available part of the image is output. 

#### Grayscale (-gs)
Converts the image to grayscale using the formula:

![encoding](https://latex.codecogs.com/svg.image?R'%20=%20G'%20=%20B'%20=0.299%20R%20&plus;%200%20.587%20G%20&plus;%200%20.%20114%20B)

#### Negative (-neg)
Converts the image to a negative using the formula:

![encoding](https://latex.codecogs.com/svg.image?R'%20=%201%20-%20R,%20G'%20=%201%20-%20G,%20B'%20=%201%20-%20B)

#### Sharpening (-sharp)
Increases sharpness using the matrix:

![encoding](https://latex.codecogs.com/svg.image?%5Cbegin%7Bbmatrix%7D%20&%20-1%20&%20%20%5C%5C-1%20&%205%20&%20-1%20%5C%5C%20&%20-1%20&%20%20%5C%5C%5Cend%7Bbmatrix%7D)

#### Edge Detection (-edge threshold)
Detects edges. Converts the image to grayscale and applies the matrix:

![encoding](https://latex.codecogs.com/svg.image?%5Cbegin%7Bbmatrix%7D%20&%20-1%20&%20%20%5C%5C-1%20&%204%20&%20-1%20%5C%5C%20&%20-1%20&%20%20%5C%5C%5Cend%7Bbmatrix%7D)

Pixels with values exceeding `threshold` are colored white, the rest are black.

#### Gaussian Blur (-blur sigma)
[Гауссово размытие](https://ru.wikipedia.org/wiki/Размытие_по_Гауссу),
Applies Gaussian blur, with the parameter being sigma. 

The value of each color of the pixel `C[x0][y0]` is determined by the formula: 

![encoding](https://latex.codecogs.com/svg.image?C%5Bx_0%5D%5By_0%5D%20%3D%20%5Csum_%7Bx%3D0%2Cy%3D0%7D%5E%7Bwidth-1%2C%20height-1%7DC%5Bx%5D%5By%5D%5Cfrac%7B1%7D%7B2%5Cpi%5Csigma%5E2%7De%5E%7B-%5Cfrac%7B%5Cleft%7Cx_o-x%5Cright%7C%5E2%20%26plus%3B%20%5Cleft%7Cy_o-y%5Cright%7C%5E2%7D%7B2%5Csigma%5E2%7D%7D)

There are various implementations and optimizations for calculating this filter, described on Wikipedia. 

#### Smoothing (-cm radius)
Smoothes the image according to the specified radius. The larger the radius value, the stronger the effect is applied. 


## Implementation

For reading and writing files [std::ifstream](https://en.cppreference.com/w/cpp/io/basic_ifstream) и [std::ofstream](https://en.cppreference.com/w/cpp/io/basic_ofstream) are used.

The following components are implemented: 
- A class representing the image and providing operations on it 
- Classes for reading and writing BMP format 
- Filters 
- Controller managing sequential application of filters
