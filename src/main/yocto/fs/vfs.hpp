//! \file

#ifndef YOCTO_VFS_INCLUDED
#define YOCTO_VFS_INCLUDED

#include "yocto/string.hpp"
#include "yocto/functor.hpp"
#include "yocto/container/vslot.hpp"

namespace yocto {
	
	
    class vfs : public object {
    public:
        virtual ~vfs() throw();
        //==================================================================
        //
        // entry API
        //
        //==================================================================
        class entry : public object
        {
        public:
            typedef size_t attribute;          //!< attribute type
            static const attribute no_ent = 0; //!< no entry
            static const attribute is_reg = 1; //!< regular file
            static const attribute is_dir = 2; //!< directory
            static const attribute is_unk = 4; //!< unknown
            
            explicit entry( const string &vfs_path, const vfs &vfs_from );
            virtual ~entry() throw();
            entry( const entry &other );
            
            const string     path;
            const char      *cstr;
            const char      *base_name;
            const char      *extension;
            const bool       link;
            const attribute  attr;
            
            typedef functor<bool,TL1(const entry &)> callback;
            
            bool is_regular()   const throw();
            bool is_directory() const throw();
            bool has_extension( const string &ext ) const throw();
            bool has_extension( const char   *ext ) const throw();
            
        private:
            YOCTO_DISABLE_ASSIGN(entry);
        };
        
        //======================================================================
        //
        // scanner API
        //
        //======================================================================
        class scanner
        : public object
        {
        public:
            virtual ~scanner() throw();
            virtual  const entry * next() = 0;
            
            const vfs    &fs;     //!< owner fs
            const string  folder; //!< directory
            
        protected:
            explicit scanner( const string &dirname, const vfs &owner );

            const entry * make_entry( const char *entry_name );
            
            
        private:
            vslot    data;
            YOCTO_DISABLE_COPY_AND_ASSIGN(scanner);
        };
        
        
        void foreach_in( const string &dirname, entry::callback &on_entry ) const;
        
        
        //==================================================================
        //
        // virtual interface
        //
        //==================================================================
        virtual entry::attribute query_attribute( const string &path, bool &is_link ) const throw() = 0;
        virtual void             create_dir(    const string &dirname, const bool allow_already_exists ) = 0;
        virtual void             remove_dir(    const string &dirname  ) = 0 ;
        virtual void             remove_file(   const string &filename ) = 0;
        virtual scanner *        new_scanner(   const string &dirname ) const = 0;
        virtual uint64_t         get_file_size( const string &path ) const = 0;
        
        //==================================================================
        //
        // non virtual interface
        //
        //==================================================================
        void         create_sub_dir( const string &dirname );
        bool         is_reg( const string &path ) const throw();
        bool         is_dir( const string &path ) const throw();
        void         remove_files( const string &dirname, entry::callback &filter );
        void         remove_files( const string &dirname, bool (*filter)( const entry &) );
        void         remove_files_with_extensions( const string &dirname, const string &extension);
        
        //==================================================================
        //
        // utilities
        //
        //==================================================================
        static const char * get_base_name( const char *buffer, size_t buflen ) throw();
        static const char * get_extension(  const char *buffer, size_t buflen ) throw();
        static const char * get_file_dir( size_t &dirlen, const char *buffer, size_t buflen ) throw();
        
        static const char * get_base_name( const char   *path ) throw();
        static const char * get_base_name( const string &path ) throw();
        
        static const char * get_extension( const char   *path ) throw();
        static const char * get_extension( const string &path ) throw();
        
        static const char * get_file_dir( size_t &dirlen, const char   *path) throw();
        static const char * get_file_dir( size_t &dirlen, const string &path) throw();
        
        static string get_file_dir( const char   *path, size_t pathlen );
        static string get_file_dir( const char   *path );
        static string get_file_dir( const string &path );
        
        
        static string &as_directory( string &path );
        static string  to_directory( const string &path );
        static void    change_extension( string &path, const string &ext );
        static void    remove_extension( string &path ) throw();
        
    protected:
        explicit vfs() throw();
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(vfs);
    };
	
}

#endif
