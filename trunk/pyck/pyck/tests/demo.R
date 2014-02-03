dyn.load("pyck_R.so");

.Call("dumpVec", rnorm(10) );
.Call("sumVec",  rnorm(20) );

seq1 <- seq(1:6)
mat1 <- matrix(seq1, 2)
mat1
.Call("dumpMat", mat1);

L <- .Call("getList");
print(L$A);
print(L$B);



d=1:10000/30;

dmin = min(d);
dmax = max(d);

u    = runif(1,dmin,dmax);
i1   = min(which(d>u)-1);
i2   = findInterval(u,d);
i3   = .Call("find_lower",u,d);

u;
i1;
i2;
i3;

Loops = 1000;
system.time(for(i in 1:Loops){ i1= min(which(d>u))-1; } )
system.time(for(i in 1:Loops){ i2= findInterval(u,d); } )
system.time(for(i in 1:Loops){ i3= .Call("find_lower",u,d); })


v1 = sort(runif(100));
v2 = sort(runif(200));

v3 = sort(c(v1,v2));
v4 = .Call("merge_sorted",v1,v2);

norm(as.matrix(v3-v4));

Loops2 = 1000;

system.time( for(i in 1:Loops2) { v3 = sort(c(v1,v2) ) }             )
system.time( for(i in 1:Loops2) { v4 = .Call("merge_sorted",v1,v2) } );
