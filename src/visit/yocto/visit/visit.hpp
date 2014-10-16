#ifndef YOCTO_VISIT_INCLUDED
#define YOCTO_VISIT_INCLUDED 1

#include <VisItControlInterface_V2.h>
#include <VisItDataInterface_V2.h>

#include "yocto/mpi/mpi.hpp"


namespace yocto
{
    //! libSim wrapper, assuming compiled with MPI...
    struct VisIt
    {
        
        //======================================================================
        //
        // Auxiliary classes
        //
        //======================================================================
        
        //______________________________________________________________________
        //
        //! dedicated exception class
        //______________________________________________________________________
        class exception : public yocto::exception
        {
        public:
            explicit exception(const char *fmt,...) throw() YOCTO_PRINTF_CHECK(2,3);
            exception(const exception &) throw();
            virtual ~exception() throw();
            
        private:
            YOCTO_DISABLE_ASSIGN(exception);
            char what_[80];
        };
        
        
        //______________________________________________________________________
        //
        //! auto open/close trace file
        //______________________________________________________________________
        class TraceFile
        {
        public:
            //! only on master node
            TraceFile( const mpi &MPI, const string &filename );
            TraceFile( const mpi &MPI, const char   *filename );
            virtual ~TraceFile() throw();
        private:
            const int rank_;
            YOCTO_DISABLE_COPY_AND_ASSIGN(TraceFile);
            void initialize(const string &filename);
        };

        
        //======================================================================
        //
        // API
        //
        //======================================================================
        static void SetDirectory( const string &path );      //!< VisItSetDirectory
        static void SetOption(const string &options);        //!< VisItSetOptions
        
        //! prepare simulation
        /**
         \param MPI         a MPI instance
         \param sim_name    simulation name
         \param sim_comment simulation comment
         \param sim_path    simulation path
         \param sim_ui      optional simulation UI file
         */
        static void SetupParallel(const mpi &   MPI,
                                  const string &sim_name,
                                  const string &sim_comment,
                                  const string &sim_path,
                                  const string *sim_ui = 0
                                  );

        
    };
    
}

#endif
