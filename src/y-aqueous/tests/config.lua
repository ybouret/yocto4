
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
    { 0,      { 1,  "Na+" } },
    { 0.005,  { 1 , "AcH" }, {1, "Ac-" } }
};

-- effector
effectors = { 
    { "MCT", { "H+", "Ac-" }, {} }
    };

function MCT(t,h,am)
    print( "MCT@t=" .. t);
    print( "h=" .. h .. ", am=" .. am );
end