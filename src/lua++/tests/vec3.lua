-- testing vec3

i=vec3(1,0,0);
j=vec3(0,1,0);
k=(i^j);

print('i=',i);
print('j=',j);
print('k=',k);

a=vec3(1,2,3);
b=vec3(3,2,1);

print('a=',a);
print('b=',b);

v1 = 5*a;
s1 = a*b;

print('v1=',v1);
print('s1=',s1);


base = { ex=i, ey=j, ez=k }
print('base=',base);

for k,v in pairs(base) do print(k,v) end
