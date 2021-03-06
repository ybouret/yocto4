-- declaring species
species = {
    { "H+",   1,   1.0  },
    { "HO-", -1, "info" },
    { "AH2",  0 },
    { "A--", -2 },
    { "Na+", 1  },
    { "Cl-", -1 },
    { "NH3",  0 },
    { "NH4+", 1 },
    { "OSM",  0 }
};


function KNH3(t)
return 10^(-9.2)*(2+math.sin(t));
end

eqs =
{
    { "water",    1e-14,     { 1, "H+"}, {1, "HO-"}  },
    { "acid",     10^(-4.7), { 1, "A--" }, {2, "H+"}, { -1, "AH2" }},
    { "ammoniac", "KNH3",    { 1, "NH3" }, {1, "H+"}, { -1, "NH4+"} }
};

ini =
{
    { "E/N" },
    { "osmolarity", 0 },
    { 0.0000, {1,"Na+"} },
    { 0.0000, {1,"Cl-"} },
    { 0.0000, {1,"AH2"}, {1,"A--"} },
    { 0.00001, {1,"NH3"}, {1,"NH4+"} }
};

ini2 =
{
    { "E/N" },
    { "osmolarity", 0 },
    { 1e-4,   {1, "H+" }},
    { 0.0000, {1, "Na+"} },
    { 0.0,    {1, "AH2"}, {1,"A--"} },
    { 0.0,    {1, "NH3"}, {1,"NH4+"} }
}


function Soude(t,Cin,Cout,params)
local zeta = params["zeta"];
print( "Soude@t=" .. t .. ", zeta=" .. zeta);
local a = {};
local rho = 0.001;
a["Na+"] = rho;
a["HO-"] = rho;
return a;
end

function DoNothing()
a = {};
return a;
end

eff =
{
    "Soude",
    "DoNothing"
};