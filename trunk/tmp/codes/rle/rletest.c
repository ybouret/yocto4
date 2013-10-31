#include "stdio.h"
#include "stdlib.h"

/*
   Crappy little test program for testing RLE code
   Read 100x100 pixel greyscale images
   Write them as RLE
   Decompress them back to their originals
*/

int Nx = 100;
int Ny = 100;

int main(int argc,char **argv)
{
   FILE *fptr,*fout;
   char foutname[128],finname[128];
   unsigned char *inbuffer,*outbuffer;
   int len,i,j,c;
   signed char count;

   /* Open the input file */
   if ((fptr = fopen(argv[1],"r")) == NULL) {
      fprintf(stderr,"Failed to read file\n");
      exit(-1);
   }

   /* Open the output file */
   sprintf(foutname,"%s.rle",argv[1]);
   if ((fout = fopen(foutname,"w")) == NULL) {
      fprintf(stderr,"Failed to open output file\n");
      exit(-1);
   }

   /* Malloc space for the buffers */
   if ((inbuffer = malloc(Nx*sizeof(unsigned char))) == NULL) {
      fprintf(stderr,"malloc failed for input buffer!\n");
      exit(-1);
   }
   if ((outbuffer = malloc((Nx+1+Nx/128)*sizeof(unsigned char))) == NULL) {
      fprintf(stderr,"malloc failed for output buffer!\n");
      exit(-1);
   }

   /* Compress the file, scan line at a time */
   for (j=0;j<Ny;j++) {

      /* Read one row */
      for (i=0;i<Nx;i++) {
         if ((c = fgetc(fptr)) == EOF) {
            fprintf(stderr,"Unexpected end of file\n");
            exit(-1);
         }
         inbuffer[i] = c;
      }
      
      /* Compress it */
      len = RLECompress(outbuffer,inbuffer,Nx);

      /* Write it to a file */
      if (fwrite(outbuffer,sizeof(unsigned char),len,fout) != len) {
         fprintf(stderr,"Failed to write output for row %d\n",j);
         exit(-1);
      }

      fprintf(stderr,"Row: %4d Length: %4d\n",j,len);
   }
   fclose(fptr);
   fclose(fout);

   /* Reinstate the files */
   strcpy(finname,foutname);
   if ((fptr = fopen(finname,"r")) == NULL) {
      fprintf(stderr,"Failed to read input compressed file\n");
      exit(-1);
   }
   sprintf(foutname,"%s.unrle",argv[1]);
   if ((fout = fopen(foutname,"w")) == NULL) {
      fprintf(stderr,"Failed to read output compressed file\n");
      exit(-1);
   }
   len = 0;
   while ((c = fgetc(fptr)) != EOF) {
      count = (signed char)c;
      if (count > 0) {
         len += count;
         c = fgetc(fptr);
         fputc(c,fout);
         for (i=1;i<count;i++) 
            fputc(c,fout);
         fprintf(stderr,"Replicate run of length %3d\n",count);
      } else if (count < 0) {
         count = -count;
         len += count;
         for (i=0;i<count;i++) {
            c = fgetc(fptr);
            fputc(c,fout);
         }
         fprintf(stderr,"Literal   run of length %3d\n",count);
      }
   }
   fprintf(stderr,"Total length = %d, should be %d\n",len,Nx*Ny);
   fclose(fptr);
}

#include "rle.c"

