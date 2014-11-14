#ifndef YOCTO_SPADE_DATA_BLOCK_INCLUDED
#define YOCTO_SPADE_DATA_BLOCK_INCLUDED 1

#include "yocto/spade/linear.hpp"

namespace yocto
{
    
    namespace spade
    {
        //! memory for any array
        class data_block
        {
        public:
            explicit data_block( linear &l );
            virtual ~data_block() throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(data_block);
            size_t size; //!< l.bytes
            void  *data; //!< >= size
        };
        
#if defined(_MSC_VER)
		// this in constructor
#pragma warning ( disable : 4355 )
#endif
        template <typename ARRAY>
        class standalone : public ARRAY
        {
        public:
            explicit standalone( const typename ARRAY::layout_type &L ) :
            ARRAY(L),
            data(*this)
            {
            }
            
            virtual ~standalone() throw() {}
            
        private:
            const data_block data;
            YOCTO_DISABLE_COPY_AND_ASSIGN(standalone);
        };
        
    }
    
}

#endif
