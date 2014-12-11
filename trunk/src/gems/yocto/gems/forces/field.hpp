#ifndef YOCTO_GEMS_FORCES_FIELD_INCLUDED
#define YOCTO_GEMS_FORCES_FIELD_INCLUDED 1

#include "yocto/gems/forces/unary.hpp"
#include "yocto/gems/forces/binary.hpp"

namespace yocto
{
    namespace gems
    {
        
        template <typename T>
        class force_field
        {
        public:
            typedef typename unary_force<T>::pointer  unary_force_ptr;
            typedef typename binary_force<T>::pointer binary_force_ptr;

            explicit force_field() throw() :
            unary_forces(),
            binary_forces()
            {}
            virtual ~force_field() throw() {}
            
            vector<unary_force_ptr>  unary_forces;
            vector<binary_force_ptr> binary_forces;
            
            void append( unary_force<T> *f )
            {
                const unary_force_ptr p(f);
                unary_forces.push_back(p);
            }
            
            void append( binary_force<T> *f )
            {
                const binary_force_ptr p(f);
                binary_forces.push_back(p);
            }
            
            void reset( atoms<T> &a )
            {
                for(size_t i=a.size();i>0;--i)
                {
                    a[i]->f.ldz();
                }
            }
            
            //! add unary and binary forces
            T collect( atoms<T> &a )
            {
                T Ep = 0;
                const size_t n1 = unary_forces.size();
                const size_t n2 = binary_forces.size();
                for(size_t i=a.size();i>0;--i)
                {
                    atom<T> &ai = *a[i];
                    for(size_t k=n1;k>0;--k)
                    {
                        Ep += (*unary_forces[k])(ai);
                    }
                    
                    for(size_t j=i-1;j>0;--j)
                    {
                        for(size_t k=n2;k>0;--k)
                        {
                            Ep += (*binary_forces[k])(ai,*a[j]);
                        }
                    }
                }
                return Ep;
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(force_field);
            
        };
    }
    
}


#endif

