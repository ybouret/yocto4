
namespace yocto {
    
    namespace math
    {
        
        real_t Atan2( real_t x, real_t y ) throw()
        {
            static const real_t full_pi = numeric<real_t>::pi;
            static const real_t half_pi = numeric<real_t>::half_pi;
            
            if( x > 0 )
            {
                return Atan(y/x);
            }
            else
            {
                if( x < 0 )
                {
                    return (y >= 0) ? Atan(y/x) + full_pi : Atan(y/x) - full_pi;
                }
                else
                {
                    // undefined for (0,0) => set 0
                    return y > 0 ? half_pi : ( y < 0 ? -half_pi : 0 );
                }
            }
            
            
        }
        
        
        template <>
        void numeric<real_t>:: round_error(real_t &x) throw()
        {
            x = Fabs(x);
            if(x<=numeric<real_t>::tiny)
            {
                x=0;
            }
            else
            {
                x = Pow( real_t(10), Ceil( Log10(x) ) );
            }
        }
        
        template <>
        real_t numeric<real_t>:: min_space(const real_t x) throw()
        {
            
            return 0;
        }
        
    }
}
