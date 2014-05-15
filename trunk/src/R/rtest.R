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
