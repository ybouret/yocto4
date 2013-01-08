dyn.load("pyck_R.so");

.Call("dumpVec", rnorm(10) );
.Call("sumVec",  rnorm(20) );
