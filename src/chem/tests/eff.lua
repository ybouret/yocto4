-- species, level 2

species =
{
    { "H+",   1},
    { "HO-", -1},
    { "AcH",  0},
    { "Ac-", -1},
    { "NH4+", 1},
    { "NH3",  0},
    { "Na+",  1},
    { "Cl-", -1}
};


-- one function for effector
function eff1(t,zeta,S,S_out)
print( "eff1@t=" .. t .. ", zeta=" .. zeta );
local h=S["H+"];
ans = {};
ans["H+"]  = h;
ans["HO-"] = 2;
return ans;
end
