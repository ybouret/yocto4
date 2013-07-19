-- species

species =
{
    { "H+", 1 },
    { "HO-", -1}
};

eqs = 
{
    { "water",  1e-14,     { 1, "H+"}, { 1, "HO-" } },
    { "acetic", 10^(-4.8), { 1, "H+" }, { 1, "Ac-" }, { -1, "AcH" } }
};
