#ifndef YOCTO_MATH_SIG_EXPAND_INCLUDED
#define YOCTO_MATH_SIG_EXPAND_INCLUDED 1

#include "yocto/math/types.hpp"
#include "yocto/sequence/array.hpp"

namespace yocto
{

    namespace math
    {

        enum expand_type
        {
            expand_zero,     //!< returns 0 out of the boundary
            expand_constant, //!< return border value
            expand_cyclic,   //!< assuming cyclic value
            expand_odd,      //!< odd  behavior w.r.t the boundary
            expand_even      //!< even behavior w.r.t the boundary
        };

        inline expand_type get_derivative_expand_type( const expand_type t ) throw()
        {
            switch(t)
            {
                case expand_cyclic: return expand_cyclic;
                case expand_odd:    return expand_even;
                case expand_even:   return expand_odd;
                default:;
            }
            return expand_zero;
        }

        template <typename T>
        class expand
        {
        public:
            const expand_type lower;
            const expand_type upper;

            inline virtual ~expand() throw() {}

            inline explicit expand(const expand_type lo, const expand_type up) throw() :
            lower(lo),upper(up)
            {

            }

            inline explicit expand(const expand_type both) throw() :
            lower(both), upper(both)
            {

            }


            inline explicit expand(const expand &other) throw() :
            lower(other.lower),
            upper(other.upper)
            {
            }

            //! get a virtual X array
            inline T get_x(unit_t i, const array<T> &X, const unit_t N) const throw()
            {
                assert(N==unit_t(X.size()));
                assert(N>0);
                if(i<1)
                {
                    switch(lower)
                    {

                        case expand_cyclic:
                            return get_x(--i+N,X,N) - (X[N]-X[1]);

                        default:
                            return X[1]+(X[1] - get_x(2-i, X, N));
                    }
                }
                else
                {
                    if(i>N)
                    {
                        switch(upper)
                        {
                            case expand_cyclic:
                                return get_x(++i-N,X,N)+(X[N]-X[1]);

                            default:
                                return X[N] + (X[N] - get_x(N+N-i,X,N));
                        }
                    }
                    else
                        return X[i];
                }
            }


            //! get a virtual Y array
            inline T get_y(unit_t i, const array<T> &Y, const unit_t N) const throw()
            {
                assert(N==unit_t(Y.size()));
                assert(N>0);
                if(i<1)
                {
                    switch(lower)
                    {
                        case expand_zero:
                            return 0;

                        case expand_constant:
                            return Y[1];

                        case expand_cyclic:
                            return get_y(--i+N,Y,N);

                        case expand_odd:
                            return Y[1] - (get_y(2-i,Y,N)-Y[1]);

                        case expand_even:
                            return get_y(2-i,Y,N);
                    }
                }
                else
                {
                    if(i>N)
                    {
                        switch(upper)
                        {
                            case expand_zero:
                                return 0;

                            case expand_constant:
                                return Y[N];

                            case expand_cyclic:
                                return get_y(++i-N,Y,N);

                            case expand_odd:
                                return Y[N] - (get_y(N+N-i,Y,N)-Y[N]);
                                
                            case expand_even:
                                return get_y(N+N-i,Y,N);
                                
                        }
                    }
                    else
                    {
                        return Y[i];
                    }
                }
                // never get here
                return 0;
            }
            
            
            
        private:
            YOCTO_DISABLE_ASSIGN(expand);
            
        };
        
    }
}

#endif
