#ifndef YOCTO_VISIT_INCLUDED
#define YOCTO_VISIT_INCLUDED 1

#if 0
#include <VisItControlInterface_V2.h>
#include <VisItDataInterface_V2.h>

#include "yocto/mpi/mpi.hpp"
#include "yocto/string.hpp"
#include "yocto/memory/buffers.hpp"
#include "yocto/sequence/vector.hpp"

namespace yocto
{
    
    //! VisIt wrapper
    class VisIt
    {
    public:
        
        //! auto open/close trace file
        class TraceFile
        {
        public:
            //! only on master node
            TraceFile( int rank, const string &filename );
            virtual ~TraceFile() throw();
        private:
            const int rank_;
            YOCTO_DISABLE_COPY_AND_ASSIGN(TraceFile);
        };
        
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
        
        //! I/O memory buffer type
        typedef memory::buffer_of<char,memory::global> IOBuffer;
        static const size_t                            IOBufferSize = 1024; //!< default length
        
        
        //! base class for a simulation
        class Simulation
        {
        public:
            explicit Simulation( const mpi & );
            virtual ~Simulation() throw();
            
            const mpi &MPI;       //!< reference to the MPI singleton
            int        cycle;     //!< number of cycles, upgraded by MainLoop
            int        runMode;   //!< stopped or running
            double     runTime;   //!< should be cycle * dt
            bool       done;      //!< end of simulation flag
            IOBuffer   iobuff;    //!< buffer for console input
            double     stepTime;  //!< last wall time for step
            double     loopTime;  //!< stepTime + VisIt time
            unsigned   commTime;  //!< from MPI, in microseconds
            bool       console;   //!< shall use the interactive console
            const int  par_rank;  //!< alias MPI.CommWorldRank
            const int  par_size;  //!< alias MPI.CommWorldSize
            const int  par_last;  //!< alias MPI.CommWorldLast
            const bool parallel;  //!< alias for MPI.IsParallel
            const bool is_first;  //!< 0 == par_rank
            const bool is_final;  //!< par_last == par_rank
            
            //! provide meta data information
            /**
             meta data for meshes, specific commands...
             default does nothing.
             */
            virtual void get_meta_data( visit_handle &md ) const;
            
            //! add a generic command
            /**
             VisIt wrapper to add a generic command to the simulation control.
             */
            void add_generic_command( const string &cmd, visit_handle &md ) const;
            
            //! add a generic command
            void add_generic_command( const char   *cmd, visit_handle &md ) const;
            
            //! provide mesh
            /**
             user's mesh data
             */
            virtual visit_handle get_mesh( int domain, const string &name ) const;
            
            //! provide variable
            /**
             user's variable data
             */
            virtual visit_handle get_variable( int domain, const string &name ) const;
            
            //! provide curve
            /**
             user's curve data
             */
            virtual visit_handle get_curve( const string &name ) const;
            
            
            //! if you want to display some info before step
            /**
             cycle is updated before the call.
             */
            virtual void step_prolog();
            
            //! the main step function
            /**
             called by Visit::OneStep
             */
            virtual void step();
            
            //! process the command to update the simulation
            virtual void perform( const string &cmd, const array<string> &args);
            
            static const char  *GenericCommandReg[];
            static const size_t GenericCommandNum;
            
            //! if you want statistics after one step
            virtual void step_epilog();
            
            //! commands arguments
            void   no_cmd_args() throw();
            void   add_cmd_arg( const string &);
            void   add_cmd_arg( const char   *);
            
            //! return first string, fill cmdArgs array
            /**
             format :'cmd arg1;arg2;arg3;argN'
             */
            string parse_user_command( const string &user_cmd);
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Simulation);
            void   performAlways( const string &user_cmd );
            friend class VisIt;
            vector<string> cmdArgs;
        };
        
        //! handle visit I/O for simulation
        static void MainLoop(Simulation &sim );
        
        //! perform one step when possible
        /**
         sim.cycle++;
         sim.step_prolog();
         sim.step();
         sim.step_epilog();
         */
        static void OneStep( Simulation &sim );
        
        //! call sim.performAlways() and propagate to VisIt.
        static void Perform( Simulation &sim, const string &user_cmd );
    };
    
    
    
}
#endif

#endif
