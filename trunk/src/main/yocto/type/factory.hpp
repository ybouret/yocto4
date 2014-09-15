#ifndef YOCTO_TYPE_FACTORY_INCLUDED
#define YOCTO_TYPE_FACTORY_INCLUDED 1

#include "yocto/type/spec.hpp"
#include "yocto/type/traits.hpp"
#include "yocto/associative/map.hpp"


namespace yocto
{
    namespace hidden
    {
        void factory_reg_failure( const char *interface, const char *spec_name );
        void factory_get_failure( const char *interface, const char *spec_name );
        void factory_null_address(const char *interface);
        
        template <typename INTERFACE,typename TLIST>
        struct factory_settings;
        
        template <typename INTERFACE>
        struct factory_settings<INTERFACE,void>
        {
            typedef void          param;
            typedef INTERFACE * (*creator)(void);
        };
        
        template <typename INTERFACE,typename P1>
        struct factory_settings<INTERFACE,TL1(P1)>
        {
            typedef typename type_traits<P1>::parameter_type param1;
            typedef INTERFACE * (*creator)(param1);
        };
        
        template <typename INTERFACE,typename P1,typename P2>
        struct factory_settings<INTERFACE,TL2(P1,P2)>
        {
            typedef typename type_traits<P1>::parameter_type param1;
            typedef typename type_traits<P2>::parameter_type param2;
            typedef INTERFACE * (*creator)(param1,param2);
        };
        

    }
    
    template <typename INTERFACE,typename TLIST>
    class factory
    {
    public:
        typedef hidden::factory_settings<INTERFACE,TLIST> settings;
        typedef typename settings::creator                creator;
        typedef map<type_spec,creator>                    database;
        typedef typename type_traits<typename tl::safe_type_at<TLIST,0,empty_type>::result>::parameter_type param1;
	typedef typename type_traits<typename tl::safe_type_at<TLIST,1,empty_type>::result>::parameter_type param2;

        inline virtual ~factory() throw() {}
        
        //! default registering
        inline void reg( const std::type_info &ti, creator fn )
        {
            assert(fn);
            const type_spec sp(ti);
            if(!creators.insert(sp,fn))
            {
                hidden::factory_reg_failure(typeid(*this).name(),sp.name());
            }
        }
        
        //! add using
        template <typename T>
        inline void add( creator fn )
        {
            assert(fn);
            reg(typeid(T),fn);
        }
        
        //! zero argument build
        inline INTERFACE * operator()( const std::type_info &t ) const
        {
            return check_addr(get(t)());
        }
        
        
        //! one argument build
        inline INTERFACE * operator()( const std::type_info &t, param1 p1) const
        {
            return check_addr(get(t)(p1));
        }
        
        //! two arguments build
        inline INTERFACE * operator()( const std::type_info &t, param1 p1, param2 p2) const
        {
            return check_addr(get(t)(p1,p2));
        }
        
       
	inline explicit factory() : creators() {}
 
    protected:
        database creators;
        
    private:
        YOCTO_DISABLE_ASSIGN(factory);
        
        INTERFACE * check_addr( INTERFACE *I ) const
        {
            if(!I)
            {
                hidden::factory_null_address(typeid(*this).name());
            }
            return I;
        }
        inline const creator & get( const std::type_info &t ) const
        {
            const type_spec spec(t);
            const creator * ctor = creators.search(spec);
            if(!ctor)
            {
                hidden::factory_get_failure(typeid(*this).name(),spec.name());
            }
            return *ctor;
        }
        
    };
    
}

#endif
