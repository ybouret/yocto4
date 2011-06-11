#include "yocto/utest/run.hpp"

#include "yocto/hashing/adler32.hpp"
#include "yocto/hashing/bjh32.hpp"
#include "yocto/hashing/crc16.hpp"
#include "yocto/hashing/crc32.hpp"
#include "yocto/hashing/elf.hpp"
#include "yocto/hashing/fnv.hpp"
#include "yocto/hashing/pjw.hpp"
#include "yocto/hashing/sfh.hpp"
#include "support.hpp"

#include <iomanip>

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(hash)
{
	hashing::adler32   h_adler32;
	hashing::bjh32     h_bjh32;
	hashing::crc16     h_crc16;
	hashing::crc32     h_crc32;
	hashing::elf       h_elf;
	hashing::fnv       h_fnv;
	hashing::pjw       h_pjw;
	hashing::sfh       h_sfh;
	
	hashing::function *h_reg[] = { & h_adler32,  & h_bjh32, &h_crc16, &h_crc32, &h_elf, &h_fnv, &h_pjw, &h_sfh };
	const size_t       h_num   = sizeof(h_reg)/sizeof(h_reg[0]);
	
	
	for( size_t i=0; i < h_num; ++i ) h_reg[i]->set();
	
	if( argc > 1 )
	{
		
	}
	else
	{
		for( size_t j=0; j < 1024; ++j )
		{
			const string msg = gen<string>::get();
			for( size_t i=0; i < h_num; ++i ) (*h_reg[i]) << msg;
		}
	}

	for( size_t i=0; i < h_num; ++i )
	{
		const uint64_t key = h_reg[i]->key<uint64_t>();
		std::cerr << std::setfill(' ') << std::setw(16) << h_reg[i]->name() << ": " << std::hex << std::setfill('0') << std::setw(16) << key << std::endl;
	}
	
	
}
YOCTO_UNIT_TEST_DONE()
