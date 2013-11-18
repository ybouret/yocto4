#ifndef YOCTO_AQ_SPECIES_INCLUDED
#define YOCTO_AQ_SPECIES_INCLUDED 1

#include "yocto/ptr/intr.hpp"
#include "yocto/counted.hpp"
#include "yocto/string.hpp"
#include "yocto/associative/set.hpp"

namespace yocto
{
    namespace aqueous
    {
        
        //! basic common info for a species
        /**
         plus extra space
         */
        class species : public object, public counted
        {
        public:
            
            explicit species(const string &id,
                             const int    charge,
                             size_t       extra_bytes   = 0
                             );
            
            explicit species( const char *id, const int charge, size_t extra_bytes = 0 );
            
            virtual ~species() throw();
            
            const string name; //!< unique name
            const int    z;    //!< algebraic charge
            
            
            typedef intr_ptr<string,species> ptr;
            typedef set<string,ptr>          db;
            typedef db::const_iterator       iterator;
            
            const string & key() const throw();
            
            template <typename T>
            static inline void __kill( void *addr ) throw() { assert(addr); static_cast<T*>(addr)->~T(); }
            
            template <typename T>
            inline void make()
            {
                assert(blen>=sizeof(T));
                assert(addr!=0);
                assert(kill==0);
                new (addr) T();
                kill = __kill<T>;
            }
            
            template <typename T, typename U>
            inline void make( typename type_traits<U>::parameter_type args)
            {
                assert(blen>=sizeof(T));
                assert(addr);
                new (addr) T(args);
                kill = __kill<T>;
            }
            
            template <typename T>
            inline T &get() const throw() { assert(blen>=sizeof(T)); assert(addr!=0); return *(T*)addr; }
            
            
            
        private:
            size_t blen;
            void  *addr;
            void (*kill)(void *);
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(species);
        };
        
    }
    
}

#endif
