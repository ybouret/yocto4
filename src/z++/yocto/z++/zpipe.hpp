#ifndef YOCTO_ZPIPE_INCLUDED
#define YOCTO_ZPIPE_INCLUDED 1

#include "yocto/ios/istream.hpp"
#include "yocto/ios/ostream.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/memory/buffers.hpp"
#include "yocto/memory/blender.hpp"

namespace yocto
{

	namespace zlib
	{

		class zpipe
		{
		public:
			explicit zpipe( size_t chunk_size );
			virtual ~zpipe() throw();

			void def( ios::ostream &target, ios::istream &source, size_t level, memory::blender &blend );
			void inf( ios::ostream &target, ios::istream &source, memory::blender &blend);

			

		private:
			typedef memory::buffer_of<char,memory::global> buff_t;
			const size_t          CHUNK;
			buff_t                wksp_;
			char                 *in;
			char                 *out;
			bool                  eof_;

			YOCTO_DISABLE_COPY_AND_ASSIGN(zpipe);
			size_t _read( ios::istream &source );                   //!< fill  in, return>0 if more chars
			void   _save( ios::ostream &target, size_t len ) const; //!< write len bytes of out
		};
	}

}


#endif
