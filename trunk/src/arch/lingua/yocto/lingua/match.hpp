#ifndef YOCTO_LINGUA_MATCH_INCLUDED
#define YOCTO_LINGUA_MATCH_INCLUDED 1

#include "yocto/lingua/source.hpp"
#include "yocto/lingua/pattern/dict.hpp"

#include "yocto/ptr/auto.hpp"

namespace yocto
{
    namespace lingua
    {
        
        class match
        {
        public:
            enum mode_type
            {
                exactly,
                partly
            };
            
            match() throw();
            match(const string &expr);
            match(const char   *expr);
            match( const match &other);
            
            virtual ~match() throw();
            
            void load( pattern *p ) throw();
            void load( const string &expr, p_dict *dict = 0 );
            void load( const char   *expr, p_dict *dict = 0 );
            
            match & operator=( const string & );
            match & operator=( const char   * );
            match & operator=( const match &other );
            
            //! returns NULL is no pattern is allocated
            pattern *safe_clone() const;
            
            bool operator()( const memory::ro_buffer &buf,
                            mode_type flag );
            
            string last_accepted() const;
            
        private:
            auto_ptr<pattern> ptr;
            source            src;
            bool              __find( const memory::ro_buffer &buf, size_t shift);
        };
    }
}

#endif
