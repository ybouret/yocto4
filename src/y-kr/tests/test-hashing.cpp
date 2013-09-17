#include "yocto/utest/run.hpp"
#include "yocto/hashing/md2.hpp"
#include "yocto/hashing/md4.hpp"
#include "yocto/hashing/md5.hpp"
#include "yocto/hashing/sha1.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/hashing/digest.hpp"
#include "yocto/hashing/sha256.hpp"
#include "yocto/hashing/sha512.hpp"
#include "yocto/hashing/rmd160.hpp"
#include "yocto/hashing/rmd128.hpp"
#include "yocto/sys/wtime.hpp"
#include "yocto/ios/raw-file.hpp"

#include <iomanip>

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(hashing)
{
	hashing::md2    h_md2;
	hashing::md4    h_md4;
	hashing::md5    h_md5;
	hashing::sha1   h_sha1;
	hashing::sha224 h_sha224;
	hashing::sha256 h_sha256;
	hashing::sha384 h_sha384;
	hashing::sha512 h_sha512;
    hashing::rmd160 h_rmd160;
    hashing::rmd128 h_rmd128;

    
	{
		const string wiki = "Wikipedia, l'encyclopedie libre et gratuite";
		{
			const digest o_md = digest::checksum( h_md2, wiki);
			const digest i_md = digest::hex("c9b3cbbeb539034aac22567fa975f98e");
			if( o_md != i_md ) throw exception("MD2 failure");
		}
		
		{
			const digest o_md = digest::checksum( h_md4, wiki);
			const digest i_md = digest::hex("b94e66e0817dd34dc7858a0c131d4079");
			if( o_md != i_md ) throw exception("MD4 failure");
		}
		
		{
			const digest o_md = digest::checksum( h_md5, wiki);
			const digest i_md = digest::hex("d6aa97d33d459ea3670056e737c99a3d");
			if( o_md != i_md ) throw exception("MD5 failure");
		}
		{
			const digest o_md = digest::checksum( h_sha1, wiki);
			const digest i_md = digest::hex("c18cc65028bbdc147288a2d136313287782b9c73");
			if( o_md != i_md ) throw exception("SHA-1 failure");
		}
		
		{
			const digest o_md = digest::checksum( h_sha224, wiki);
			const digest i_md = digest::hex("21a80161612f4625b5250e2fb3114bdcadf468e41390d5e61c37a4f8");
			if( o_md != i_md ) throw exception("SHA-224 failure");
		}
		
		
		{
			const digest o_md = digest::checksum( h_sha256, wiki);
			const digest i_md = digest::hex("26e6a5869428afd4393ce9edff2fe70ec37725d2d81612e64fc9ce8effbf9a75");
			if( o_md != i_md ) throw exception("SHA-256 failure");
		}
		
		{
			const digest o_md = digest::checksum( h_sha384, wiki);
			const digest i_md = digest::hex("cda113c0c525d008285f8027936303d9f90e9c26553f267b451e8b695598f54cef65f6826b25d48bb428289c8fb5e736");
			if( o_md != i_md ) throw exception("SHA-384 failure");
		}
		
		{
			const digest o_md = digest::checksum( h_sha512, wiki);
			const digest i_md = digest::hex("73585d7c393cc548cfd6c4774a62f49c20bf16585a08a1e276f84fe3523538da12b61742f059ab56b05905eab6bab94b22b8b576ca7f5d8b612b2959083b84f3");
			if( o_md != i_md ) throw exception("SHA-512 failure");
		}
        
        {
            const digest o_md = digest::checksum( h_rmd160, wiki);
			const digest i_md = digest::hex("6ac643de598c9f47e584b10ef9e1ff9c22103d8a");
            if( o_md != i_md ) throw exception("RMD-160 failure");
        }
		
	}
	
	hashing::function * h_reg[] = { &h_md2, &h_md4, &h_md5, &h_sha1, &h_sha224, &h_sha256, &h_sha384, &h_sha512, &h_rmd160, &h_rmd128 };
	const size_t        h_num   = sizeof(h_reg)/sizeof(h_reg[0]);
	
	for( size_t i=0; i < h_num; ++i )
	{
		std::cerr << std::setw(16) << h_reg[i]->name() << " : " << h_reg[i]->length << " | " << h_reg[i]->window << std::endl;
	}
	vector<string> words;
	
	if( argc > 1 )
	{
		//-- Get Global Hash
		{
			ios::icstream input( argv[1] );
			for( size_t i=0; i < h_num; ++i )
			{
				h_reg[i]->set();
			}
			char   buf[512];
			size_t len = 0;
			for(;;)
			{
				input.get(buf,sizeof(buf),len);
				if(!len) break;
				for( size_t i=0; i < h_num; ++i )
				{
					h_reg[i]->run(buf,len);
				}
			}
			
			for( size_t i=0; i < h_num; ++i )
			{
				const digest D = digest::get_from(*h_reg[i]);
				std::cerr << std::setw(16) << h_reg[i]->name() << " : " << D << std::endl;
			}
		}
		
		//-- extract words
		{
			ios::icstream input( argv[1] );
			string line;
			while( input.read_line(line) >= 0 )
			{
				words.push_back(line);
				line.clear();
			}
		}
        
        //-- perf
        wtime        chrono;
        vector<char> data;
        {
            ios::raw_file inp( argv[1], ios::readable);
            const size_t  len = inp.length();
            data.make(len,0);
            inp.get_all(data(), data.size());
        }
        std::cerr << "hashing " << data.size() << " bytes" << std::endl;
        uint8_t hashcode[32];
        const size_t niter = 16;
        for( size_t i=0; i < h_num; ++i )
        {
            hashing::function &h = *h_reg[i];
            chrono.start();
            for(size_t iter=1; iter<=niter;++iter)
            {
                h.set();
                h.run( data(), data.size() );
                h.get(hashcode, sizeof(hashcode) );
            }
            const double ell = chrono.query();
            const double spd = niter * (data.size() /ell);
            std::cerr << std::setw(16) << h.name() << ": " << (spd*1e-6) << " MB/s" << std::endl;
        }
	}
	else
	{
		for( size_t i=0; i < 256; ++i )
		{
			const string tmp = char(i);
			words.push_back(tmp);
		}
	}
	
	
}
YOCTO_UNIT_TEST_DONE()
