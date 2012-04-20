#ifndef YOCTO_VISIT_INCLUDED
#define YOCTO_VISIT_INCLUDED 1

#include <VisItControlInterface_V2.h>
#include <VisItDataInterface_V2.h>
#include "yocto/mpi/mpi.hpp"
#include "yocto/string.hpp"

namespace yocto 
{
    
    class visit   : public singleton<visit>
    {
    public:
        
        class simulation
        {
        public:
            explicit simulation() throw();
            virtual ~simulation() throw();
            
            int  cycle;
            int  runMode;
            bool done;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(simulation);
        };
        
        //! setup visit AND mpi
        static visit &SetupEnvironment( int *argc, 
                                       char ***argv,
                                       const string &sim_name,
                                       const string &sim_comment,
                                       const string &sim_path  );
        const int  rank;
        const int  size;
        const bool is_parallel;
        const bool is_master;
        
        void MainLoop( simulation &sim );
        
        
        
    private:
        visit();
        virtual ~visit() throw();
        YOCTO_DISABLE_COPY_AND_ASSIGN(visit);
        static const threading::longevity life_time = 1;       //!< TODO: set to a better value, greater than MPI
		static const char                 name[];
        friend class singleton<visit>;
    };
    
    
}

#endif
