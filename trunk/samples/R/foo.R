
dyn.load("foo.so");

hello2 <- function(n) {
  .C("hello", as.integer(n))
}

hello2(3);
m <- matrix( data=1, nrow=3, ncol=4)

.C("showMatrix", as.double(m), as.integer(nrow(m)), as.integer(ncol(m)) );

.Call("vecSum",rnorm(10));

