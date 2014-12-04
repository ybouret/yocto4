-- little test

a = 1;
b = 2;
c = 3;

S = EllipsoidSurface(a,b,c);
print("S=" .. S );

V = EllipsoidVolume(a,b,c);
print("V=" .. V );



-- let us go

ftol=1e-5;

species =
{
    { "H+",   1},
    { "HO-", -1},
    { "Na+",  1},
    { "Cl-", -1},
    { "K+",   1},
    { "AH",   0},
    { "A-",  -1},
    { "OSM",  0}
};


eqs =
{
    { "water", 10^(-14),  { 1, "H+" }, { 1, "HO-" } },
    { "acid",  10^(-4.8), { 1, "H+" }, { 1, "A-"}, {-1,"AH"}}
};

CA = 0.001;

ini =
{
    { 10^(-7.2), {1,"H+"}},
    { 150e-3,    {1,"K+"} },
    { CA,        {1,"AH"}, {1,"A-"}},
    { 20e-3,     {1,"Na+"} },
    { 20e-3,     {1,"Cl-"} },
    { "osmolarity", 300 }
};

out =
{
    { 10^(-7.4), {1,"H+"}},
    { 4e-3,      {1,"K+"} },
    { 0,         {1,"AH"}, {1,"A-"}},
    { 140e-3,    {1,"Na+"} },
    { 100e-3,    {1,"Cl-"} },
    { "osmolarity", 300 }
};



eff =
{
    "leak_K",
    "leak_Na",
    "leak_Cl"
};


function NaOH(t,Cin,Cout,params)
local a = {}
local rho = 1e-5;
a["Na+"] = rho;
a["HO-"] = rho;
return a;
end

function leak_K(t,Cin,Cout,params)
local a = {}
local zeta = params["zeta"];
a["K+"] = 4.0*Psi(zeta) * (Cout["K+"] - Cin["K+"]*math.exp(zeta));
return a;
end

function leak_Na(t,Cin,Cout,params)
local a = {}
local zeta = params["zeta"];
a["Na+"] = 1.0*Psi(zeta) * (Cout["Na+"] - Cin["Na+"]*math.exp(zeta));
return a;
end

function leak_Cl(t,Cin,Cout,params)
local a = {}
local zeta = params["zeta"];
a["Cl-"] = 1.0*Psi(-zeta) * (Cout["Cl-"] - Cin["Cl-"]*math.exp(-zeta));
return a;
end


