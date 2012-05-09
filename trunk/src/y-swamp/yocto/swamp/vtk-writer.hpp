#ifndef YOCTO_SWAMP_VTK_WRITER_INCLUDED
#define YOCTO_SWAMP_VTK_WRITER_INCLUDED 1

#include "yocto/swamp/types.hpp"
#include "yocto/ios/ostream.hpp"
#include "yocto/string.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/type-spec.hpp"

namespace yocto 
{
    namespace swamp 
    {
        
        class vtk_format
        {
        public:
            explicit vtk_format();
            virtual ~vtk_format() throw();
            
            string real_fmt; //!< format for real, default is %g
            string unit_fmt; //!< format for unit, default is %ld
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(vtk_format);
        };
        
                
        
        
        class vtk_writer
        {
        public:
            typedef void (*callback)( ios::ostream &fp, const void *data, const vtk_format &fmt );
            
            class record
            {
            public:
                const type_spec spec; //!< identifier
                const string    kind; //!< SCALARS | VECTORS
                const callback  proc; //!< print procedure
                
                const type_spec & key() const throw();
                record( const record & );
                record( const std::type_info &id, const char *k, callback cb );
                
            private:
                YOCTO_DISABLE_ASSIGN(record);
            };
            
            
            explicit vtk_writer();
            virtual ~vtk_writer() throw();
            
            const record & operator[]( const type_spec & ) const;
            
            void prolog( ios::ostream &fp, const string &name, size_t num, const record &r) const;
            void write1( ios::ostream &fp, const void *data, const record &r) const;
            
            
        private:            
            vtk_format              format;
            set<type_spec,record>   out_db;
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(vtk_writer);
        };
        
    }
    
}
#endif

