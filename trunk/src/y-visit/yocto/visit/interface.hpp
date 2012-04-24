#ifndef YOCTO_VISIT_INCLUDED
#define YOCTO_VISIT_INCLUDED 1

#include <VisItControlInterface_V2.h>
#include <VisItDataInterface_V2.h>

#include "yocto/mpi/mpi.hpp"
#include "yocto/string.hpp"
#include "yocto/memory/buffers.hpp"

namespace yocto 
{
    
    class VisIt
    {
    public:
        
        static void OpenTraceFile( const string &filename ); //!< VisItOpenTraceFile
        static void SetDirectory( const string &path );      //!< VisItSetDirectory
        static void SetOption(const string &options);        //!< VisItSetOptions
        static void SetupEnvironment(); //!< VisItSetupEnvironment
        static void SetupParallel( mpi &        MPI, 
                                  const string &sim_name,
                                  const string &sim_comment,
                                  const string &sim_path
                                  ); //!< auxiliary functions
        
        typedef memory::buffer_of<char,memory::global> IOBuffer;
        static const size_t                            IOBufferSize = 1024;
        
        
        
        class Simulation
        {
        public:
            explicit Simulation( mpi &MPI );
            virtual ~Simulation() throw();
            
            int      cycle;
            int      runMode;
            bool     done;
            bool     isConnected;
            IOBuffer iobuff;
            const int par_rank;
            const int par_size;
            
            
            virtual void step();
            virtual void perform( const string &cmd );
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Simulation);
            bool performAlways( const string &cmd );
            friend class VisIt;
        };
        
        static void MainLoop( mpi &MPI, Simulation &sim, bool WithConsole = true );
        static void OneStep( Simulation &sim );
        static void Perform( Simulation &sim, const string &cmd );
    };
    
    
    
}

#endif
