#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void gatherPlane(unsigned char *plane, unsigned char *inter, int w, int h) {
   int x,y;
   for (y = 0; y < h; y++) {
      for (x = 0; x < w * 3; x += 3) {
         inter[y * w * 3 + x] = plane[y * w + x / 3];
      }
   }
}

int main(int argc, char **argv) {
   FILE *fp, *fw;
   int w;
   int h;

   if (argc != 5 && argc != 6) {
      printf("Usage: %s filein fileout width height\n", argv[0]);
      printf("Transforms fileout (where fileout contains rgb data plane by plane) to interlaced with ppm header.\n");
      return -1;
   }

   if (1 != sscanf(argv[3], "%d", &w)) {
      printf("Error reading width\n");
      return -1;
   }
   if (1 != sscanf(argv[4], "%d", &h)) {
      printf("Error reading height\n");
      return -1;
   }

   unsigned char *data = (unsigned char *)malloc(w * h * 3);
   unsigned char *reorg_data = (unsigned char *)malloc(w * h * 3);

   fp = fopen(argv[1], "r");
   if (!fp) {
      perror("Error opening file\n");
      return -1;
   }

   if (w * h * 3 != fread(data, sizeof(unsigned char), w * h * 3, fp)) {
      perror("Error reading data\n");
      fclose(fp);
      return -1;
   }

   fw = fopen(argv[2], "w");
   if (!fw) {
      perror("Error opening file write\n");
      fclose(fp);
      return -1;
   }

   gatherPlane(&data[0], &reorg_data[0], w, h);
   fprintf(stdout, "Plane 1 gathered\n");
   gatherPlane(&data[w * h], &reorg_data[1], w, h);
   fprintf(stdout, "Plane 2 gathered\n");
   gatherPlane(&data[w * h * 2], &reorg_data[2], w, h);
   fprintf(stdout, "Plane 3 gathered\n");

   fprintf(fw, "P6\n%d %d\n255\n", w, h);

   fwrite(reorg_data, sizeof(unsigned char), w * h * 3, fw);

   fclose(fp);
   fclose(fw);
   free(data);
   free(reorg_data);

   return 0;
}

