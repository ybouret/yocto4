#ifndef YOCTO_WORKING_DIRECTORY_INCLUDED
#define YOCTO_WORKING_DIRECTORY_INCLUDED 1

#include "yocto/string.hpp"
#include "yocto/threading/singleton.hpp"

namespace yocto
{
    class working_directory : public singleton<working_directory>
    {
    public:
        string get() const;
        void   set( const string & ) const;
        
        class change_lock
        {
        public:
            change_lock( const string &target, bool *status );
            change_lock( const char   *target, bool *status );
            ~change_lock() throw();
            
        private:
            const string source_;
            bool        *status_;
            YOCTO_DISABLE_COPY_AND_ASSIGN(change_lock);
        };
        
    private:
        explicit working_directory() throw();
        virtual ~working_directory() throw();
        
        YOCTO_DISABLE_COPY_AND_ASSIGN(working_directory);
        friend class singleton<working_directory>;
        
        static const threading::longevity life_time = object::provider::life_time - 9; // TODO: check this value
        static const char name[];
    };
    
    typedef volatile working_directory::change_lock chdir_lock;
    
    	
}

#endif
