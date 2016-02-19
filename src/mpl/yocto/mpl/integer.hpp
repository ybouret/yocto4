#ifndef YOCTO_MPL_INTEGER_INCLUDED
#define YOCTO_MPL_INTEGER_INCLUDED 1

#include "yocto/mpl/natural.hpp"

namespace yocto
{
    namespace mpl
    {
        enum sign_type
        {
            __negative = -1,
            __zero     =  0,
            __positive =  1
        };


        inline sign_type int2sign(const int s) throw()
        {
            return (s<0) ? __negative : ( (0<s) ? __positive : __zero );
        }

        inline sign_type integer2sign(const integer_t s) throw()
        {
            return (s<0) ? __negative : ( (0<s) ? __positive : __zero );
        }

        inline word_t   integer2word(const integer_t s) throw()
        {
            return (s<0) ? word_t(-s) : word_t(s);
        }

        inline sign_type sign_neg(const sign_type s) throw()
        {
            switch(s)
            {
                case __negative : return __positive;
                case __zero:      return __zero;
                case __positive:  return __negative;
            }
        }


        class integer : public object
        {
        public:
            const sign_type s;
            const natural   n;

            //! guess validity. TODO: check !!!!
            inline void update() throw()
            {
                YOCTO_CHECK_MPN(n);
                if(n.is_zero()) (sign_type&)s = __zero;
            }

            //__________________________________________________________________
            //
            // canonical
            //__________________________________________________________________
            virtual ~integer() throw() {}
            inline integer() : s(__zero), n() {}
            inline integer(const integer &other) : s(other.s), n(other.n) {}
            inline void xch(integer &other) throw()
            {
                cswap_const(s,other.s);
                ( (natural &)n ).xch( (natural &)(other.n) );
            }
            inline integer & operator=(const integer &other)
            {
                integer tmp(other);
                xch(tmp);
                return *this;
            }

            inline integer(const integer_t   I) : s(integer2sign(I)), n(integer2word(I)) { }
            inline integer(const natural    &N) : s( N.is_zero() ? __zero : __positive ), n(N) {}

            inline integer & operator=( const natural &N )
            {
                integer tmp(N);
                xch(tmp);
                return *this;
            }

            inline integer & operator=( const integer_t I )
            {
                integer tmp(I);
                xch(tmp);
                return *this;
            }

            inline integer(const sign_type S, const natural &N) : s(S), n(N) { update(); }
            inline integer(const sign_type S, const void *buf, const size_t len) :
            s(S), n(buf,len)
            {
                update();
            }

            //__________________________________________________________________
            //
            //
            // output
            //
            //__________________________________________________________________
            inline friend std::ostream & operator<<(std::ostream &os, const integer &z )
            {
                switch(z.s)
                {
                    case __negative: os << '-'; break;
                    case __zero:     os << '0'; return os;
                    case __positive: break;
                }
                os << z.n;
                return os;
            }

            //__________________________________________________________________
            //
            //
            // comparison
            //
            //__________________________________________________________________
            inline static
            int compare(const sign_type ls,
                        const void     *lb,
                        const size_t    ln,
                        const sign_type rs,
                        const void     *rb,
                        const size_t    rn) throw()
            {
                switch(ls)
                {
                    case __negative:
                    {
                        switch(rs)
                        {
                            case __negative: return  -natural::compare(lb,ln,rb,rn); //!< LHS<0, RHS<0
                            case __zero    : return -1; //!< LHS<0, RHS=0
                            case __positive: return -1; //!< LHS<0, RHS>0
                        }

                    }

                    case __zero:
                    {
                        switch(rs)
                        {
                            case __negative: return 1; //!< LHS=0, RHS<0
                            case __zero    : return 0; //!< LHS=0, RHS=0
                            case __positive: return 1; //!< LHS=0, RHS>0
                        }
                    }

                    case __positive:
                    {
                        switch(rs)
                        {
                            case __negative: return 1; //!< LHS>0, RHS<0
                            case __zero:     return 1; //!< LHS>0, RHS=0
                            case __positive: return natural::compare(lb,ln,rb,rn); //!< LHS>0, RHS>0
                        }
                    }
                }
            }

#define YOCTO_MPZ_CMP(OP) \
inline friend bool operator OP(const integer &lhs, const integer  &rhs) throw() { return integer::compare(lhs.s,lhs.n.ro(),lhs.n.length(),rhs.s,rhs.n.ro(),rhs.n.length()) OP 0; } \
inline friend bool operator OP(const integer &lhs, const integer_t rhs) throw()\
{                                                                              \
const sign_type rs = integer2sign(rhs);                                        \
word_t          rb = integer2word(rhs);                                        \
const size_t    rn = natural::prepare(rb);                                     \
return integer::compare(lhs.s,lhs.n.ro(),lhs.n.length(),rs,&rb,rn) OP 0;       \
}\
inline friend bool operator OP(const integer_t lhs,const integer &rhs) throw() \
{                                                                              \
const sign_type ls = integer2sign(lhs);                                        \
word_t          lb = integer2word(lhs);                                        \
const size_t    ln = natural::prepare(lb);                                     \
return integer::compare(ls,&lb,ln,rhs.s,rhs.n.ro(),rhs.n.length());            \
}

            YOCTO_MPZ_CMP(==)
            YOCTO_MPZ_CMP(!=)
            YOCTO_MPZ_CMP(<)
            YOCTO_MPZ_CMP(<=)
            YOCTO_MPZ_CMP(>)
            YOCTO_MPZ_CMP(>=)


            //__________________________________________________________________
            //
            //
            // Addition
            //
            //__________________________________________________________________
            static inline integer add(const sign_type ls,
                                      const void     *lb,
                                      const size_t    ln,
                                      const sign_type rs,
                                      const void     *rb,
                                      const size_t    rn) throw()
            {
                switch(ls)
                {
                    case __negative:
                    {

                    }

                    case __zero:
                    {
                        switch(rs)
                        {
                            case __zero:     return integer();
                            case __negative:
                            case __positive: return integer(rs,rb,rn);
                        }
                    }

                    case __positive:
                    {
                        
                    }
                }
            }

        };
        
    }
    
    typedef mpl::integer mpz;
    template <>
    struct xnumeric<mpz>
    {
        inline static mpz zero() { return mpz();  }
        inline static mpz one()  { return mpz(1); }
    };
    
}

#endif

