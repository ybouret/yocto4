#include "yocto/code/utils.hpp"

namespace yocto
{
	const char hexa_char[16] =
	{
		'0', '1', '2', '3',
		'4', '5', '6', '7',
		'8', '9', 'A', 'B',
		'C', 'D', 'E', 'F'
	};
	
	uint32_t ihash32( uint32_t a)
	{
		a = (a+0x7ed55d16) + (a<<12);
		a = (a^0xc761c23c) ^ (a>>19);
		a = (a+0x165667b1) + (a<<5);
		a = (a+0xd3a2646c) ^ (a<<9);
		a = (a+0xfd7046c5) + (a<<3);
		a = (a^0xb55a4f09) ^ (a>>16);
		return a;
		
	}
	
	int hex2dec( char c ) throw()
	{
		switch( c )
		{
			case '0': return 0;
			case '1': return 1;
			case '2': return 2;
			case '3': return 3;
			case '4': return 4;
			case '5': return 5;
			case '6': return 6;
			case '7': return 7;
			case '8': return 8;
			case '9': return 9;

			case 'a':
			case 'A':
				return 0xa;
				
			case 'b':
			case 'B':
				return 0xb;
				
			case 'c':
			case 'C':
				return 0xc;
				
			case 'd':
			case 'D':
				return 0xd;
				
			case 'e':
			case 'E':
				return 0xe;
				
			case 'f':
			case 'F':
				return 0xf;
				
			default:
				break;
		}
		
		return -1;
		
	}
	
	
}
