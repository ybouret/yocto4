#ifndef YOCTO_LOCAL_FS_INCLUDED
#define YOCTO_LOCAL_FS_INCLUDED 1

#include "yocto/fs/vfs.hpp"
#include "yocto/threading/singleton.hpp"

namespace yocto
{
		
		class local_fs : public singleton<local_fs>, public vfs
		{
		public:
			virtual entry::attribute query_attribute( const string &path, bool &is_link ) const throw();
			virtual void     create_dir(  const string &dirname, const bool allow_already_exists );
			virtual void     remove_dir(  const string &dirname   );
			virtual void     remove_file( const string &filename );
			virtual scanner *new_scanner( const string &dirname ) const;
			virtual uint64_t get_file_size( const string &path ) const;
            
		private:
			explicit local_fs() throw();
			virtual ~local_fs() throw();
			friend class singleton<local_fs>;
			YOCTO_DISABLE_COPY_AND_ASSIGN(local_fs);
			static const threading::longevity life_time = object::provider::life_time - 2; // TODO: check this value
			static const char name[];
		};
		
    
}
#endif
