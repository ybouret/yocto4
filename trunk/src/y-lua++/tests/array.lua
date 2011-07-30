-- testing array

a=array(16000);

cos=math.cos;

for i=1,a:size() do
	a[i] = cos( (math.pi * i / a:size()) );
	print('a[',i,']=',a[i] );
end
