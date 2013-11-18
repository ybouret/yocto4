#ifndef YOCTO_QUEUE_INCLUDED
#define YOCTO_QUEUE_INCLUDED 1


#include "yocto/stock/pipe.hpp"
#include "yocto/sequence/list.hpp"

namespace yocto {
	
	namespace hidden
	{
		extern const char queue_name[];
	}
	
	template<
	typename T,
	typename SEQUENCE = list<T>
	>
	class queue : public pipe<T,SEQUENCE,fifo_type>
	{
public:
	typedef pipe<T,SEQUENCE,fifo_type> self;
	
	explicit queue() throw() {}
	virtual ~queue() throw() {}
	explicit queue( size_t n, const as_capacity_t &x) : self(n,x)  {}
	
	virtual const char *name() const throw() { return hidden::queue_name; }
	
private:
	YOCTO_DISABLE_COPY_AND_ASSIGN(queue);
	
};


}


#endif
