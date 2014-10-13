#ifndef YOCTO_FAME_VERTICES_INCLUDED
#define YOCTO_FAME_VERTICES_INCLUDED 1

#include "yocto/fame/array1d.hpp"


namespace yocto
{
    namespace fame
    {
        
        template <size_t DIM,typename T>
        class vertices : public array1D< typename vertex_for<DIM,T>::type >
        {
        public:
            typedef typename vertex_for<DIM,T>::type type;
            typedef array1D<type>                    array_type;
            
            explicit vertices(const string     &user_name,
                              const layout1D    L) :
            array_type(user_name,L,0)
            {
            }
            
            virtual ~vertices() throw()
            {
            }
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(vertices);
        };
        
        
    }
}

#endif

