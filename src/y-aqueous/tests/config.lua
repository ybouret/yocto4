
species = { 
    { "H+",    1, nil },
    { "HO-",  -1, nil },
    { "AcH",   0  },
    { "Ac-",  -1  },
    { "Na+",   1  }
    };

weak = { 
    { "water", 1e-14, { 1, "H+"}, { 1, "HO-" } },
    { "acetic", 10^(-4.8), { 1, "H+" }, { 1, "Ac-" }, { -1, "AcH" } }
}

init = 
{
    { 1e-2, { 1, "H+" } }
};