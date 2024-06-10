# PPM File Encoder & Decoder
A simple header only file encoder & decoder for **PPM (Portable Pixel Map)** type **P6** image file formats.

## Example Usage
```c
#include "ppm.h"

int main(int argc, char *argv[])
{
    int x, y;
    int width, height, depth;

    // read the image data
    ppm_image *image = ppm_decode("input.ppm");

    // access the image attributes
    width  = image->width;
    height = image->height;
    depth  = image->depth;

    // access a specific image pixel
    ppm_pixel pixel = image->buffer[y * image->width + x];

    // write image data to a file
    ppm_encode("output.ppm", image);

    // free the memory allocated to the image
    free(image);

    return 0;
}
```

## References
- [PPM Format Specification](https://netpbm.sourceforge.net/doc/ppm.html)