#include "yocto/ocl/driver.hpp"
#include "yocto/exception.hpp"

#include "yocto/lexical/scanner.hpp"

#include "yocto/rx/matcher.hpp"
#include "yocto/rx/pattern/posix.hpp"
#include "yocto/rx/pattern/basic.hpp"

#include "yocto/ios/imstream.hpp"

namespace yocto
{
	
	namespace ocl
	{
		
		static const char __dm[] = "ocl::DeviceMapper";
		
		class Driver:: DeviceMapper : public lexical::scanner
		{
		public:
			virtual const char *name() const throw() { return __dm; }
			
			typedef lexical::scanner  self_type;
			
			static const lexical::label_t wait_for_platform  = YOCTO_FOURCC( 'P', 'L', 'A', 'T' );
			static const lexical::label_t wait_for_separator = YOCTO_FOURCC( 'S', 'E', 'P', ':' );
			static const lexical::label_t wait_for_device    = YOCTO_FOURCC( 'D', 'E', 'V', ' ' );
			
			
			const Driver       &driver;
			Core::DeviceIDsMap &devmap;
			regex::matcher      xmatch;
			size_t              p_indx;
			
			DeviceMapper( const Driver & _driver, Core::DeviceIDsMap & _devmap) :
			self_type( wait_for_platform ),
			driver( _driver ),
			devmap( _devmap ),
			xmatch(    ),
			p_indx( 0  )
			{
				assert( devmap.maxi == driver.max_devices );
				const lexical::action _discard( this, & self_type::discard_any );
				
				
				declare( wait_for_separator );
				declare( wait_for_device    );
				define( "INDEX", "[:digit:]+" );
				define( "REGEX", "#([-[:word:] @\\[\\]{}?*.()+^\\\\])*#");
				
				// [ wait_for_platform ] : skip spaces
				make( wait_for_platform, regex::posix::space(), _discard );
				{
					// [ wait_for_platform ] : accept index => wait_for_separator
					const lexical::action _on_platform_index( this, & DeviceMapper:: OnPlatformIndex );
					jump( wait_for_platform, wait_for_separator, "{INDEX}", _on_platform_index );
				}
				{
					// [ wait_for_platform ] : accept regex => wait_for_separator
					const lexical::action _on_platform_regex( this, & DeviceMapper:: OnPlatformRegex );
					jump( wait_for_platform, wait_for_separator, "{REGEX}", _on_platform_regex );
				}
				{
					// [ wait_for_platform ] : invalid char !
					const lexical::action _platform_error( this, & DeviceMapper:: OnPlatformError );
					make( wait_for_platform, regex::basic::any1::create(), _platform_error );
				}
				
				
				
				// [ wait_for_separator ] : skip spaces
				make( wait_for_separator, regex::posix::space(), _discard );
				// [ wait_for_separator ] : ':' => wait for device
				jump( wait_for_separator, wait_for_device, regex::basic::single::create(':'), _discard);
				// [ wait_for_separator ] : other => error
				{
					const lexical::action _on_separator_error( this, & DeviceMapper:: OnSeparatorError );
					make( wait_for_separator, regex::basic::any1::create(), _on_separator_error);
				}
				
				// [ wait_for_device] : skip spaces
				make( wait_for_device, regex::posix::space(), _discard );
				{
					// [ wait_for_device] : accept index => wait_for_platform
					const lexical::action _on_device_index( this, & DeviceMapper:: OnDeviceIndex );
					jump( wait_for_device, wait_for_platform, "{INDEX}", _on_device_index);
				}
				{
					// [ wait_for_device] : accept index => wait_for_platform
					const lexical::action _on_device_regex( this, & DeviceMapper:: OnDeviceRegex );
					jump( wait_for_device, wait_for_platform, "{REGEX}", _on_device_regex);
				}
				{
					// [ wait_for_device] : other => error
					const lexical::action _on_device_error( this, & DeviceMapper:: OnDeviceError );
					make( wait_for_device, regex::basic::any1::create(), _on_device_error );
				}
				
				dict().clear();
			}
			
			static inline
			size_t token2index( const regex::token &p ) throw()
			{
				size_t ans = 0;
				for( const regex::t_char *i = p.head(); i != NULL; i = i->next )
				{
					ans *= 10;
					ans += int(i->data) - int('0');
				}
				return ans;
			}
			
			void check_device_count()
			{
				if( devmap.size >= driver.max_devices )
					throw exception("%s(too many devices)", __dm);
			}
			//==================================================================
			//
			// Platform Parsing
			//
			//==================================================================
			void OnPlatformIndex( const regex::token &p )
			{
				std::cerr << "...found platform index '" << p << "'" << std::endl;
				
				check_device_count();
				p_indx = token2index( p );
				if( p_indx >= driver.num_platforms )
					throw exception("%s(invalid platform index #%u)", __dm, unsigned(p_indx) );
				
			}
			
			void OnPlatformRegex( const regex::token &p )
			{
				std::cerr << "...found platform regex '" << p << "'" << std::endl;
				check_device_count();
				const string rx   = p.to_string(1,1); //!< extract the regular expression
				xmatch            = rx;               //!< compile it
				bool has_platform = false;            //!< ready to scan
				for( size_t i = 0; i < driver.num_platforms; ++i )
				{
					const string &p_name = driver.platform[ p_indx ]->NAME;
					if( xmatch.partly( p_name ) )
					{
						if( !has_platform )
						{
							has_platform = true;
							p_indx       = i;
							std::cerr << "...selecting '" << p_name << "'" << std::endl;
						}
						else
						{
							throw exception("%s(ambiguous platform regex <%s>)", __dm, &rx[0] );
						}
						
					}
				}
				if( !has_platform )
					throw exception("%s(no platform matching <%s>", __dm, &rx[0] );
			}
			
			
			void OnPlatformError( const regex::token &p )
			{
				assert( 1 == p.size() );
				throw exception("%s(invalid char '%c', expecting platform index/regex)", __dm, p.head()->data );
			}
			
			//==================================================================
			//
			// Separator
			//
			//==================================================================
			void OnSeparatorError( const regex::token &p )
			{
				assert( 1 == p.size() );
				throw exception("%s(invalid char '%c', expecting separator ':')", __dm, p.head()->data );
			}
			
			//==================================================================
			//
			// Device Parsing
			//
			//==================================================================
			void check_device( const cl_device_id dev ) const
			{
				for( size_t i=0; i < devmap.size; ++i )
				{
					if( devmap[i] == dev )
						throw exception("%s(multiple physical device)", __dm );
				}
				devmap.append( dev );
				std::cerr << "...... (*) " << driver[ dev ]->NAME << std::endl;

			}
			void OnDeviceIndex( const regex::token & p )
			{
				assert( p_indx < driver.num_platforms );
				assert( devmap.size < devmap.maxi     );
				
				const Platform &P = driver.platform[ p_indx ];
				const char *    S = & ( P->NAME[0] );
				std::cerr << "......found device index '" << p << "'" << std::endl;
				
				const size_t d_indx = token2index( p );
				if( d_indx >= P->num_devices )
					throw exception("%s(invalid device index #%u for '%s')", __dm, unsigned(d_indx), S);
				
				check_device( P->device_id[ d_indx ] );
				
			}
			
			void OnDeviceRegex( const regex::token & p )
			{
				assert( p_indx < driver.num_platforms );
				assert( devmap.size < devmap.maxi     );
				const string rx   = p.to_string(1,1); //!< extract the regular expression
				xmatch            = rx;               //!< compile it
				const Platform &P = driver.platform[ p_indx ];
				const char *    S = & ( P->NAME[0] );
				bool has_device   = false;
				const Device *  D = NULL;
				
				for( size_t i=0; i < P->num_devices; ++i )
				{
					const Device &d = P->device[i];
					const string &d_name = d->NAME;
					if( xmatch.partly( d_name ) )
					{
						if( !has_device )
						{
							has_device = true;
							D          = &d;
							std::cerr << "......selecting " << d_name << std::endl;
						}
						else
						{
							throw exception("%s(ambiguous device regex <%s>)", __dm, &rx[0] );
						}
						
					}
				}
				if( !has_device )
					throw exception("%s(no device matching <%s> for '%s'", __dm, &rx[0], S );
				assert( D != NULL );
				check_device( (*D)->id );
			}
			
			
			void OnDeviceError( const regex::token &p )
			{
				assert( 1 == p.size() );
				throw exception("%s(invalid char '%c', expecting device index/regex)", __dm, p.head()->data );
			}
			
			
			Core::DeviceIDsMap &create_from( const string &DevMap )
			{
				//-- reset lexer
				start();
				
				//-- initialize map
				devmap.free();
				
				//-- connect devmap
				ios::imstream input( DevMap );
				
				//-- run lexer
				(*this)( input );
				
				//-- check good syntax
				if( status() != wait_for_platform )
				{
					throw exception("%s: unfinished device map", __dm);
				}
				
				//-- check not empty
				if( 0 == devmap.size )
				{
					throw exception("%s: empty device map", __dm );
				}
				
				return devmap;
				
			}
			
			
			
			virtual ~DeviceMapper() throw() {}
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(DeviceMapper);
		};
		
		void Driver:: CreateDeviceMapper() const
		{
			lock access;
			if( !mapper_ ) mapper_ = new DeviceMapper( *this, device_map );
		}
		
		void Driver:: DeleteDeviceMapper() const throw()
		{
			lock access;
			if( mapper_ )
			{
				delete mapper_;
				mapper_ = NULL;
			}
		}
		
		const Core::DeviceIDsMap   & Driver:: MapDeviceIDs( const string &devmap ) const
		{
			lock access;
			assert( device_map.maxi == max_devices );
			CreateDeviceMapper();
			return mapper_->create_from( devmap );
		}
		
		
	}
	
}
