#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <sys/time.h>
#include "paulslib.h"

/*
   Standard port names
      Dec Alpha Xp1000 are tty00 and tty01 (bottom and top)
      Linux                ttyS0     ttyS1
                           cua0      cua1
      Irix                 ttyf1     ttyf2
      Solaris/SunOS        ttya      ttyb
*/

int  OpenSerial(char *);
int  SetupSerial(int,int,int,int,int);
void CheckSerialPort(int);
int  SendSerialString(int,char *);
int  ReadSerialChar(int,unsigned int *);
int  SendSerialRaw(int,unsigned char *,int);
int  ReadSerialString(int,char *,int);
int  ReadSerialRaw(int,unsigned char *,int,int);
void ClearSerial(int);
int  SkipSerialUntil(int,char,double);
int  ReadSerialUntil(int,char *,char,int,double);
void Raw2stderr(unsigned char *,int);
void Str2stderr(char *);
void DecodeSerialMouse(unsigned char *,int *,int *,int *);

int SerialSpacemouse(char *);

