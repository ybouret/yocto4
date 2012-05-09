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
        
        //! auto open/close trace file
        class TraceFile
        {
        public:
            TraceFile( int rank, const string &filename );
            virtual ~TraceFile() throw();
        private:
            const int rank_;
            YOCTO_DISABLE_COPY_AND_ASSIGN(TraceFile);
        };
        
        static void SetDirectory( const string &path );      //!< VisItSetDirectory
        static void SetOption(const string &options);        //!< VisItSetOptions
        static void SetupParallel(const mpi &   MPI, 
                                  const string &sim_name,
                                  const string &sim_comment,
                                  const string &sim_path,
                                  const string *sim_ui = NULL
                                  ); //!< auxiliary functions
        
        typedef memory::buffer_of<char,memory::global> IOBuffer;
        static const size_t                            IOBufferSize = 1024;
        
        
        
        class Simulation
        {
        public:
            explicit Simulation( const mpi & );
            virtual ~Simulation() throw();
            
            int        cycle;
            int        runMode;
            bool       done;
            IOBuffer   iobuff;    //!< buffer for console input
            const mpi &MPI;       //!< MPI singleton reference
            const bool console;   //!< shall use the interactive console
            const int  par_rank;  //!< alias MPI.CommWorldRank 
            const int  par_size;  //!< alias MPi.CommWorldSize
            const bool parallel;  //!< par_size > 1
            const bool master;    //!< 0 == par_rank
            
            //! provide meta data information
            virtual void get_meta_data( visit_handle &md ) const;
            
            //! provide mesh
            virtual visit_handle get_mesh( int domain, const string &name ) const;
            
            //! provide variable
            virtual visit_handle get_variable( int domain, const string &name ) const;
            
            //! the step function
            virtual void step();  
            
            //! process the command to update the simulation
            virtual void perform( const string &cmd ); 
            
            static const char  *GenericCommandReg[];
            static const size_t GenericCommandNum;
            
            void invite() const;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Simulation);
            void   performAlways( const string &cmd );
            friend class VisIt;
        };
        
        static void MainLoop(Simulation &sim );
        static void OneStep( Simulation &sim );
        static void Perform( Simulation &sim, const string &cmd );
    };
    
    
    
}

#endif
