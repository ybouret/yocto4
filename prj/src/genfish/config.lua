
local sqrt = math.sqrt

t=0.1;

function Width(z)
-- return sqrt(z*(1-z)^2);
return t/0.2*( 0.2969*sqrt(z)-0.1260*z - 0.3516*z*z + 0.2843*z*z*z -0.1015*z*z*z*z)
end

function Up(z)
return 3*Width(z);
end

function Down(z)
return 2*Width(z);
end

-- Mix
function Mix(u)
return u^2;
end


--N = 200;
N = 100;