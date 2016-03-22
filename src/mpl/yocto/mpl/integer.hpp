#ifndef YOCTO_MPL_INTEGER_INCLUDED
#define YOCTO_MPL_INTEGER_INCLUDED 1

#include "yocto/mpl/natural.hpp"
#include "yocto/error.hpp"
#include <cerrno>

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
	    libc::critical_error( EINVAL, "unexpected sign_neg failure!!!");
	    return __zero;
        }
        
        inline sign_type sign_mul(const sign_type a, const sign_type b) throw()
        {
            switch(a)
            {
                case __negative: return sign_neg(b);
                case __zero:     return __zero;
                case __positive: return b;
            }
	    libc::critical_error( EINVAL, "unexpected sign_mul failure!!!");
	    return __zero;
        }
        
        inline sign_type sign_abs(const sign_type s) throw()
        {
	    return (__zero==s) ? __zero : __positive;
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
                            case __negative: return  1; //!< LHS=0, RHS<0
                            case __zero    : return  0; //!< LHS=0, RHS=0
                            case __positive: return -1; //!< LHS=0, RHS>0
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
                libc::critical_error( EINVAL, "unexpected MPZ sign failure!!!");
                return 0;
            }
            
#define YOCTO_MPZ_CMP(OP) \
inline friend bool operator OP(const integer &lhs, const integer  &rhs) throw()\
{ return integer::compare(                                                     \
lhs.s,lhs.n.ro(),lhs.n.length(),                                               \
rhs.s,rhs.n.ro(),rhs.n.length()                                                \
) OP 0; }                                                                      \
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
return integer::compare(ls,&lb,ln,rhs.s,rhs.n.ro(),rhs.n.length()) OP 0;       \
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
                        switch(rs)
                        {
                            case __zero:     return integer(__negative,lb,ln);
                            case __negative:
                            {
                                const natural nn = natural::add(lb,ln,rb, rn);
                                return integer(__negative,nn);
                            }
                            case __positive:
                            {
                                // LHS<0, RHS>0
                                const int cmp = natural::compare(lb, ln, rb, rn);
                                if(cmp>=0)
                                {
                                    const natural nn = natural::sub(lb,ln,rb,rn);
                                    return integer(__negative,nn);
                                }
                                else
                                {
                                    const natural nn = natural::sub(rb,rn,lb,ln);
                                    return integer(__positive,nn);
                                }
                            }
                        }
                    }
                        
                    case __zero:
                    {
                        switch(rs)
                        {
                            case __zero:     return integer();                 //-- LHS=0, RHS=0
                            case __negative: return integer(__negative,rb,rn); //-- LHS=0, RHS<0
                            case __positive: return integer(__positive,rb,rn); //-- LHS=0, RHS>0
                        }
                    }
                        
                    case __positive:
                    {
                        switch(rs)
                        {
                            case __zero:      return integer(__positive,lb,ln);
                            case __positive:
                            {
                                const natural nn = natural::add(lb,ln,rb, rn);
                                return integer(__positive,nn);
                            }
                            case __negative:
                            {
                                // LHS >0, RHS < 0
                                const int cmp = natural::compare(lb, ln, rb, rn);
                                if(cmp>=0)
                                {
                                    const natural nn = natural::sub(lb,ln,rb,rn);
                                    return integer(__positive,nn);
                                }
                                else
                                {
                                    const natural nn = natural::sub(rb,rn,lb,ln);
                                    return integer(__negative,nn);
                                }
                            }
                        }
                    }
                }
                libc::critical_error(EINVAL,"unexpected sign failure in MPZ add!!!");
				return integer();
            }
            
            inline integer operator+() const { return integer(*this); }
#define YOCTO_MPZ_DECL(OP,OP1,CALL) \
inline friend integer operator OP ( const integer &lhs, const integer &rhs )      \
{ return CALL(lhs.s,lhs.n.ro(),lhs.n.length(),rhs.s,rhs.n.ro(),rhs.n.length()); } \
inline friend integer operator OP(const integer &lhs, const integer_t rhs)        \
{                                                                                 \
const sign_type rs = integer2sign(rhs);                                           \
word_t          rb = integer2word(rhs);                                           \
const size_t    rn = natural::prepare(rb);                                        \
return CALL(lhs.s,lhs.n.ro(),lhs.n.length(),rs,&rb,rn);                           \
}                                                                                 \
inline friend integer operator OP(const integer_t lhs,const integer &rhs)         \
{                                                                                 \
const sign_type ls = integer2sign(lhs);                                           \
word_t          lb = integer2word(lhs);                                           \
const size_t    ln = natural::prepare(lb);                                        \
return CALL(ls,&lb,ln,rhs.s,rhs.n.ro(),rhs.n.length());                           \
}\
inline integer & operator OP1 ( const integer &rhs ) { \
integer tmp = (*this) + rhs; xch(tmp); return *this; } \
inline integer & operator OP1 ( const integer_t rhs) { \
integer tmp = (*this) + rhs; xch(tmp); return *this; }
            
            YOCTO_MPZ_DECL(+,+=,add)
            
            inline integer &inc()
            {
                word_t       __one = 1;
                const size_t __len = natural::prepare(__one);
                integer      __ans = add(s,n.ro(),n.length(),__positive,&__one,__len);
                xch(__ans);
                return *this;
            }
            
            //! prefix increment
            inline integer & operator++()
            {
                return inc();
            }
            
            //! postfix increment
            integer   operator++ (int)
            {
                integer sav(*this);
                (void)inc();
                return sav;
            }
            
            
            //__________________________________________________________________
            //
            //
            // subtraction
            //
            //__________________________________________________________________
            static inline integer sub(const sign_type ls,
                                      const void     *lb,
                                      const size_t    ln,
                                      const sign_type rs,
                                      const void     *rb,
                                      const size_t    rn) throw()
            {
                return add(ls,lb,ln, sign_neg(rs), rb, rn);
            }
            
            inline integer operator-() const { return integer( sign_neg(s), n ); }
            
            YOCTO_MPZ_DECL(-,-=,sub)
            
            inline integer &dec()
            {
                word_t       __one = 1;
                const size_t __len = natural::prepare(__one);
                integer      __ans = add(s,n.ro(),n.length(),__negative,&__one,__len);
                xch(__ans);
                return *this;
            }
            
            //! prefix increment
            inline integer & operator--()
            {
                return dec();
            }
            
            //! postfix increment
            integer   operator-- (int)
            {
                integer sav(*this);
                (void)dec();
                return sav;
            }
            
            //__________________________________________________________________
            //
            //
            // multiplication
            //
            //__________________________________________________________________
            static inline integer mul(const sign_type ls,
                                      const void     *lb,
                                      const size_t    ln,
                                      const sign_type rs,
                                      const void     *rb,
                                      const size_t    rn) throw()
            {
                if(__zero==ls||__zero==rs)
                {
                    return integer();
                }
                else
                {
                    const natural p = natural::mul(lb, ln, rb, rn);
                    return integer( sign_mul(ls,rs), p );
                }
            }
            YOCTO_MPZ_DECL(*,*=,mul)
            
            inline friend integer operator*(const integer &lhs, const natural &rhs)
            {
                const  natural p = lhs.n * rhs;
                return integer(lhs.s,p);
            }
            
            //__________________________________________________________________
            //
            //
            // division
            //
            //__________________________________________________________________
            static inline integer div(const sign_type ls,
                                      const void     *lb,
                                      const size_t    ln,
                                      const sign_type rs,
                                      const void     *rb,
                                      const size_t    rn) throw()
            {
                const natural q = natural::div(lb, ln, rb, rn);
                return integer( sign_mul(ls,rs), q );
            }
            YOCTO_MPZ_DECL(/,/=,div)
            
            inline friend integer operator/(const integer &lhs, const natural &rhs)
            {
                const  natural q = lhs.n * rhs;
                return integer(lhs.s,q);
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

