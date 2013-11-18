
template <>
real_t strconv:: to_real<real_t>( const string &txt, const char *ctx )
{
	static const char fmt[] = "strconv::to_real(invalid char '%c') for <%s>";
	if( !ctx )        ctx   = "";
	static const char fmt2[] = "strconv::to_real(end of input after %s) for <%s>";
    if( txt.size() <= 0 ) throw libc::exception( EINVAL, "strconv::to_real(empty string)");
	
	const size_t n = txt.size();
	size_t       i = 0;
	bool         neg = false;
	
	//-- optional sign
	switch( txt[i] )
	{
		case '-':
			neg = true;
		case '+':
			++i;
			if( i >= n ) throw libc::exception( EINVAL, fmt2, "sign", ctx );
			break;
			
		default:
			break;
	}
	
	//-- integer part
	assert( i < n );
	
	real_t ip = 0;
	real_t fp  = 0;
	real_t fm  = 1;
	do
	{
		const char C = txt[i];
		switch( C )
		{
			case '0': ip *= real_t(10);
				break;
				
			case '1': ip *= real_t(10); ip += real_t(1); break;
			case '2': ip *= real_t(10); ip += real_t(2); break;
			case '3': ip *= real_t(10); ip += real_t(3); break;
			case '4': ip *= real_t(10); ip += real_t(4); break;
			case '5': ip *= real_t(10); ip += real_t(5); break;
			case '6': ip *= real_t(10); ip += real_t(6); break;
			case '7': ip *= real_t(10); ip += real_t(7); break;
			case '8': ip *= real_t(10); ip += real_t(8); break;
			case '9': ip *= real_t(10); ip += real_t(9); break;
				
			case '.':
				goto PARSE_FP;
				
			case 'E':
			case 'e':
			case 'D':
			case 'd':
				goto PARSE_EP;
				
			default:
				throw libc::exception( EINVAL, fmt, C, ctx );
		}
		
	}
	while( ++i < n );
	return neg ? -ip : ip;
	
PARSE_FP:
	assert( i < n );
	assert( txt[i] == '.' );
	{
		
		
		
		for( ++i; i < n; ++i )
		{
			const char C = txt[i];
			switch( C )
			{
				case '0': fm *= real_t(0.1); break;
				case '1': fm *= real_t(0.1); fp += fm; break;
				case '2': fm *= real_t(0.1); fp += 2*fm; break;
				case '3': fm *= real_t(0.1); fp += 3*fm; break;
				case '4': fm *= real_t(0.1); fp += 4*fm; break;
				case '5': fm *= real_t(0.1); fp += 5*fm; break;
				case '6': fm *= real_t(0.1); fp += 6*fm; break;
				case '7': fm *= real_t(0.1); fp += 7*fm; break;
				case '8': fm *= real_t(0.1); fp += 8*fm; break;
				case '9': fm *= real_t(0.1); fp += 9*fm; break;
					
				case 'E':
				case 'e':
				case 'D':
				case 'd':
					goto PARSE_EP;
					
				default:
					throw libc::exception( EINVAL, fmt, C, ctx );
			}
		}
		
		return neg ? -(ip+fp) : (ip+fp) ;
		
	PARSE_EP:
		assert(i<n);
		assert( 'e' == txt[i] || 'E' == txt[i] || 'd' == txt[i] || 'D' == txt[i] );
		{
			bool ep_neg = false;
			if( ++i >= n )
				throw libc::exception( EINVAL, fmt2, "exponent", ctx);
			
			switch( txt[i] )
			{
				case '-':
					ep_neg = true;
				case '+':
					++i;
					if( i >= n ) throw libc::exception( EINVAL, fmt2, "exponent sign", ctx );
					break;
					
				default:
					break;
			}
			
			assert( i < n );
			size_t ep = 0;
			for( ; i < n; ++i )
			{
				ep *= 10;
				const char C = txt[i];
				switch( C )
				{
					case '0':          break;
					case '1': ++ep;    break;
					case '2': ep += 2; break;
					case '3': ep += 3; break;
					case '4': ep += 4; break;
					case '5': ep += 5; break;
					case '6': ep += 6; break;
					case '7': ep += 7; break;
					case '8': ep += 8; break;
					case '9': ep += 9; break;
					
					default:
						throw libc::exception( EINVAL, fmt, C, ctx );
				}
			}
			const real_t tmp = neg ? -(ip+fp) : (ip+fp) ;
			
			return tmp * (ep_neg ? ipower<real_t>(real_t(0.1),ep) : ipower<real_t>(real_t(10),ep) );
		}
	}
	
	
	
}

