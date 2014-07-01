#ifndef YOCTO_TABLEAU1D_INCLUDED
#define YOCTO_TABLEAU1D_INCLUDED 1

#include "yocto/memory/global.hpp"
#include "yocto/type/args.hpp"

namespace yocto
{
    
    
    class ITableau
    {
    public:
        virtual ~ITableau() throw();
        const size_t cmin;
        const size_t cmax;
        const size_t cols;
        
    protected:
        explicit ITableau(size_t imin, size_t imax) throw();
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(ITableau);
    };
    
    template <typename> class Tableau2D;
    
    template <typename T>
    class TableauOf : public ITableau
    {
    public:
        YOCTO_ARGUMENTS_DECL_T;
        
        
        inline type       & operator[](size_t i) throw()       { assert(i>=cmin); assert(i<=cmax); return addr[i]; }
        inline const_type & operator[](size_t i) const throw() { assert(i>=cmin); assert(i<=cmax); return addr[i]; }
        
        inline virtual ~TableauOf() throw() {}
        
        
    protected:
        mutable_type *addr;
        inline explicit TableauOf(size_t      imin,
                                  size_t      imax) throw() :
        ITableau(imin,imax), addr(0) {}
      
        inline void attach(mutable_type *base) throw()
        {
            assert(base);
            assert(!addr);
            addr = base-cmin;
        }
        
        friend class Tableau2D<T>;
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(TableauOf);
    };
    
    template <typename T>
    class Tableau1D : public TableauOf<T>
    {
    public:
        YOCTO_ARGUMENTS_DECL_T;
        
        explicit Tableau1D(size_t   imin,
                           size_t   imax,
                           param_type args ) :
        TableauOf<T>(imin,
                     imax)
        {
            this->attach( (mutable_type *)memory::global::__calloc( this->cols,sizeof(T)));
            size_t i=this->cmin;
            try
            {
                while(i<this->cmax)
                {
                    new (this->addr+i) mutable_type(args);
                    ++i;
                }
            }
            catch(...)
            {
                for(size_t j=this->cmin;j<i;++j)
                {
                    this->addr[j].~mutable_type();
                }
                kill();
                throw;
            }
        }
        
        
        virtual ~Tableau1D() throw()
        {
            for(size_t i=this->cmin;i<=this->cmax;++i)
            {
                this->addr[i].~mutable_type();
            }
            kill();
        }
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(Tableau1D);
        inline void kill() throw() { memory::global::__free(this->addr+=this->cmin); }
    };
    
    
}

#endif