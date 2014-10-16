#ifndef YOCTO_VISIT_INCLUDED
#define YOCTO_VISIT_INCLUDED 1

#include <VisItControlInterface_V2.h>
#include <VisItDataInterface_V2.h>

#include "yocto/mpi/mpi.hpp"
#include "yocto/memory/buffers.hpp"

namespace yocto
{
    //! libSim wrapper, assuming compiled with MPI...
    class VisIt
    {
    public:
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
        // Setup API
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
        static void SetupParallel(const mpi    &MPI,
                                  const string &sim_name,
                                  const string &sim_comment,
                                  const string &sim_path,
                                  const string *sim_ui = 0
                                  );
        
        
        //======================================================================
        //
        // Simulation class
        //
        //======================================================================
        class Simulation
        {
        public:
            typedef memory::buffer_of<char,memory::global> IOBuffer;
            static  const size_t                           IOLength = 1024;
            
            explicit Simulation( const mpi &, bool useConsole=true );
            virtual ~Simulation() throw();
            
            //__________________________________________________________________
            //
            // dynamic variables
            //__________________________________________________________________
            int      cycle;    //!< modified by main loop
            int      runMode;  //!< halted/runningm initially halted
            double   runTime;  //!< user run time
            bool     done;     //!< initially true, stop when user set false
            IOBuffer iobuff;
            
            //__________________________________________________________________
            //
            // constant info
            //__________________________________________________________________
            const bool console;  //!< use the interactive console ?
            const mpi &MPI;      //!< the MPI instance
            const int  par_size; //!< the comm world size
            const int  par_rank; //!< the comm world rank
            const int  par_last; //!< the comm world last
            const bool parallel; //!< par_size>0
            const bool is_first; //!< 0 == par_rank
            const bool is_final; //!< par_last == par_rank
            
            
            //__________________________________________________________________
            //
            // methods to interact with VisIt
            //__________________________________________________________________
            //! adds a generic command to UI
            void add_generic_command( const string &name, visit_handle &md ) const;
            
            //! adds a generic command to UI
            void add_generic_command( const char   *name, visit_handle &md ) const;
            
            
            //! provide meta data information
            /**
             meta data for meshes, specific commands...
             default does nothing.
             */
            virtual void get_meta_data( visit_handle &md ) const;

            
            
            //__________________________________________________________________
            //
            // handling user or UI commands
            //__________________________________________________________________
            virtual void execute(const string cmd, const array<string> &args);
            
            //__________________________________________________________________
            //
            // Generic UI commands
            //__________________________________________________________________
            static const char * GenericCommandReg[];
            static const size_t GenericCommandNum;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Simulation);
            
            //! intercept generic commands, then call execute
            void execute_all(const string cmd, const array<string> &args );
            
            friend class   VisIt;
        };
        
        //======================================================================
        //
        // RunTime API
        //
        //======================================================================
        static void Loop( Simulation &sim );
        static void Execute( Simulation &sim, const string &code );
        
        
    };
    
}

#endif
