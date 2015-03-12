#include "yocto/sequence/list.hpp"
#include "yocto/sequence/vector.hpp"

#include "yocto/memory/pooled.hpp"

#include "yocto/utest/run.hpp"
#include "yocto/type/spec.hpp"

#include "support.hpp"

#include <cstdlib>

using namespace yocto;

namespace  {
	
	static int num_dummy = 0;
	
	class dummy 
	{
	public:
		int a;
		dummy(int x ) throw() : a(x)
		{
			++num_dummy;
		}
		
		dummy( const dummy &d ) : a(d.a)
		{
			++num_dummy;
		}
		
		
		~dummy() throw()
		{
			--num_dummy;
		}
		
		friend inline std::ostream & operator<<( std::ostream &os, const dummy &d ) 
		{
			os << "d#" << d.a;
			return os;
		}
		
	private:
		YOCTO_DISABLE_ASSIGN(dummy);
	};
}


template <>
struct gen<dummy> { static inline dummy get() { return dummy(alea_of<int>()); } };

namespace {	
	
	template <typename SEQUENCE>
	static inline 
	void perform( size_t n )
	{
		typedef typename SEQUENCE::mutable_type T;
		SEQUENCE seq;
		
		type_spec spec = typeid(SEQUENCE);
		type_spec ts   = typeid(T);
		std::cerr << "---------------------------------------------------------" << std::endl;
		std::cerr << "-- " << spec.name() << std::endl;
		std::cerr << "---------------------------------------------------------" << std::endl;
		std::cerr << "<< " << ts.name() << " >>:" << std::endl;
		std::cerr << ( support_bitwise_ops<T>::value  ? '+' : '-' ) << "bitwise_ops" << std::endl;
		std::cerr << ( support_no_destruct<T>::value  ? '+' : '-' ) << "no_destruct" << std::endl;
		std::cerr << "fill..." << std::endl;
		
		for( size_t i=0; i<n; ++i )
		{
			const T obj = gen<T>::get();
			if( alea<float>() < 0.5f )
			{
				seq.push_front( obj );
			}
			else
			{
				seq.push_back( obj );
			}
		}
		std::cerr << ".. " << seq.size() << " / " << seq.capacity() << std::endl;
		for( size_t i=seq.size()/2; i >0; --i )
		{
			if( alea<float>() < 0.5f )
			{
				seq.pop_back();
			}
			else
			{
				seq.pop_front();
			}
		}
		std::cerr << ".. " << seq.size() << " / " << seq.capacity() << std::endl;
		for( size_t i=0; i<n; ++i )
		{
			const T obj = gen<T>::get();
			if( alea<float>() < 0.5f )
			{
				seq.push_front( obj );
			}
			else
			{
				seq.push_back( obj );
			}
		}
		std::cerr << ".. " << seq.size() << " / " << seq.capacity() << std::endl;
		std::cerr << "...copy" << std::endl;
		SEQUENCE seq2( seq );
		std::cerr << ".. " << seq2.size() << " / " << seq2.capacity() << std::endl;
		
		std::cerr << "...free" << std::endl;
		seq.free();
		std::cerr << ".. " << seq.size() << " / " << seq.capacity() << std::endl;
		std::cerr << "...done" << std::endl;
		seq2.free();
		
		for( size_t i=0; i < 16; ++i )
		{
			const T obj = gen<T>::get();
			if( alea<float>() < 0.5f )
			{
				seq.push_front( obj );
			}
			else
			{
				seq.push_back( obj );
			}
		}
		
		for( typename SEQUENCE::iterator i = seq.begin(); i != seq.end(); ++i )
		{
			std::cerr << " '" << *i << "'";
		}
		std::cerr << std::endl;
		
		for( typename SEQUENCE::reverse_iterator i = seq.rbegin(); i != seq.rend(); ++i )
		{
			std::cerr << " '" << *i << "'";
		}
		std::cerr << std::endl;

        for( size_t i=1; i <= seq.size(); ++i)
        {
            const T &obj = seq.at(i);
            std::cerr << "#" << i << ": " << obj << std::endl;
        }
		
	}
}

YOCTO_UNIT_TEST_IMPL(sequence)
{
	size_t n = 0;
	if( argc > 1 )
	{
		n = strtol( argv[1], NULL, 10);
	}
	
	if( n <= 0 )
		n = 1000;
	perform< list<int>    >(n);
	perform< list<double> >(n);
	perform< list<string> >(n);
	perform< list<dummy>  >(n); if( num_dummy != 0 ) throw exception("list: invalid dummy count");
	
	perform< vector<int>    >(n);
	perform< vector<double> >(n);
	perform< vector<string> >(n);
	perform< vector<dummy>  >(n); if( num_dummy != 0 ) throw exception("vector: invalid dummy count");
	perform< vector<int,memory::pooled::allocator>  >(n);
}
YOCTO_UNIT_TEST_DONE()
