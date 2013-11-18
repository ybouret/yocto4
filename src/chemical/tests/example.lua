-- species

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
    { 0.01, { 1, "AcH" }, {1, "Ac-" } },
    { 0, {1,"Na+"} },
    { 0, {1,"Cl-"} },
    { 0, {1,"NH4+"}, { 1, "NH3" } }
};


-- effector
effectors = { 
    { "MCT",                  -- function name
      { "H+", "Ac-" },        -- input components
      { "H+", "AcH", "Ac-"}   -- output components
    }
};

function MCT(t,zeta,h,am)
    print( "MCT@t=" .. t .. ", zeta=" .. zeta );
    print( "h=" .. h .. ", am=" .. am );
    return 1,2,3;
end
