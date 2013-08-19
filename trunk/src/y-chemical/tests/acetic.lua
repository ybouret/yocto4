-- species

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

C0 = 1e-6;

ini =
{
{ C0, { 1, "AcH" }, {1, "Ac-" } },
{ 0 , { 1, "Na+" } }
}