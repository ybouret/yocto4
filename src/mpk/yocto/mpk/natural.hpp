#ifndef YOCTO_MPK_NATURAL_INCLUDED
#define YOCTO_MPK_NATURAL_INCLUDED 1

#include "yocto/mpk/types.hpp"
#include "yocto/memory/buffer.hpp"
#include "yocto/code/endian.hpp"

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


#define YOCTO_MPN_DECL2_NOTHROW(RETURN_TYPE,OP,CALL) \
inline friend RETURN_TYPE OP(const natural &lhs,const natural &rhs) throw() { return CALL(lhs,rhs); } \
friend        RETURN_TYPE OP(const natural &lhs,const word_t   rhs) throw(); \
friend        RETURN_TYPE OP(const word_t   lhs,const natural &rhs) throw()

#define YOCTO_MPN_IMPL2_NOTHROW(RETURN_TYPE,OP,CALL)          \
RETURN_TYPE OP(const natural &lhs,const word_t   rhs) throw() \
{ const word2mpn RHS(rhs); return CALL(lhs,RHS.n); }          \
RETURN_TYPE OP(const word_t lhs, const natural &rhs) throw()  \
{ const word2mpn LHS(lhs); return CALL(LHS.n,rhs); }

#define YOCTO_MPN_DECL2(RETURN_TYPE,OP,CALL) \
inline friend RETURN_TYPE OP(const natural &lhs,const natural &rhs) { return CALL(lhs,rhs); } \
friend        RETURN_TYPE OP(const natural &lhs,const word_t   rhs); \
friend        RETURN_TYPE OP(const word_t   lhs,const natural &rhs)

#define YOCTO_MPN_IMPL2(RETURN_TYPE,OP,CALL)            \
RETURN_TYPE OP(const natural &lhs,const word_t   rhs)   \
{ const word2mpn RHS(rhs); return CALL(lhs,RHS.n); }    \
RETURN_TYPE OP(const word_t lhs, const natural &rhs)    \
{ const word2mpn LHS(lhs); return CALL(LHS.n,rhs); }


        class natural : public memory::ro_buffer
        {
        public:
            virtual ~natural() throw();
            virtual size_t length() const throw();

            //__________________________________________________________________
            //
            // canonic functions
            //__________________________________________________________________
            natural(); //!< zero
            natural(const size_t num_bytes, const as_capacity_t &);
            natural(const natural &other);
            natural & operator=( const natural &other);

            //__________________________________________________________________
            //
            // helpers
            //__________________________________________________________________
            natural(word_t w);
            natural & operator=(const word_t w);
            void xch(natural &other) throw();
            void ldz() throw();

            friend std::ostream & operator<<( std::ostream &os, const natural &n);

            //__________________________________________________________________
            //
            // comparison: are equal
            //__________________________________________________________________
            static bool are_equal(const natural &lhs, const natural &rhs) throw();
            YOCTO_MPN_DECL2_NOTHROW(bool,operator==,are_equal);

            //__________________________________________________________________
            //
            // comparison: are different
            //__________________________________________________________________
            static bool are_different(const natural &lhs, const natural &rhs) throw();
            YOCTO_MPN_DECL2_NOTHROW(bool,operator!=,are_different);

            //__________________________________________________________________
            //
            // comparison: generic case
            //__________________________________________________________________
            static int compare(const natural &lhs, const natural &rhs) throw();


            //__________________________________________________________________
            //
            // add
            //__________________________________________________________________
            static natural add(const natural &lhs, const natural &rhs);
            inline natural operator+() { return natural(*this); }
            YOCTO_MPN_DECL2(natural,operator+,add);

        private:
            size_t   maxi; //!< capacity
            size_t   size; //!< significant bytes
            uint8_t *byte; //!< memory

            void update() throw(); //!< check size reduction
            void rescan() throw(); //!< size=maxi and update

            virtual const void * get_address() const throw();
            friend class word2mpn;
        };

       
    }
}

#endif
