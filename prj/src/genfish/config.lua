
local sqrt = math.sqrt

function Width(z)
return sqrt(z*(1-z)^2);
end

Up = Width;
Down = Up;

function Mix(u)
return u;
end


N = 10;