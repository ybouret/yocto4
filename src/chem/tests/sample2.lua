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


function Kam(t)
return 10^(-9.2)
end

eqs =
{
    { "water",  1e-14,     { 1, "H+"}, { 1, "HO-" } },
    { "acetic", 10^(-4.8), { 1, "H+"}, { 1, "Ac-" }, { -1, "AcH" } },
    { "ammoniac", "Kam",   { 1, "H+"},  { 1, "NH3" }, { -1, "NH4+"} }
};


ini =
{
    { 0.0001, { 1, "AcH" }, {1, "Ac-" } },
    { 0, {1,"Na+"} },
    { 0, {1,"Cl-"} },
    { 0.0002, {1,"NH4+"}, { 1, "NH3" } }
};


-- effector
effectors = {
    "MCT"
};

-- one function for effector
function MCT(t,zeta,S,S_out)
print( "MCT@t=" .. t .. ", zeta=" .. zeta );
    local h=S["H+"];
ans = {};
ans["H+"] = 1;
ans["HO-"] = 2;
return ans;
end
