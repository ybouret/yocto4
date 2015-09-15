/*
	Encode an RLE buffer
	Return the length in the output buffer
	Note: this is not my code, it was handed to me as is.
	If you are the author or know the author please let me know
	so I can give appropriate credit.
*/
int RLECompress(unsigned char *output,unsigned char *input,int length)
{
   int count = 0,index,i;
   unsigned char pixel;
   int out = 0;

   while (count < length) {
      index = count;
      pixel = input[index++];
      while (index < length && index - count < 127 && input[index] == pixel)
         index++;
      if (index - count == 1) {
         /* 
            Failed to "replicate" the current pixel. See how many to copy.
            Avoid a replicate run of only 2-pixels after a literal run. There
            is no gain in this, and there is a risK of loss if the run after
            the two identical pixels is another literal run. So search for
            3 identical pixels.
         */
         while (index < length && index - count < 127
               && (input[index] != input[index-1]
               || index > 1 && input[index] != input[index-2]))
            index++;
         /* 
            Check why this run stopped. If it found two identical pixels, reset
            the index so we can add a run. Do this twice: the previous run
            tried to detect a replicate run of at least 3 pixels. So we may be
            able to back up two pixels if such a replicate run was found.
         */
         while (index < length && input[index] == input[index-1])
            index--;
         output[out++] = (unsigned char)(count - index);
         for (i=count;i<index;i++)
            output[out++] = input[i];
      } else {
         output[out++] = (unsigned char)(index - count);
         output[out++] = pixel;
      } /* if */
      count=index;
   } /* while */
   return(out);
}

/*
	Uncompress a RLE buffer
	This is of little use since the size of the output buffer
	isn't known. However this can be used as the basis for ones
	own decoder.
*/
void RLEUncompress(unsigned char *output,unsigned char *input,int length)
{
   signed char count;

   while (length > 0) {
      count = (signed char)*input++;
      if (count > 0) {                      /* replicate run */
         memset(output,*input++,count);
      } else if (count < 0) {               /* literal run */
         count = (signed char)-count;
         memcpy(output,input,count);
         input += count;
      } /* if */
      output += count;
      length -= count;
   } /* while */
}

