#include "yocto/math/ztype.hpp"
#include "yocto/math/polynomial.hpp"
#include "yocto/math/types.hpp"
#include <iostream>
#include "yocto/code/ipower.hpp"

namespace yocto
{
    namespace math
    {
        
        template <>
        polynomial<z_type>::term:: ~term() throw()
        {
            
        }
        
        template <>
        polynomial<z_type>::term:: term(size_t coef, z_type value) throw() :
        p(coef),
        a(value),
        next(0),
        prev(0)
        {
            
        }
        
        
        template <>
        polynomial<z_type>:: polynomial() throw() : terms()
        {
        }
        
        template <>
        polynomial<z_type>:: ~polynomial() throw()
        {
        }
        
        template <>
        size_t polynomial<z_type>:: degree() const throw()
        {
            return terms.size ? terms.tail->p : 0;
        }
        
        
        template <>
        void polynomial<z_type>:: add(const size_t coef, const z_type value)
        {
            if(Fabs(value)<=0)
                return;
            
            
            for(term *cur = terms.head;cur;cur=cur->next)
            {
                const size_t p = cur->p;
                
                if(p==coef)
                {
                    cur->a += value;
                    if(Fabs(cur->a)<=0)
                    {
                        delete terms.unlink(cur);
                    }
                    return;
                }
                
                if(p>coef)
                {
                    //insert a new node before term
                    term *New = new term(coef,value);
                    if(terms.size==1||cur==terms.head)
                    {
                        terms.push_front(New);
                    }
                    else
                    {
                        term *prv = cur->prev; assert(prv);
                        prv->next = New;
                        cur->prev = New;
                        New->prev = prv;
                        New->next = cur;
                        terms.size++;
                    }
                    return;
                }
                
                assert(p<coef);
            }
            
            // append a a new node
            terms.push_back( new term(coef,value) );
        }
        
        template <>
        void polynomial<z_type>:: clear() throw()
        {
            terms.clear();
        }
        
        template <>
        void polynomial<z_type>:: output(std::ostream &os) const
        {
            if(terms.size)
            {
                os << "(";
                for(const term *t = terms.head;t;t=t->next)
                {
                    if(t!=terms.head) os << "+";
                    os << "(" << t->a << ")";
                    if(t->p>0)
                    {
                        os << "*(x";
                        for(size_t j=t->p-1;j>0;--j)
                            os << "*x";
                        os << ")";
                    }
                }
                os << ")";
            }
            else
                os << "(0)";
        }
        
        template <>
        polynomial<z_type>:: polynomial( const polynomial &other ) :
        terms()
        {
            for(const term *t = other.terms.head;t;t=t->next)
            {
                terms.push_back( new term(t->p,t->a) );
            }
        }
        
        template <>
        void polynomial<z_type>::xch( polynomial &other ) throw()
        {
            terms.swap_with(other.terms);
        }
        
        template <>
        polynomial<z_type> & polynomial<z_type>::operator=( const polynomial &other )
        {
            if(this!=&other)
            {
                polynomial tmp(other);
                xch(tmp);
            }
            return *this;
        }
        
        template <>
        polynomial<z_type> polynomial<z_type>:: add( const polynomial &lhs, const polynomial &rhs )
        {
            polynomial P;
            
            for(const term *l=lhs.terms.head;l;l=l->next)
            {
                P.add(l->p,l->a);
            }
            
            for(const term *r=rhs.terms.head;r;r=r->next)
            {
                P.add(r->p,r->a);

            }
            
            return P;
        }
        
        
        template <>
        polynomial<z_type> polynomial<z_type>:: sub( const polynomial &lhs, const polynomial &rhs )
        {
            polynomial P;
            
            for(const term *l=lhs.terms.head;l;l=l->next)
            {
                P.add(l->p,l->a);
            }
            
            for(const term *r=rhs.terms.head;r;r=r->next)
            {
                P.add(r->p,-r->a);
                
            }
            
            return P;
        }

        
        template <>
        polynomial<z_type> polynomial<z_type>:: mul( const polynomial &lhs, const polynomial &rhs )
        {
            polynomial P;
            for(const term *l=lhs.terms.head;l;l=l->next)
            {
                for(const term *r=rhs.terms.head;r;r=r->next)
                {
                    P.add(l->p+r->p, (l->a*r->a) );
                }
            }
            return P;
        }
        
        template <>
        polynomial<z_type> polynomial<z_type>:: mul( const polynomial &lhs, z_type a )
        {
            polynomial P;
            for(const term *l=lhs.terms.head;l;l=l->next)
            {
                P.add(l->p,a*l->a);
            }
            return P;
        }
        
        template <>
        polynomial<z_type> polynomial<z_type>::derivative(const polynomial &other)
        {
            polynomial P;
            for(const term *t=other.terms.head;t;t=t->next)
            {
                if(t->p>0)
                {
                    P.add(t->p-1,z_type(real_t(t->p))*t->a);
                }
            }
            return P;
        }
        
        template <>
        polynomial<z_type> polynomial<z_type>::primitive(const polynomial &other)
        {
            polynomial P;
            for(const term *t=other.terms.head;t;t=t->next)
            {
                const size_t q = t->p+1;
                P.add(q,t->a/(z_type(real_t(q))));
            }
            
            return P;
        }
        template <>

        z_type polynomial<z_type>:: operator()( z_type X ) const throw()
        {
            z_type ans(0);
            for(const term *t=terms.head;t;t=t->next)
            {
                ans += t->a * ipower(X,t->p);
            }
            return ans;
        }

        
    }
}
