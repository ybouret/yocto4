#include "yocto/utest/driver.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_INIT(32)
{
	
	YOCTO_UNIT_TEST_DECL(platform);
	YOCTO_UNIT_TEST_DECL(mutex);
	YOCTO_UNIT_TEST_DECL(round);
	YOCTO_UNIT_TEST_DECL(unroll);
	YOCTO_UNIT_TEST_DECL(wtime);
	YOCTO_UNIT_TEST_DECL(rand);
	YOCTO_UNIT_TEST_DECL(thread);
	YOCTO_UNIT_TEST_DECL(pool);
	YOCTO_UNIT_TEST_DECL(list);
	YOCTO_UNIT_TEST_DECL(chunk);
	YOCTO_UNIT_TEST_DECL(at_exit);
	YOCTO_UNIT_TEST_DECL(singleton);
	YOCTO_UNIT_TEST_DECL(global);
	YOCTO_UNIT_TEST_DECL(arena);
	YOCTO_UNIT_TEST_DECL(blocks);
	YOCTO_UNIT_TEST_DECL(small_object);
	YOCTO_UNIT_TEST_DECL(object);
	YOCTO_UNIT_TEST_DECL(slice);
	YOCTO_UNIT_TEST_DECL(carver);
	YOCTO_UNIT_TEST_DECL(pooled);
	YOCTO_UNIT_TEST_DECL(string);
	YOCTO_UNIT_TEST_DECL(type_traits);
	YOCTO_UNIT_TEST_DECL(functor);
	YOCTO_UNIT_TEST_DECL(container);
	YOCTO_UNIT_TEST_DECL(sequence);
	YOCTO_UNIT_TEST_DECL(primes);
	YOCTO_UNIT_TEST_DECL(hash);
	
}
YOCTO_UNIT_TEST_EXEC()

