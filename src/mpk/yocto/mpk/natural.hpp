#ifndef YOCTO_MPK_NATURAL_INCLUDED
#define YOCTO_MPK_NATURAL_INCLUDED 1

#include "yocto/mpk/types.hpp"
#include "yocto/memory/buffer.hpp"

#include <iosfwd>

namespace yocto
{
    namespace mpk
    {
#define YOCTO_CHECK_MPN(X)    \
assert( (X).maxi>0 );         \
assert( (X).size<=(X).maxi ); \
assert( (X).byte!=NULL     ); \
assert( !( ((X).size>0) && ( (X).byte[ (X).size-1 ] == 0) ) )

        //______________________________________________________________________
        //
        //! base class for natural, little endian
        //______________________________________________________________________
        class  _natural : public memory::ro_buffer
        {
        public:
            virtual ~ _natural() throw(); //!< destructor, do nothing
            virtual size_t length() const throw(); //!< ro_buffer interface

            void ldz() throw();

            friend std::ostream & operator<<( std::ostream &os, const _natural &n);


        protected:
            size_t   maxi; //!< capacity
            size_t   size; //!< number of active bytes: 0 => null value...
            uint8_t *byte; //!< current bytes

            void      update() throw();  //!< check if size decreased
            void      rescan() throw();  //!< check size from maxi
            explicit _natural() throw(); //!< all to zero

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(_natural);
            virtual const void *get_address() const throw();
        };


        //______________________________________________________________________
        //
        //! wrapper for a word_t
        //______________________________________________________________________
        class natural_word : public _natural
        {
        public:
            natural_word(const word_t x=0) throw();
            virtual ~natural_word() throw();
            natural_word(const natural_word &) throw();
            natural_word & operator=(const natural_word &) throw();
            
        private:
            word_t w; //!< stored le
        };

    }
}

#endif
