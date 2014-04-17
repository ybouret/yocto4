#ifndef YOCTO_MATH_POLYNOMIAL_INCLUDED
#define YOCTO_MATH_POLYNOMIAL_INCLUDED 1

#include "yocto/core/list.hpp"
#include "yocto/object.hpp"
#include <iosfwd>

namespace yocto
{
    
    namespace math
    {
        
        template <typename T>
        class polynomial
        {
        public:
            class term : public object
            {
            public:
                const size_t p;
                T            a;
                
                explicit term(size_t coef,T value) throw();
                virtual ~term() throw();
                term *next;
                term *prev;

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(term);
            };
            
            size_t degree() const throw();
            
            explicit polynomial() throw();
            virtual ~polynomial() throw();
            
            void clear() throw();
            
            void add(const size_t coef, const T value);
            
            void output( std::ostream &) const;
            
            inline friend std::ostream & operator<<(std::ostream &s, const polynomial &p)
            {
                p.output(s);
                return s;
            }
            
            polynomial( const polynomial &other );
            void xch( polynomial &other ) throw();
            polynomial & operator=( const polynomial &other );
            
            
            static polynomial add( const polynomial &lhs, const polynomial &rhs );
            
            inline friend polynomial operator+( const polynomial &lhs, const polynomial &rhs )
            {
                return add(lhs,rhs);
            }
            
            inline polynomial & operator+=( const polynomial &rhs )
            {
                polynomial ans = add(*this,rhs);
                xch(ans);
                return *this;
            }

            inline polynomial operator+( const polynomial &rhs )
            {
                return rhs;
            }
            
            static polynomial sub(const polynomial &lhs, const polynomial &rhs);
           
            inline polynomial & operator-=( const polynomial &rhs )
            {
                polynomial ans = sub(*this,rhs);
                xch(ans);
                return *this;
            }
            
            inline polynomial operator-( const polynomial &rhs )
            {
                polynomial z;
                return sub(z,rhs);
            }

            
            
            static polynomial mul( const polynomial &lhs, const polynomial &rhs );
            static polynomial mul( const polynomial &lhs, T a );
            
            inline friend polynomial operator*( const polynomial &lhs, const polynomial &rhs )
            {
                return mul(lhs,rhs);
            }
            
            inline friend polynomial operator*( const polynomial &lhs, T a )
            {
                return mul(lhs,a);
            }
            
            inline friend polynomial operator*( T a, const polynomial &lhs)
            {
                return mul(lhs,a);
            }
            
            
            inline polynomial & operator*=( const polynomial &rhs )
            {
                polynomial ans = mul(*this,rhs);
                xch(ans);
                return *this;
            }
            
            static polynomial derivative(const polynomial &other);
            static polynomial primitive(const polynomial &other);
            
            T operator()( T X ) const throw();
            
            
        private:
            typedef core::list_of_cpp<term> term_list;
            term_list terms;
        };
        
    }
    
}


#endif
