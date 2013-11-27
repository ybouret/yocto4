#ifndef YOCTO_RAND32_INCLUDED
#define YOCTO_RAND32_INCLUDED 1

#include "yocto/code/bswap.hpp"

namespace yocto 
{
    
    namespace core 
    {
        
        //! Marsaglia's 32 bit random generators
        class rand32
        {
        public:
            uint32_t z,w,jsr,jcong,a,b;
            uint32_t x,y,bro;
            uint8_t  c;
            uint32_t t[256];
            
            explicit rand32() throw();
            virtual ~rand32() throw();
            
            typedef uint32_t (rand32::*generator)();
            uint32_t mwc()   throw();
            uint32_t shr3()  throw();
            uint32_t cong()  throw();
            uint32_t fib()   throw();
            uint32_t kiss()  throw();
            uint32_t lfib4() throw();
            uint32_t swb()   throw();
            
            void settable( uint32_t i1, uint32_t i2, uint32_t i3, uint32_t i4, uint32_t i5, uint32_t i6 ) throw();
            void reset( uint32_t s ) throw();
            
            //! make a random 0 or 1
            template <typename T>
            static inline T to_bit( uint32_t u ) throw() 
            { 
                static const uint32_t threshold = uint32_t(1) << 31;
                static const T __zero(0);
                static const T __one(1);
                return u >= threshold ? __one : __zero;
            }
            
            
            //! convert to 0:1 exclusive
            static inline double to_double( uint32_t u ) throw()
            {
                return (0.5+double(u))/4294967296.0;
            }
            
            //! convert to 0:1 exclusive
            static inline float to_float( uint32_t u ) throw()
            {
                return (0.5f+float(u))/4294967296.0f;
            }
            
            static void test();
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(rand32);
        };
        
        
        
        
    }
    
    //! interface to a 32 bits uniform generator
    class urand32 
    {
    public:
        virtual ~urand32() throw();
        
        virtual uint32_t next() throw() = 0;
        virtual void     seed(uint32_t) throw() = 0;
        void             wseed() throw(); //!< seed with wtime::seed + pid
        
        
        template <typename T> 
        T get() throw(); //!< valid for uin32_t, float, double
        
        //! generate a pair of gaussian number
        void gaussian(double &u, double &v ) throw();
        
        void hypersphere(const double radius, double arr[], const size_t num) throw();
        
        template <typename T>
        inline T full() throw()
        {
            return __full<T>( int2type<(sizeof(T)>sizeof(uint32_t))>() ); 
        }
        
        
        //! returns in 0..n
        inline size_t leq( size_t n ) throw() 
        {
            return full<size_t>() % (++n);
        }
        
        //! return in 0..n-1
        inline size_t lt( size_t n ) throw()
        {
            assert(n>0);
            return full<size_t>() % n;
        }
        
         
        //! Knuth shuffle
        template <typename T>
        inline void shuffle( T *a, size_t n ) throw()
        {
            assert(!(NULL==a&&n>0));
            if( n > 1 )
            {
                for( size_t i=n-1;i>0;--i)
                {
                    const size_t j   = leq(i);
					bswap( a[i], a[j] );
                }
            }
        }
        
        
    protected:
        explicit urand32() throw();
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(urand32);
        template <typename T>
        inline T __full( int2type<false> ) throw()
        {
            assert(sizeof(T)<=sizeof(uint32_t));
            return T(next());
        }
        
        template <typename T>
        inline T __full( int2type<true> ) throw()
        {
            assert(sizeof(T)>sizeof(uint32_t));
            T ans(0);
            for( size_t i=sizeof(T)/sizeof(uint32_t);i>0;--i)
            {
                ans <<= 32;
                ans |=  T(next());
            }
            return ans;
        }
        
    };
    
    
    template <core::rand32::generator G>
    class grand32 : public urand32
    {
    public:
        explicit grand32() throw() : urand32(), r() {}
        virtual ~grand32() throw() {}
        virtual uint32_t next() throw() { return (r.*G)(); }
        virtual void     seed(uint32_t s) throw() { r.reset(s); }
        
    private:
        core::rand32 r;
        YOCTO_DISABLE_COPY_AND_ASSIGN(grand32);
    };
    
    typedef grand32<&core::rand32::mwc>    rand32_mwc;
    typedef grand32<&core::rand32::shr3>   rand32_shr3;
    typedef grand32<&core::rand32::cong>   rand32_cong;
    typedef grand32<&core::rand32::fib>    rand32_fib;
    typedef grand32<&core::rand32::kiss>   rand32_kiss;
    typedef grand32<&core::rand32::lfib4>  rand32_lfib4;
    typedef grand32<&core::rand32::swb>    rand32_swb;
    
    template <typename T,typename GRAND32 = rand32_kiss>
    class uniform_generator : public GRAND32
    {
    public:
        explicit uniform_generator() throw() :
        GRAND32() { this->wseed(); }
        virtual ~uniform_generator() throw() {}
        inline T operator()(void) throw() { return this->template get<T>(); }
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(uniform_generator);
    };
    
}

#endif
