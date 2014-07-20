
species =
{
    { "H+", 1, nil },
    { "HO-", -1, "Hello" },
    { "AH", 0 },
    { "A-", -1},
    { "Na+", 1}
};

function Ka(t)
return 1e-4*(1.0+0.1*math.sin(t));
end

eqs =
{
    { "water" , 1e-14, { 1, "H+" }, {1, "HO-"} },
    { "acid",   "Ka", {1, "H+"}, {1,"A-"}, {-1,"AH"}}
};


ini =
{
    { 0,    {1, "Na+" } },
    -- { 0.1 , {1, "AH"  }, {1, "A-" } }
    { 1e-6, {1,"H+"}}
};
