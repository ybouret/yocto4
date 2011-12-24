#ifndef SAMPLE_INCLUDED
#define SAMPLE_INCLUDED 1

#include "yocto/os.hpp"

class Foo
{
public:
	int a;
	explicit Foo() throw();
	virtual ~Foo() throw();
	
private:
	YOCTO_DISABLE_COPY_AND_ASSIGN(Foo);
};

namespace Toto
{
	class Bar
	{
	public:
		int b;
		explicit Bar() throw();
		virtual ~Bar() throw();
		
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(Bar);
	};
}

#endif
