#include "yocto/utest/run.hpp"

#include "yocto/hashing/adler32.hpp"
#include "yocto/hashing/bjh32.hpp"
#include "yocto/hashing/crc16.hpp"
#include "yocto/hashing/crc32.hpp"
#include "yocto/hashing/elf.hpp"
#include "yocto/hashing/fnv.hpp"
#include "yocto/hashing/pjw.hpp"
#include "yocto/hashing/sfh.hpp"
#include "yocto/hashing/sha1.hpp"

#include "support.hpp"
#include "yocto/ios/icstream.hpp"

#include "yocto/memory/global.hpp"
#include "yocto/wtime.hpp"

#include <iomanip>

using namespace yocto;
using namespace memory;

static inline
void h_perf( hashing::function *h_reg[], const size_t h_num, const uint8_t *data, const size_t size )
{
	wtime chrono;
	const size_t iter = 16;
	for( size_t i=0; i < h_num; ++i )
	{
		hashing::function &H = *h_reg[i];
		std::cerr << "[[ " << std::setw(16) << H.name() << " ]]: ";
		std::cerr.flush();
		double ell = 0;
		size_t key = 0;
		for( size_t j=0; j  < iter; ++j )
		{
			chrono.start();
			H.set();
			H.run( data, size );
			H.get( &key, sizeof(key));
			ell += chrono.query();
		}
		const double tmx = ell/iter;
		std::cerr << "\t" << (double(size)/tmx)/(1024.0*1024.0) << " MB/s" << std::endl;
		std::cerr.flush();
	}
}

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
	hashing::sha1      h_sha1;
	
	hashing::function *h_reg[] = { & h_adler32,  & h_bjh32, &h_crc16, &h_crc32, &h_elf, &h_fnv, &h_pjw, &h_sfh, & h_sha1 };
	const size_t       h_num   = sizeof(h_reg)/sizeof(h_reg[0]);
	
	
	for( size_t i=0; i < h_num; ++i ) h_reg[i]->set();
	
	if( argc > 1 )
	{
		ios::icstream input( argv[1] );
		char buffer[512];
		size_t loaded = 0;
		while( (loaded=input.get(buffer,sizeof(buffer))) > 0 )
		{
			for( size_t i=0; i < h_num; ++i ) h_reg[i]->run(buffer, loaded);
		}
		
	}
	else
	{
		for( size_t j=0; j < 1024; ++j )
		{
			const string msg = gen<string>::get();
			for( size_t i=0; i < h_num; ++i ) (*h_reg[i])( msg );
		}
	}
	
	for( size_t i=0; i < h_num; ++i )
	{
		const uint64_t key = h_reg[i]->key<uint64_t>();
		std::cerr << std::setfill(' ') << std::setw(16) << h_reg[i]->name() << ": " << std::hex << std::setfill('0') << std::setw(16) << key << std::endl;
	}
	std::cerr << std::setfill(' ');
	
	{
		std::cerr << std::dec;
		size_t   bytes = 1024 * 1024;
		uint8_t *data  = kind<global>::acquire_as<uint8_t>( bytes );
		for( size_t i=0; i < bytes; ++i ) data[i] = alea_of<uint8_t>();
		
		h_perf( h_reg, h_num, data, bytes);
		kind<global>::release_as<uint8_t>( data, bytes );
	}
	
	
}
YOCTO_UNIT_TEST_DONE()
