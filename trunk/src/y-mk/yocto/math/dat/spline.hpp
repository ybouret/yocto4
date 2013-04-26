#ifndef YOCTO_MATH_DAT_SPLINE_INCLUDED
#define YOCTO_MATH_DAT_SPLINE_INCLUDED 1

#include "yocto/sequence/lw-arrays.hpp"

namespace yocto {

	namespace math {
        
        enum spline_type
        {
            spline_natural,       //!< both sides are natural
            spline_periodic,      //!< both sides are periodic
            spline_tangent_left,  //!< left tangent, natural at right
            spline_tangent_right, //!< right tangent, natural at left
            spline_tangent_both   //!< both tangents are set
        };
        
        
#if 0
        enum spline_type
        {
            spline_regular,
            spline_cyclic
        };
        
        template <class T>
		class spline : public object
		{
        public:
            class boundary
            {
            public:
                
                const bool natural;
                const T    slope;
                inline boundary(bool is_natural, const T user_slope = 0) throw() :
                natural(is_natural), slope(user_slope) {}
                              
                inline ~boundary() throw() {}
                
                boundary( const boundary &b ) throw() :
                natural(b.natural), slope(b.slope) {}
                
                
            private:
                YOCTO_DISABLE_ASSIGN(boundary);
            };
            
            class boundaries
            {
            public:
                const spline_type type;
                const boundary    lower; //!< meaningless if type is cyclic
                const boundary    upper; //!< meaningless if type is cyclic
                
                //! regular spline with given boundaries
                inline boundaries( const boundary lo, const boundary up ) throw() :
                type( spline_regular ),
                lower(lo),
                upper(up)
                {
                }
                
                //! regular spline with given boundaries parameters
                inline boundaries( bool lo_natural, T lo_slope, bool up_natural, T up_slope) throw() :
                type( spline_regular ),
                lower(lo_natural,lo_slope),
                upper(up_natural,up_slope)
                {
                }
                
                //! quick spline setup
                /**
                 \param t spline_cyclic => cyclic spline / spline_regular => doubly natural spline
                 */
                inline boundaries( const spline_type t ) throw() :
                type( t ),
                lower(true),
                upper(true)
                {
                }
                
                
                inline ~boundaries() throw() {}
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(boundaries);
            };
            
            explicit spline( const array<T> &X, const array<T> &Y, const boundaries & );
            virtual ~spline() throw();
            
            T operator()(T X) const throw();

            
        private:
            const spline_type type;
            lw_arrays<T,memory::global> arrays;
            lw_array<T> &x;
            lw_array<T> &y;
            lw_array<T> &y2;
            const size_t n;
            const T      xlo;
            const T      xup;
            const T      width;
            //! compute once x/y are set
            void compute(const boundaries &);
        };
#endif
        
	}

}


#endif
