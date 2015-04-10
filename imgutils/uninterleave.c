#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
   unsigned int width, height;

   if (argc != 5) {
      printf("Usage: %s file-in file-out width height\n", argv[0]);
      return -1;
   }

   if (1 != sscanf(argv[3], "%d", &width)) {
      fprintf(stderr, "Couldn't read width: %s\n", argv[3]);
      exit(-1);
   }

   if (1 != sscanf(argv[4], "%d", &height)) {
      fprintf(stderr, "Couldn't read height: %s\n", argv[4]);
      exit(-1);
   }

   FILE *fr = fopen(argv[1], "r");
   unsigned char *yuv = calloc(width*height*2, sizeof(unsigned char));
   unsigned char *y = calloc(width*height, sizeof(unsigned char));

   fread(yuv, width*height*2, sizeof(unsigned char), fr);
   fclose(fr);

   int i;
   int c,r;
   for (r = 0; r < height; r++) {
      for (c = 0; c < width; c++) {
         y[r*width+c] = yuv[2*width*r+2*c];
      }
   }

   FILE *fw = fopen(argv[2], "w");
   fwrite(y, width*height, sizeof(unsigned char), fw);
   fclose(fw);

   free(yuv); free(y);

   return 0;
}
