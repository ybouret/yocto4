################################################################################
##
## Loading the binary code
##
################################################################################

## build the DLL name
lyre_dll <- paste("rtest", .Platform$dynlib.ext, sep="")
print(paste("[LYRE] Loading ",lyre_dll) )

lyre_version <- "lyre_version";

## load the dll only once
if( is.loaded(lyre_version) ) dyn.unload(lyre_dll);
dyn.load(lyre_dll)
if( !is.loaded(lyre_version) ) stop( paste("Unable to find ",lyre_version) );

################################################################################
##
## display the version
##
################################################################################
lyre_version <- function()
{
    .Call("lyre_version");
}

lyre_version();

lyre_sum <- function( A )
{
    .Call("lyre_sum",A);
}

lyre_sum(seq(1,10));


lyre_mat <- function(M)
{
    .Call("lyre_mat",M);
}
# 4 colums, 2 rows
mat=replicate(4,rnorm(2));

lyre_mat(mat);
