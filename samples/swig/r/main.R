dyn.load(paste("example", .Platform$dynlib.ext, sep=""))
source("example.R")
cacheMetaData(1)

My_variable_get()
fact(5)
get_time()
v <- c(1,2,3,4,5,6,7,8)
my_sum( as.double(v), length(v) )

m <- matrix( data=NA, nrow=3,ncol=4)
my_mat( as.double(m), nrow(m), ncol(m) );
m

