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
        const size_t imin;
        const size_t imax;
        
        static size_t SizeOf(size_t __imin, size_t __imax);
        static void   Check(size_t __imin,size_t __imax);
    protected:
        explicit ITableau(size_t __imin, size_t __imax);
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(ITableau);
    };
    
    template <typename T>
    class TableauOf : public ITableau
    {
    public:
        YOCTO_ARGUMENTS_DECL_T;
        
        
        inline type       & operator[](size_t i) throw()       { assert(i>=imin); assert(i<=imax); return addr[i]; }
        inline const_type & operator[](size_t i) const throw() { assert(i>=imin); assert(i<=imax); return addr[i]; }
        
        inline virtual ~TableauOf() throw() {}
        
        inline explicit TableauOf(size_t      __imin,
                                  size_t      __imax,
                                  mutable_type *base ) throw() :
        ITableau(__imin,__imax),
        addr(base-imin)
        {
            assert(imax>=imin);
            assert(base!=0);
        }
        
    protected:
        mutable_type *addr;
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(TableauOf);
    };
    
    template <typename T>
    class Tableau1D : public TableauOf<T>
    {
    public:
        YOCTO_ARGUMENTS_DECL_T;
        
        explicit Tableau1D(size_t   __imin,
                           size_t   __imax,
                           param_type args ) :
        TableauOf<T>(__imin,
                     __imax,
                     (mutable_type *)memory::global::__calloc( ITableau::SizeOf(__imin,__imax), sizeof(T)))
        {
            size_t i=this->imin;
            try
            {
                while(i<this->imax)
                {
                    new (this->addr+i) mutable_type(args);
                    ++i;
                }
            }
            catch(...)
            {
                for(size_t j=this->imin;j<i;++j)
                {
                    this->addr[j].~mutable_type();
                }
                kill();
                throw;
            }
        }
        
        
        virtual ~Tableau1D() throw()
        {
            for(size_t i=this->imin;i<=this->imax;++i)
            {
                this->addr[i].~mutable_type();
            }
            kill();
        }
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(Tableau1D);
        inline void kill() throw() { memory::global::__free(this->addr+=this->imin); }
    };
    
    
}

#endif
