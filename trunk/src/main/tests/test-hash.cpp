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
#include "yocto/sys/wtime.hpp"

#include <iomanip>
#include <cstdlib>

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
		for(;;)
		{
			const size_t loaded = input.get(buffer,sizeof(buffer) );
			if( !loaded ) break;
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

#include "yocto/sequence/vector.hpp"

template <typename T>
static inline int cmpfast( const void *lhs, const void *rhs ) throw()
{
	const int64_t a = *(const T*)lhs;
	const int64_t b = *(const T*)rhs;
	return int(a-b);
}

YOCTO_UNIT_TEST_IMPL(hcol)
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
	
	vector<uint16_t> v16;
	vector<uint32_t> v32;
	vector<size_t>   vsz;
	
	if( argc > 1 )
	{
		for( size_t i=0; i < h_num; ++i )
		{
			hashing::function &H = *h_reg[i];
			v16.free();
			v32.free();
			vsz.free();
			ios::icstream input( argv[1] );
			string line;
			size_t count=0;
			while( input.gets( line ) )
			{
				++count;
				
				const uint16_t k16 = H.key<uint16_t>( line );
				const uint32_t k32 = H.key<uint32_t>( line );
				const size_t   ksz = H.key<size_t>( line );
				//std::cerr << k16 << ':' << k32 << ':' << ksz << std::endl;
				if( !bsearch( &k16, v16(), v16.size(), sizeof(k16), cmpfast<uint16_t> ) )
				{
					v16.push_back( k16 );
					qsort( v16(), v16.size(), sizeof(k16), cmpfast<uint16_t> );
				}
				
				if( !bsearch( &k32, v32(), v32.size(), sizeof(k32), cmpfast<uint32_t> ) )
				{
					v32.push_back( k32 );
					qsort( v32(), v32.size(), sizeof(k32), cmpfast<uint32_t> );
				}	
				
				if( !bsearch( &ksz, vsz(), vsz.size(), sizeof(ksz), cmpfast<size_t> ) )
				{
					vsz.push_back( ksz );
					qsort( vsz(), vsz.size(), sizeof(ksz), cmpfast<size_t> );
				}	
				
				
			}
			std::cerr << "---- " << H.name() << " ---- " << std::endl;
			std::cerr << "count=" << count << std::endl;
			if( count >  0 )
			{
				const double p16 = 100.0 * double(v16.size())/count;
				const double p32 = 100.0 * double(v32.size())/count;
				const double psz = 100.0 * double(vsz.size())/count;
				std::cerr << "16: " << p16 << "% | 32: " << p32 << "% | sz:" << psz << "%" << std::endl;
			}
		}
		
	}
	
	
}
YOCTO_UNIT_TEST_DONE()

#include "yocto/associative/key-hasher.hpp"

YOCTO_UNIT_TEST_IMPL(key_hasher)
{
    key_hasher<string,hashing::sha1> Hash;
    
}
YOCTO_UNIT_TEST_DONE()

