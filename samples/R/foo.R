
dyn.load("foo.so");

hello2 <- function(n) {
  .C("hello", as.integer(n))
}

hello2(3);

