################################################################################
##
## Loading the binary code
##
################################################################################

## build the DLL name
the_dll <- paste("rcode", .Platform$dynlib.ext, sep="")
print(paste("rcode: loading ",the_dll) )

the_version <- "the_version";

## load the dll only once
if( is.loaded(the_version) ) dyn.unload(the_dll);
dyn.load(the_dll);
if( !is.loaded(the_version) ) stop( paste("Unable to load ",the_dll) );

## call version
Version <- function()
{
    .Call(the_version);
}

Version();

## vector load/send
VectorSum <- function(v)
{
    .Call("VectorSum",v);
}

