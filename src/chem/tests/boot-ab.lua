
species =
{
    { "A", 0 },
    { "B", 0 },
    { "C", 0 }
}

function K(t)
return 0.7 + 0.5*math.sin(t);
end

eqs =
{
    { "AB", "K", { 1, "A" }, { 1, "B" } }
}


-- 1.4 is the limit for B+C @t=0
ini =
{
    { 0.5,   { 1, "A" } },
    { 1.38,  { 1, "B"}, {1,"C"} }
}