#include "yocto/utest/run.hpp"
#include "yocto/memory/embed.hpp"
#include "yocto/string.hpp"
#include "yocto/memory/global.hpp"
#include  "yocto/memory/pooled.hpp"
using namespace yocto;


YOCTO_UNIT_TEST_IMPL(embed)
{
	std::cerr << "sizeof(string)=" << sizeof(string) << std::endl;
	string   *pStr = NULL;
	uint32_t *pDW  = NULL;
	char     *pCH  = NULL;
	memory::embed reg[] =
	{
		YOCTO_EMBED(pStr,5),
		YOCTO_EMBED(pDW,12),
		YOCTO_EMBED(pCH,4)
	};
	
	const size_t num = sizeof(reg)/sizeof(reg[0]);
	for( size_t i=0; i < num; ++i )
	{
		std::cerr << "@@" << reg[i].hook << ": @" << reg[i].offset << "+" << reg[i].length << std::endl;
	}
	std::cerr << "prepare..." << std::endl;
	
	size_t bytes = memory::embed::prepare(reg,num);
	for( size_t i=0; i < num; ++i )
	{
		std::cerr << "@@" << reg[i].hook << ": @" << reg[i].offset << "+" << reg[i].length << std::endl;
	}
	std::cerr << "bytes=" << bytes << std::endl;
	
	void *wksp = memory::kind<memory::global>::acquire( bytes );
	
	memory::embed::dispatch(reg,num,wksp);
	for( size_t i=0; i < num; ++i )
	{
		std::cerr << "@" << *(reg[i].hook) << std::endl;
	}
	memory::kind<memory::global>::release( wksp, bytes );
	
	
	pStr = NULL;
	pDW  = NULL;
	pCH  = NULL;
	
	memory::pooled::allocator pa;
	
	YOCTO_EMBED_START()
	{
		YOCTO_EMBED(pStr,18),
		YOCTO_EMBED(pDW,17),
		YOCTO_EMBED(pCH,1)
	}
	YOCTO_EMBED_ALLOC(pa,wksp,bytes);
	std::cerr << "pStr@" << (void*)pStr << std::endl;
	std::cerr << "pDW@"  << (void*)pDW << std::endl;
	std::cerr << "pCH@"  << (void*)pCH << std::endl;
	pa.release( wksp, bytes );
	
	
}
YOCTO_UNIT_TEST_DONE()

