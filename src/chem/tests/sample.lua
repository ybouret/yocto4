
species =
{
    { "H+", 1, nil },
    { "HO-", -1, "Hello" },
    { "AH", 0 },
    { "A-", -1}
};

function Ka(t)
return 1e-4*(1.0+0.1*math.sin(t));
end

eqs =
{
    { "water" , 1e-14, { 1, "H+" }, {1, "HO-"} },
    { "acid",   "Ka", {1, "H+"}, {1,"A-"}, {-1,"AH"}}
};

