dyn.load("pyck_R.so");

.Call("dumpVec", rnorm(10) );
.Call("sumVec",  rnorm(20) );

seq1 <- seq(1:6)
mat1 <- matrix(seq1, 2)
mat1
.Call("dumpMat", mat1);

