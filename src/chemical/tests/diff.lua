species =
{
    { "H+",   1},
    { "HO-", -1},
    { "AcH",  0},
    { "Ac-", -1},
    { "Na+",  1},
};

eqs =
{
    { "water", 10^(-14), { 1, "H+"}, {1,"HO-"} },
    { "acetic", 10^(-4.8), {1, "H+"}, {1, "Ac-"}, {-1, "AcH"} }
}

C0 = 1e-3;
V0 = 10;

ini =
{
    { C0, { 1, "AcH" }, {1, "Ac-" } },
    { 0 , { 1, "Na+" } }
}


effectors =
{
    {
        "NaOH",           -- function name
        { },              -- input components
        { "Na+", "HO-"}   -- output components
    }
};

-- one function for effector
function NaOH(t,zeta)

-- print( "NaOH@t=" .. t .. ", zeta=" .. zeta );
return 1,1;

end
