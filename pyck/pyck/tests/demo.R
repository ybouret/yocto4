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

print(u);
print(i1);
print(i2);
print(i3);

Loops = 10000;
system.time(for(i in 1:Loops){ i1= min(which(d>u))-1; } )
system.time(for(i in 1:Loops){ i2= findInterval(u,d); } )
system.time(for(i in 1:Loops){ i3= .Call("find_lower",u,d); })