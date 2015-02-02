#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) 
{
   int width, height, header;

   if (argc != 6)
   {
      printf("Usage: %s filein fileout width height header-options\n", argv[0]);
      printf("Where header-options is one of:\n[1] Add header to RGB file\n[2] Strip header from RGB file\n");
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

   if (1 != sscanf(argv[5], "%d", &header)) {
      fprintf(stderr, "Error reading header: %s\n", argv[5]);
      exit(EXIT_FAILURE);
   }

   switch (header) {
      case 1: case 2: break;
      default: 
          fprintf(stderr, "Header should be one of [1, 2] is %d\n", header); 
          exit(EXIT_FAILURE);
   }

   FILE *fin = fopen(argv[1], "r");
   if (fin == NULL) {
      perror("Error opening in file");
      exit(EXIT_FAILURE);
   }

   // check to see if input file is ppm format
   // we know if it is length width*height*3 + the ppm header len
   char ppmheader[100];
   memset(ppmheader, 0, 100);
   int hlen = sprintf(ppmheader, "P6\n%d %d\n255\n", width, height);

   fseek(fin, 0, SEEK_END);
   long file_len = ftell(fin);
   fseek(fin, 0, SEEK_SET);

   if (header == 1 && file_len == width*height*3 + hlen) {
      fprintf(stderr, "Error: Specified \"Add Header to File\" but %s already contains a formatted ppm header\n", argv[1]);
      fclose(fin);
      exit(EXIT_FAILURE);
   }

   if (header == 2 && file_len == width*height*3) {
      fprintf(stderr, "Error: Specified \"Strip Header from File\" but %s doesn't have a formatted ppm header\n", argv[1]);
      fclose(fin);
      exit(EXIT_FAILURE);
   }

   // if strip, set fin to start of rgb data
   // (file already reset to start after getting length of file)
   if (header == 2) {
      fseek(fin, hlen, SEEK_SET);
   }

   unsigned char *inbuf = (unsigned char *)calloc(width*height*3, sizeof(unsigned char));

   if (width*height*3 != fread(inbuf, sizeof(unsigned char), width*height*3, fin)) {
      perror("Error reading from in file");
      exit(EXIT_FAILURE);
   }

   FILE *fout = fopen(argv[2], "w");
   if (fout == NULL) {
      perror("Error opening out file");
      exit(EXIT_FAILURE);
   }

   if (header == 1) {
      fwrite(ppmheader, sizeof(char), hlen, fout);
   }

   if (width*height*3 != fwrite(inbuf, sizeof(unsigned char), width*height*3, fout)) {
      perror("Error writing to out file");
      exit(EXIT_FAILURE);
   }

   printf("Done!\n");

   free(inbuf);
   fclose(fout);
   fclose(fin);

   return 0;
}
