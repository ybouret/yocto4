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
            
            void ldz() throw(); //!< set size to 0 => null value
            
            friend std::ostream & operator<<( std::ostream &os, const _natural &n);
            
            
        protected:
            size_t   maxi; //!< capacity
            size_t   size; //!< number of active bytes: 0 => null value...
            uint8_t *byte; //!< current bytes
            
            void      update() throw();  //!< check if size decreased
            void      rescan() throw();  //!< check size from maxi
            explicit _natural() throw(); //!< all to zero
            
            friend class natural;
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
            word_t w; //!< stored little endian value
            friend class natural;
        };
        
        
#define YOCTO_MPN_WRAP2(RETURN_TYPE,FUNCTION,CALL)                           \
inline  RETURN_TYPE FUNCTION(const word_t w, const _natural &rhs) \
{ const natural_word lhs(w); return CALL(lhs,rhs); }                \
inline  RETURN_TYPE FUNCTION(const _natural &lhs, const word_t w) \
{ const natural_word rhs(w); return CALL(lhs,rhs); }
        
        //______________________________________________________________________
        //
        //! dynamic natural number
        //______________________________________________________________________
        class natural : public _natural
        {
        public:
            //__________________________________________________________________
            //
            // canonic functions
            //__________________________________________________________________
            virtual ~natural() throw();
            natural();                         //!< set to 0
            natural(const natural &other);     //!< copy
            void xch(natural &other) throw();  //!< no throw swap
            natural & operator=(const natural &other); //!< assign
            
            
            //__________________________________________________________________
            //
            // real world helpers
            //__________________________________________________________________
            natural( const size_t num_bytes, const as_capacity_t &);
            natural(const word_t w);
            natural & operator=(const word_t w); //!< assign
            
            //__________________________________________________________________
            //
            // addition
            //__________________________________________________________________
            static
            natural add(const _natural &lhs, const _natural &rhs);
            YOCTO_MPN_WRAP2(static natural,add,add)
            
            inline natural operator+()
            {
                return *this;
            }
            
            inline friend natural operator+(const _natural &lhs, const _natural &rhs)
            {
                return natural::add(lhs,rhs);
            }
            YOCTO_MPN_WRAP2(friend natural,operator+,add)
            
        private:
            void startup(size_t num_bytes);
            void cleanup() throw();
        };
        
    }
}

#endif
