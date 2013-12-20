#ifndef YOCTO_LINGUA_PATTERN_DICT_INCLUDED
#define YOCTO_LINGUA_PATTERN_DICT_INCLUDED 1

#include "yocto/lingua/pattern.hpp"
#include "yocto/associative/map.hpp"

namespace yocto
{
    namespace lingua
    {
        //______________________________________________________________________
        //
        //! pattern dictionay
        //______________________________________________________________________
        class p_dict : public object
        {
        public:
            explicit p_dict() throw();
            virtual ~p_dict() throw();
            
            //! insert a pattern, taken care of
            void insert( const string &id, pattern *p);
            
            //! insert a pattern, taken care of
            void insert( const char   *id, pattern *p);
            
            //! compile the pattern from expression and insert it
            void insert( const string &id, const string &expr);
            
            //! compile the pattern from expression and insert it
            void insert( const char   *id, const char   *expr);
            
            //! lookup by name
            bool has( const string &id ) const throw();
            
            //! lookup by name
            bool has( const char   *id ) const;
            
            //! create by name, throw on error
            pattern *create( const string &id ) const;
            
            //! create by name, throw on error
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

