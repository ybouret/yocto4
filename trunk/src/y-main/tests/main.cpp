#include "yocto/utest/driver.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_INIT(64)
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
	YOCTO_UNIT_TEST_DECL(hcol);
	YOCTO_UNIT_TEST_DECL(streams);
	YOCTO_UNIT_TEST_DECL(ppty);
	YOCTO_UNIT_TEST_DECL(uuid);
	YOCTO_UNIT_TEST_DECL(env);
	YOCTO_UNIT_TEST_DECL(scroll);
	YOCTO_UNIT_TEST_DECL(auto_scroll);
	YOCTO_UNIT_TEST_DECL(utf8);
	YOCTO_UNIT_TEST_DECL(slab);
	YOCTO_UNIT_TEST_DECL(embed);
	YOCTO_UNIT_TEST_DECL(ktable);
	YOCTO_UNIT_TEST_DECL(map);
	YOCTO_UNIT_TEST_DECL(set);	
	YOCTO_UNIT_TEST_DECL(locate);	
	YOCTO_UNIT_TEST_DECL(shared_ptr);
	YOCTO_UNIT_TEST_DECL(stock);
	YOCTO_UNIT_TEST_DECL(frag_layout);
	YOCTO_UNIT_TEST_DECL(frag_block);
	YOCTO_UNIT_TEST_DECL(frag_queue);
	YOCTO_UNIT_TEST_DECL(bitio);
	YOCTO_UNIT_TEST_DECL(xarray);
	YOCTO_UNIT_TEST_DECL(dict);
	YOCTO_UNIT_TEST_DECL(catalog);
	YOCTO_UNIT_TEST_DECL(c_array);
	YOCTO_UNIT_TEST_DECL(raw_file);
	YOCTO_UNIT_TEST_DECL(module);
	YOCTO_UNIT_TEST_DECL(sort);

}
YOCTO_UNIT_TEST_EXEC()

