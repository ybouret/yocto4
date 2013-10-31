#include "seriallib.h"

/*
	Attempt to open the serial port
	Return the file descriptor
*/
int OpenSerial(char *port)
{
	int fdes;

	if ((fdes = open(port,O_RDWR | O_NOCTTY | O_NONBLOCK)) < 0) {
		perror("OpenSerial");
	} else {
		fcntl(fdes,F_SETFL,FNDELAY);
	}

	return(fdes);
}

/*
	Setup the communication options
	Hopefully POSIX compliant
*/
int SetupSerial(int fdes,int baud,int databits,int stopbits,int parity)
{
	int n;
	struct termios options; //,options2;

	/* Get the current options */
	if (tcgetattr(fdes,&options) < 0) {
		perror("SetupSerial 1");
		return(FALSE);
	}
	/*
   fprintf(stderr,"iflag - 0%o; oflag - 0%o; cflag - 0%o; lflag - 0%o\n",
      (unsigned int)options.c_iflag,
      (unsigned int)options.c_oflag,
      (unsigned int)options.c_cflag,
      (unsigned int)options.c_lflag);
	*/
	
	/* Set the baud rate */
	switch (baud) {
   case 1200:
      n =  cfsetispeed(&options,B1200);
      n += cfsetospeed(&options,B1200);
      break;
   case 2400:
      n =  cfsetispeed(&options,B2400);
      n += cfsetospeed(&options,B2400);
      break;
	case 4800:
		n =  cfsetispeed(&options,B4800);
		n += cfsetospeed(&options,B4800);
		break;
	case 9600:
		n =  cfsetispeed(&options,B9600);
		n += cfsetospeed(&options,B9600);
      break;
   case 19200:
      n =  cfsetispeed(&options,B19200);
      n += cfsetospeed(&options,B19200);
      break;
   case 38400:
      n =  cfsetispeed(&options,B38400);
      n += cfsetospeed(&options,B38400);
      break;
   case 57600: // witilt
      n =  cfsetispeed(&options,B57600);
      n += cfsetospeed(&options,B57600);
      break;
	default:
		fprintf(stderr,"Unsupported baud rate\n");
		return(FALSE);
	}
	if (n != 0) {
		perror("SetupSerial 2");
		return(FALSE);
	}

   // Set the data size 
   options.c_cflag &= ~CSIZE; // Character size mask
   switch (databits) {
   case 7:
      options.c_cflag |= CS7;
      break;
   case 8:
      options.c_cflag |= CS8;
      break;
   default:
      fprintf(stderr,"Unsupported data size\n");
      return(FALSE);
   }

	// Set parity
	switch (parity) {
	case 'n':
	case 'N':
		options.c_cflag &= ~PARENB; // Clear parity enable
		break;
	case 'o':
	case 'O':
		options.c_cflag |= PARENB; // Parity enable
		options.c_cflag |= PARODD; // Enable odd parity 
		break;
	case 'e':
	case 'E':
		options.c_cflag |= PARENB; // Parity enable
		options.c_cflag &= ~PARODD; // Turn off odd parity = even
		break;
	default:
		fprintf(stderr,"Unsupported parity\n");
		return(FALSE);
	}

	// Set stop bits 
	switch (stopbits) {
	case 1:
		options.c_cflag &= ~CSTOPB; // Not 2 stop bits = One stop bit
		break;
	case 2:
		options.c_cflag |= CSTOPB; // Two stop bits
		break;
	default:
		fprintf(stderr,"Unsupported stop bits\n");
		return(FALSE);
	}

   // Non blocking return immediately with data
   options.c_cc[VMIN] = 0;
   options.c_cc[VTIME] = 0;

   // Local flags
   options.c_lflag = 0;  // No local flags
   options.c_lflag &= ~ICANON; // Don't canonicalise
   options.c_lflag &= ~ECHO; // Don't echo
   options.c_lflag &= ~ECHOK; // Don't echo

   // Control flags
   options.c_cflag &= ~CRTSCTS; // Disable RTS/CTS
   options.c_cflag |= CLOCAL; // Ignore status lines
   options.c_cflag |= CREAD; // Enable receiver
   options.c_cflag |= HUPCL; // Drop DTR on close

   // oflag - output processing
   options.c_oflag &= ~OPOST; // No output processing
   options.c_oflag &= ~ONLCR; // Don't convert linefeeds

   // iflag - input processing
   options.c_iflag |= IGNPAR; // Ignore parity
   options.c_iflag &= ~ISTRIP; // Don't strip high order bit
   options.c_iflag |= IGNBRK; // Ignore break conditions
   options.c_iflag &= ~INLCR; // Don't Map NL to CR
   options.c_iflag &= ~ICRNL; // Don't Map CR to NL
   options.c_iflag |= (IXON | IXOFF | IXANY); // xon/xoff flow control
	/*
   fprintf(stderr,"iflag - 0%o; oflag - 0%o; cflag - 0%o; lflag - 0%o\n",
      (unsigned int)options.c_iflag,
      (unsigned int)options.c_oflag,
      (unsigned int)options.c_cflag,
      (unsigned int)options.c_lflag);
	*/

	// Update the options and do it NOW 
	if (tcsetattr(fdes,TCSANOW,&options) < 0) {
		perror("SetupSerial 3");
		return(FALSE);
	}

	/* Read the options and check they have "taken"
	if (tcgetattr(fdes,&options2) < 0) {
      perror("SetupSerial 4");
      return(FALSE);
   }
   fprintf(stderr,"iflag - 0%o; oflag - 0%o; cflag - 0%o; lflag - 0%o\n",
      (unsigned int)options.c_iflag,
      (unsigned int)options.c_oflag,
      (unsigned int)options.c_cflag,
      (unsigned int)options.c_lflag);
	if (memcmp((void *)&options,(void *)&options2,sizeof(options)) != 0) {
		perror("SetupSerial 5");
      return(FALSE);
	} */

   // Clear the line 
   tcflush(fdes,TCIFLUSH); 

	return(TRUE);
}

void CheckSerialPort(int fdes)
{
   int input_speed,output_speed;
   struct termios options;

   /* Get the current options */
   if (tcgetattr(fdes,&options) != 0) {
      perror("CheckSpeed");
   }

   fprintf(stderr,"iflag - 0%o; oflag - 0%o; cflag - 0%o; lflag - 0%o\n",
      (unsigned int)options.c_iflag,
		(unsigned int)options.c_oflag,
		(unsigned int)options.c_cflag,
		(unsigned int)options.c_lflag);
   fprintf(stderr,"c_ispeed - 0%o; c_ospeed - 0%o\n",
		(unsigned int)options.c_ispeed,
		(unsigned int)options.c_ospeed);

   input_speed = cfgetispeed(&options);
   output_speed = cfgetospeed(&options);
   fprintf(stderr,"Input speed: %d\n",input_speed);
   fprintf(stderr,"Output speed: %d\n",output_speed);
}

/*
   Write a string to the serial port device
*/
int SendSerialString(int fdes,char *buffer)
{
   int len;

   len = strlen(buffer);
   if (write(fdes,buffer,len) != len) 
		return(FALSE);
	return(TRUE);
}

/*
   Write raw bytes to the serial port device
*/
int SendSerialRaw(int fdes,unsigned char *buffer,int len)
{
   if (write(fdes,buffer,len) != len)
      return(FALSE);
   return(TRUE);
}

/*
   Read any characters on the port
	Note that the maximum number of bytes to read has to 
	inclue the trailing '\0' which is added to make a legal
	C string.
*/
int ReadSerialString(int fdes,char *buffer,int maxread)
{
   int len;

   len = read(fdes,buffer,maxread-1);
	if (len < 0 && errno != EAGAIN)
		return(FALSE);
	buffer[len] = '\0';
   return(len);
}

/*          
   Read a single character from the port
*/       
int ReadSerialChar(int fdes,unsigned int *n)
{  
	char c[8];
   int len;

   len = read(fdes,c,1);
   if (len < 1 && errno != EAGAIN)
      return(FALSE);
	*n = c[0];
   return(TRUE);
}

/*
   Read raw bytes from the serial port
	Return the number read
*/
int ReadSerialRaw(int fdes,unsigned char *buffer,int maxread,int timeout)
{
   int len,iptr=0;
   unsigned char buf;
   struct timeval tp;
   double startsecs,secs;

   gettimeofday(&tp,NULL);
   startsecs = tp.tv_sec + tp.tv_usec / 1000000.0;

   for (;;) {
      len = read(fdes,&buf,1);
      if (len > 0) {
         buffer[iptr] = buf;
         iptr++;
			if (iptr >= maxread)
				break;
      }
      gettimeofday(&tp,NULL);
      secs = tp.tv_sec + tp.tv_usec / 1000000.0;
      if (secs-startsecs > timeout)
         break;
   }
   return(iptr);
}

/*
	Clear serial, read until nothing is left 
*/
void ClearSerial(int fdes)
{
	char buf;

   while (read(fdes,&buf,1) > 0) 
		;
}

/*
	Wait for a particular character
	The timeout is in seconds
*/
int SkipSerialUntil(int fdes,char c,double timeout)
{
	char buf;
	int len;
	struct timeval tp;
	double startsecs,secs;

	gettimeofday(&tp,NULL);
	startsecs = tp.tv_sec + tp.tv_usec / 1000000.0;

	for (;;) {
   	len = read(fdes,&buf,1);
		if (len > 0 && buf == c)
			break;
		gettimeofday(&tp,NULL);
		secs = tp.tv_sec + tp.tv_usec / 1000000.0;
		if (secs-startsecs > timeout)
			return(FALSE);
	}
   return(TRUE);
}

/*
	Read until a particular character is received
	Return a C string
*/
int ReadSerialUntil(int fdes,char *buffer,char c,int maxread,double timeout)
{
   int len,iptr=0;
	char buf;
   struct timeval tp;
   double startsecs,secs;

   gettimeofday(&tp,NULL);
   startsecs = tp.tv_sec + tp.tv_usec / 1000000.0;
	buffer[iptr] = '\0';

   for (;;) {
      len = read(fdes,&buf,1);
      if (len > 0 && buf == c)
         break;
		if (len > 0 && iptr < maxread-1) {
			buffer[iptr] = buf;
			iptr++;
			buffer[iptr] = '\0';
		}
      gettimeofday(&tp,NULL);
      secs = tp.tv_sec + tp.tv_usec / 1000000.0;
      if (secs-startsecs > timeout)
         return(FALSE);
   }
   return(TRUE);
}

/*
	Display a possibly binary string
*/
void Raw2stderr(unsigned char *buf,int len)
{
	int i;

	for (i=0;i<len;i++) {
      if (buf[i] < ' ' || buf[i] > 126)
         fprintf(stderr,"<%03d>",buf[i]);
      else
         fprintf(stderr,"%c",buf[i]);
	}
}

/*
   Display a string potentially with non printable characters
*/
void Str2stderr(char *buf)
{
   unsigned int i;

   for (i=0;i<strlen(buf);i++) {
      if (buf[i] < ' ' || buf[i] > 126)
         fprintf(stderr,"<%03d>",buf[i]);
      else
         fprintf(stderr,"%c",buf[i]);
   }
}

/*
	Given a 3 byte string from the mouse in "s"
	Return the button and position.
*/
void DecodeSerialMouse(unsigned char *s,int *button,int *x,int *y)
{
   *button = 'n';
   if ((s[0] & 0x20) != 0)
      *button = 'l';
   else if ((s[0] & 0x10) != 0)
      *button = 'r';
   *x = (s[0] & 0x03) * 64 + (s[1] & 0x3F);
   if (*x > 127)
      *x = *x - 256;
   *y = (s[0] & 0x0C) * 16 + (s[2] & 0x3F);
   if (*y > 127)
      *y = *y - 256;
}

/*
	Set up for the serialversion of the Magellan Spacemosue
*/
int SerialSpacemouse(char *s)
{
   int fd;
   struct termios options,options2;

   if ((fd = open(s,O_RDWR | O_NOCTTY | O_NONBLOCK)) < 0) {
      fprintf(stderr,"Failed to open serial port\n");
   } else {
      fcntl(fd,F_SETFL,FNDELAY);
   }

   // Get the current options
   if (tcgetattr(fd,&options) < 0) {
      fprintf(stderr,"First get failed\n");
      exit(-1);
   }

   // Change things
   cfsetispeed(&options,B9600);
   cfsetospeed(&options,B9600);
	options.c_cflag &= ~CSIZE;
   options.c_cflag |= CS8;
   options.c_cflag &= ~PARENB;
   options.c_cflag |= CSTOPB;
   options.c_cc[VMIN] = 0;
   options.c_cc[VTIME] = 0;

   options.c_lflag = 0;  // No local flags
   options.c_lflag &= ~ICANON; // Don't canonicalise
   options.c_lflag &= ~ECHO; // Don't echo
   options.c_lflag &= ~ECHOK; // Don't echo

   options.c_cflag &= ~CRTSCTS; // Disable RTS/CTS
   options.c_cflag |= CLOCAL; // Ignore status lines
   options.c_cflag |= CREAD; // Enable receiver
   options.c_cflag |= HUPCL; // Drop DTR on close

   options.c_oflag &= ~OPOST; // No output processing
   options.c_oflag &= ~ONLCR; // Don't convert linefeeds

   options.c_iflag |= IGNPAR; // Ignore parity
   options.c_iflag &= ~ISTRIP; // Don't strip high order bit
   options.c_iflag |= IGNBRK; // Ignore break conditions
   options.c_iflag &= ~INLCR; // Don't Map NL to CR
   options.c_iflag &= ~ICRNL; // Don't Map CR to NL
   options.c_iflag |= (IXON | IXOFF | IXANY); // xon/xoff flow control

   // Update the options and do it NOW
   if (tcsetattr(fd,TCSANOW,&options) < 0) {
      fprintf(stderr,"Failed to set\n");
      exit(-1);
   }

   // Read the options again and check they have "taken"
   if (tcgetattr(fd,&options2) < 0) {
      fprintf(stderr,"Second get failed\n");
      exit(-1);
   }

   // Compare
   if (memcmp((void *)&options,(void *)&options2,sizeof(options)) != 0) {
      fprintf(stderr,"Did not compare favourably!\n");
      exit(-1);
   }

	return(fd);
}
