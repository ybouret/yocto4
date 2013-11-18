#include "yocto/code/rand.hpp"
namespace yocto
{
	static   rand32_kiss   __rand;
	urand32  & _rand = __rand;
}
