#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BYTES_PER_PIXEL 3

typedef struct { uint8_t red, green, blue; }                    ppm_pixel;
typedef struct { int width, height, depth; ppm_pixel *buffer; } ppm_image;

ppm_image *ppm_decode(const char *path)
{
     FILE *file;
     ppm_image *image;

     int c;
     char *header;

     file = fopen(path, "rb");

     // check the validity of the file path
     if (!file) 
     {
          fprintf(stderr, "Error : Unable to open file '%s'\n", path);
     }

     // read the file header
     fgets(header, sizeof(header), file);

     // check for the "magic number" (file format)
     if (header[0] != 'P' || header[1] != '6')
     {
          fprintf(stderr, "Error : '%s' has invalid image format (only PPM P6 is supported)\n");
     }
     
     // skip the notes
     while ((c = getc(file)) != EOF)
     {
          if (c == '#')
          {
               // if the character is part of the note,
               // skip the entire line
               while ((c = getc(file)) != '\n' && c != EOF);
          }
          else
          {
               // if the character is not part of the note,
               // put it back to the file stream
               ungetc(c, file); break;
          }
     }

     // allocate memory for the image
     image = (ppm_image *) malloc(sizeof(ppm_image));

     // read & validate image size
     if (fscanf(file, "%d %d", &image->width, &image->height) != 2)
     {
          fprintf(stderr, "Error : '%s' has invalid image size\n", path);
     }

     // read & validate image color depth
     if (fscanf(file, "%d", &image->depth) != 1)
     {
          fprintf(stderr, "Error : '%s' has invalid color depth\n", path);
     }

     while (fgetc(file) != '\n');
     
     // allocate memory for the pixel data
     image->buffer = (ppm_pixel *) malloc(image->width * image->height * sizeof(ppm_pixel));

     // read pixel data from the file
     fread(image->buffer, BYTES_PER_PIXEL, image->width * image->height, file);

     fclose(file);

     return image;
}

void ppm_encode(const char *path, ppm_image *output)
{
     FILE *file;

     file = fopen(path, "wb");

     // check the validity of the file path
     if (!file)
     {
          fprintf(stderr, "Error : Unable to open file '%s'\n", path);
     }

     // write image format
     fprintf(file, "P6\n");

     // write image size
     fprintf(file, "%d %d\n", output->width, output->height);
     
     // write image color depth
     fprintf(file, "%d\n", output->depth);

     // write image data to the file
     fwrite(output->buffer, BYTES_PER_PIXEL, output->width * output->height, file);
     
     fclose(file);
}