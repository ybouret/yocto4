#ifndef YOCTO_FAME_ARRAY1D_INCLUDED
#define YOCTO_FAME_ARRAY1D_INCLUDED 1

#include "yocto/fame/layout.hpp"
#include "yocto/fame/linear.hpp"

namespace yocto
{
    
    namespace fame
    {
        
        template <size_t,typename> struct array_for;

        template <typename T>
        class array1D : public linear<T,layout1D>
        {
        public:
            YOCTO_ARGUMENTS_DECL_T;
            typedef layout1D              layout_type;
            typedef linear<T,layout_type> linear_type;
            typedef array1D<T>            varray_type;
            
            inline explicit array1D(const string     &user_name,
                                    const layout_type L,
                                    void *            user_data = 0) :
            linear_type(user_name,L,user_data),
            addr( this->entry - this->lower )
            {
            }
            
            
            inline virtual ~array1D() throw()
            {
            }
            
            inline type & operator[]( unit_t i ) throw()
            {
                assert(i>=this->lower);
                assert(i<=this->upper);
                return addr[i];
            }
            
            inline const_type & operator[]( unit_t i ) const throw()
            {
                assert(i>=this->lower);
                assert(i<=this->upper);
                return addr[i];
            }
            
            inline std::ostream & display( std::ostream &os ) const
            {
                os << '[';
                for(unit_t i=this->lower;i<=this->upper;++i)
                {
                    os << ' ' << (*this)[i];
                }
                os << ']';
                return os;
            }
            
                      
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(array1D);
            type *addr;
            virtual void                 *get_handle() const throw() { return (void*)this; }
            virtual const std::type_info &get_typeid() const throw() { return typeid(varray_type); }
        };
        
        template <typename T> struct array_for<1,T> { typedef array1D<T> type; };
        
    }
    
}

#endif
