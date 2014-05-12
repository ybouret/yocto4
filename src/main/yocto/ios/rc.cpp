#include "yocto/ios/rc.hpp"
#include "yocto/exception.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/ios/iflux.hpp"
#include "yocto/ptr/auto.hpp"


#include <iostream>

namespace yocto
{
    namespace ios 
    {
        ////////////////////////////////////////////////////////////////////////
        //
        // resources::packer
        //
        ////////////////////////////////////////////////////////////////////////
        
#define Y_RC_PACK_CTOR()          \
file_( filename, ios::writable ), \
count(0),                         \
start(0),                         \
H(), db(16,as_capacity)
        
        
        resources:: packer:: packer( const string &filename, error_type &status ) :
        Y_RC_PACK_CTOR()
        {
            init(status);
        }
        
        resources:: packer:: packer( const char *filename, error_type &status ) :
        Y_RC_PACK_CTOR()
        {
            init(status);
        }
        
        void resources:: packer:: init( error_type &status )
        {
            file_.status = &status;
            count = 0;
            file_.unwind();
            start = file_.tell();
        }
        
        void resources:: packer:: operator()( const string &rcname, const string &filename )
        {
            
            if( db.search(rcname) )
                throw exception("resources::packer(multiple '%s')", rcname.c_str() );
            
            //------------------------------------------------------------------
            //-- initialize hasher
            ///-----------------------------------------------------------------
            H.set();
            H(rcname);
            
            //------------------------------------------------------------------
            //-- open the resource
            //------------------------------------------------------------------
            ios::raw_file  source( filename, ios::readable );
            const uint64_t source_length = source.length();
            
            //------------------------------------------------------------------
            //-- save MAGIC
            //------------------------------------------------------------------
            file_.emit<uint32_t>( MAGIC );
            
            //------------------------------------------------------------------
            //-- save the rcname
            //------------------------------------------------------------------
            file_.save_buffer( rcname );
            
            //------------------------------------------------------------------
            //-- save the resource by raw copy
            //------------------------------------------------------------------
            file_.emit<uint64_t>( source_length );
            uint64_t written_length = 0;
            {
                char   buff[ BUFSIZ ];
                size_t done = 0;
                while(true)
                {
                    source.get(buff,sizeof(buff),done);
                    if( done <= 0 )
                        break;
                    file_.put_all(buff, done);
                    H.run( buff, done );
                    written_length += done;
                }
            }
            if( source_length != written_length )
                throw exception("copy error for '%s'", rcname.c_str());
            
            //------------------------------------------------------------------
            //-- save hash-64
            //------------------------------------------------------------------
            const uint64_t hkey = H.key<uint64_t>();
            file_.emit<uint64_t>( hkey );
            
            //------------------------------------------------------------------
            //-- all done
            //------------------------------------------------------------------
            (void)db.insert(rcname);
            ++count;
            assert( db.size() == count );
        }
        
        resources:: packer:: ~packer() throw() 
        {
            try
            {
                //--------------------------------------------------------------
                // write epilog
                //--------------------------------------------------------------
                file_.emit< int64_t>(start);
                file_.emit<uint32_t>(count);
                file_.emit<uint32_t>(MAGIC);
            }
            catch(...)
            {
                //-- error in status !
            }
            
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // resources::item
        //
        ////////////////////////////////////////////////////////////////////////
        resources::item:: item( const string &id, int64_t at, uint64_t nb ) :
        label(id),
        start(at),
        bytes(nb) 
        {
        }
        
        resources::item:: ~item() throw() {}
        
        resources::item:: item( const item &other ) :
        label( other.label ),
        start( other.start ),
        bytes( other.bytes )
        {
        }
        
        const string & resources::item:: key() const throw()
        {
            return label;
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // resources
        //
        ////////////////////////////////////////////////////////////////////////
        resources:: ~resources() throw() {}
        
        resources:: resources( const string &filename ) :
        rc( new raw_file(filename, ios::readable) ), 
        H(),
        db()
        {
            extract();
        }
        
        resources:: resources( const char *filename ) :
        rc( new raw_file(filename, ios::readable) ),
        H(),
        db()
        {
            extract();
        }
        
        resources::iterator resources::begin() const { return db.begin(); }
        resources::iterator resources::end()   const { return db.end();   }
        size_t   resources::size() const throw() { return db.size(); }
        
        void resources:: extract()
        {
            int64_t       start = 0;
            uint32_t      count = 0;
            uint32_t      magic = 0;
            const int64_t epilog_size = sizeof(count) + sizeof(start) + sizeof(magic);
            
            rc->seek(-epilog_size, from_end);
            start = rc->read<int64_t>();  
            count = rc->read<uint32_t>(); 
            magic = rc->read<uint32_t>();
            if( MAGIC != magic )
                throw exception("resources: invalid MAGIC tag");
            
            //------------------------------------------------------------------
            //-- move at start
            //------------------------------------------------------------------
            rc->seek( start, from_set );
            db.reserve(count);
            
            for( unsigned int i=1; i <= count; ++i )
            {
                H.set();
                
                //--------------------------------------------------------------
                //-- read MAGIC
                //--------------------------------------------------------------
                magic = rc->read<uint32_t>();
                if( MAGIC != magic )
                    throw exception("resources: invalid item #%u", i );
                
                //--------------------------------------------------------------
                //-- read resource name
                //--------------------------------------------------------------
                const string rcname = rc->load_string();
                H( rcname );
                std::cerr << "rc: load '" << rcname << "'" << std::endl;
                
                //--------------------------------------------------------------
                // read resource length
                //--------------------------------------------------------------
                const uint64_t rclen = rc->read<uint64_t>();
                
                //--------------------------------------------------------------
                // read resource offset
                //--------------------------------------------------------------
                const int64_t  rcoff = rc->tell();
                
                //std::cerr << "rc: #" << i << "@" << int(rcoff) << "+" << int(rclen) << std::endl;
                // create resource
                const item it( rcname, rcoff, rclen );
                if( !db.insert(it) )
                    throw exception("resources: multiple '%s'", rcname.c_str());
                
                //--------------------------------------------------------------
                // skip to next while hashing
                //--------------------------------------------------------------
                for( uint64_t j=0; j < rclen; ++j )
                {
                    const uint8_t C = rc->read<uint8_t>();
                    H.run(&C,1);
                }
                
                //--------------------------------------------------------------
                // Basic integrity verification
                //--------------------------------------------------------------
                const uint64_t hkey = H.key<uint64_t>();
                if( rc->read<uint64_t>() != hkey )
                    throw exception("resources: corrupted item #%u",i);
            }
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // resources link
        //
        ////////////////////////////////////////////////////////////////////////
        resources:: rc_channel:: ~rc_channel() throw()
        {
        }
        
        resources:: rc_channel:: rc_channel( const shared_ptr<raw_file> &fp, int64_t pos, uint64_t len ) throw() :
        rc(fp),
        at(pos),
        curr(at),
        size(len),
        last(at+size)
        {
        }
        
        void resources:: rc_channel:: get( void *data, size_t size, size_t &done )
        {
            assert( !(size>0 && data==0) );
            volatile scoped_lock guard( *rc );
            rc->seek( curr, from_set );
            done = 0;
            const uint64_t to_read = min_of<uint64_t>(size,last-curr);
            try 
            {
                rc->get(data, size_t(to_read), done);
                curr += done;
            }
            catch(...)
            {
                curr += done;
                throw;
            }
        }
        
        ios::ichannel *resources:: load_channel( const string &rcname ) const
        {
            const item *it = db.search(rcname);
            if( !it ) throw exception("resources(no channel '%s')", rcname.c_str());
            return new rc_channel( rc, it->start, it->bytes );
        }
        
        ios::ichannel * resources:: load_channel( const char   *rcname ) const
        {
            const string id(rcname);
            return load_channel(id);
        }
        
        
        ////////////////////////////////////////////////////////////////////////
        //
        // resources stream
        //
        ////////////////////////////////////////////////////////////////////////
        resources:: rc_stream:: ~rc_stream() throw() {}
        
#if defined(_MSC_VER)
		// this in ctor
#pragma warning ( disable : 4355 )
#endif
        resources:: rc_stream:: rc_stream( const shared_ptr<raw_file> &fp, int64_t pos, uint64_t len ) :
        rc_channel( fp, pos, len ),
        rc_buffer( BUFSIZ ),
        iflux( static_cast<rc_channel&>(*this), static_cast<memory::rw_buffer&>(*this) )
        {
        }
        
        ios::istream * resources:: load_stream(const string &rcname ) const
        {
            const item *it = db.search(rcname);
            if( !it ) throw exception("resources(no stream '%s')", rcname.c_str());
            return new rc_stream( rc, it->start, it->bytes );
        }
        
        ios::istream  * resources:: load_stream(  const char    *rcname ) const
        {
            const string id(rcname);
            return load_stream(id);
        }
        
        void resources:: load_chars( sequence<char> &seq, const string &rcname, size_t num_pad ) const
        {
            const item *it = db.search(rcname);
            if( !it ) 
                throw exception("resources(no chars '%s')", rcname.c_str() );
            const size_t len = size_t( it->bytes );
            seq.free();
            seq.reserve(len+num_pad);
            auto_ptr<ios::istream> fp( new rc_stream(rc,it->start,len) );
            char C = 0;
            while( fp->query(C) ) seq.push_back(C);
            for( size_t i=num_pad; i>0; --i ) seq.push_back(0);
        }
        
        void resources:: load_chars( sequence<char> &seq, const char *id, size_t num_pad ) const
        {
            const string rcname(id);
            load_chars(seq, rcname, num_pad);
        }
        
        void resources:: load_string(string &str, const string &rcname ) const 
        {
            const item *it = db.search(rcname);
            if( !it ) 
                throw exception("resources(no string '%s')", rcname.c_str() );
            const size_t len = size_t( it->bytes );
            str.clear();
            string tmp(len,as_capacity);
            char C = 0;
            auto_ptr<ios::istream> fp( new rc_stream(rc,it->start,len) );
            while( fp->query(C) ) tmp.append(C);
            str.swap_with(tmp);
        }
        
        void resources:: load_string(string &str, const char *id ) const 
        {
            const string rcname(id);
            load_string(str, rcname);
        }
        
    }
}
