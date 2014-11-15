-- declaring species
species = {
    { "H+",   1,   1.0  },
    { "HO-", -1, "info" },
    { "AH",  0 },
    { "A-", -1 }
};


eqs =
{
    { "water", 1e-14, { 1, "H+"}, {1, "HO-"}  },
    { "acid",  10^(-4.7), { 1, "A-" }, {1, "H+"}, { -1, "AH" }}
};