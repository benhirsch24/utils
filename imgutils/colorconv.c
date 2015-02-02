#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void rgb888toyuv444(unsigned char *inbuf, unsigned char *outbuf, int width, int height)
{
   int row,col;

   float y,u,v;
   float r,g,b;

   for (row = 0; row < height; row++) {
      for (col = 0; col < width; col++) {
         r = inbuf[3*row*width + 3*col];
         g = inbuf[3*row*width + 3*col+1];
         b = inbuf[3*row*width + 3*col+2];

         y = 0.299f * r + 0.587f * g + 0.114f * b;
         u = 128.0f + -0.169f * r + -0.331f * g +  0.5f   * b;
         v = 128.0f +  0.5f   * r + -0.419f * b + -0.081f * b;

         outbuf[3*row*width + 3*col] = (unsigned char)y;
         outbuf[3*row*width + 3*col+1] = (unsigned char)u;
         outbuf[3*row*width + 3*col+2] = (unsigned char)v;
      }
   }
}

void yuv444torgb888(unsigned char *inbuf, unsigned char *outbuf, int width, int height)
{
   int row,col;

   float y,u,v;
   float r,g,b;

   for (row = 0; row < height; row++) {
      for (col = 0; col < width; col++) {
         y = inbuf[3*row*width + 3*col];
         u = inbuf[3*row*width + 3*col+1];
         v = inbuf[3*row*width + 3*col+2];

         u -= 128.0f; v -= 128.0f;

         r = y + 1.4f * v;
         g = y + -0.343f * u + -0.711 * v;
         b = y + 1.765f * u;

         outbuf[3*row*width + 3*col] = (unsigned char)r;
         outbuf[3*row*width + 3*col+1] = (unsigned char)g;
         outbuf[3*row*width + 3*col+2] = (unsigned char)b;
      }
   }
}

int main(int argc, char **argv) {
   int width, height, colorconv;
   unsigned char *inbuf, *outbuf;

   if (argc != 6) 
   {
      printf("Usage: %s filein fileout width height color-conv\n", argv[0]);
      printf("Where color-conv is one of:\n[1] RGB888 to YUV444 (both interlaced)\n[2] YUV444 to RGB888 (both interlaced)\n");
      exit(EXIT_FAILURE);
   }

   if (1 != sscanf(argv[3], "%d", &width)) {
      fprintf(stderr, "Error reading width: %s\n", argv[3]);
      exit(EXIT_FAILURE);
   }

   if (1 != sscanf(argv[4], "%d", &height)) {
      fprintf(stderr, "Error reading height: %s\n", argv[4]);
      exit(EXIT_FAILURE);
   }

   if (1 != sscanf(argv[5], "%d", &colorconv)) {
      fprintf(stderr, "Error reading color-conv: %s\n", argv[5]);
      exit(EXIT_FAILURE);
   }

   switch (colorconv) {
      case 1: case 2: break;
      default:
          fprintf(stderr, "Error: color-conv must be one of [1, 2] (is %d)\n", colorconv);
          exit(EXIT_FAILURE);
          break;
   }

   FILE *fin = fopen(argv[1], "r");
   if (fin == NULL) {
      perror("Error opening in file");
      exit(EXIT_FAILURE);
   }

   inbuf = (unsigned char *)calloc(width*height*3, sizeof(unsigned char));

   if (width*height*3 != fread(inbuf, sizeof(unsigned char), width*height*3, fin)) {
      perror("Error reading from in file");
      exit(EXIT_FAILURE);
   }

   fclose(fin);

   outbuf = (unsigned char *)calloc(width*height*3, sizeof(unsigned char));

   switch (colorconv) {
      case 1:
         rgb888toyuv444(inbuf, outbuf, width, height);
         break;
      case 2:
         yuv444torgb888(inbuf, outbuf, width, height);
         break;
   }

   FILE *fout = fopen(argv[2], "w");
   if (fout == NULL) {
      perror("Error opening out file");
      exit(EXIT_FAILURE);
   }

   if (width*height*3 != fwrite(outbuf, sizeof(unsigned char), width*height*3, fout)) {
      perror("Error writing to out file");
      exit(EXIT_FAILURE);
   }

   fclose(fout);

   free(outbuf);
   free(inbuf);

   printf("Done!\n");

   return 0;
}
