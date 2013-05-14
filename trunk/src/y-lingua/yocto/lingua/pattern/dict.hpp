#ifndef YOCTO_LINGUA_PATTERN_DICT_INCLUDED
#define YOCTO_LINGUA_PATTERN_DICT_INCLUDED 1

#include "yocto/lingua/pattern.hpp"
#include "yocto/associative/map.hpp"

namespace yocto
{
    namespace lingua
    {
        
        class p_dict : public object
        {
        public:
            explicit p_dict() throw();
            virtual ~p_dict() throw();
            
            void insert( const string &id, pattern *p);
            void insert( const char   *id, pattern *p);
            
            bool has( const string &id ) const throw();
            bool has( const char   *id ) const;
            
            pattern *create( const string &id ) const;
            pattern *create( const char   *id ) const;
            
        private:
            typedef pattern  *motif;
            p_list            patterns;
            map<string,motif> db;
            YOCTO_DISABLE_COPY_AND_ASSIGN(p_dict);
        };
        
    }
    
}

#endif

