
exp = math.exp;

-- spontaneous
function lambda0(t)
return 1+0.1*t;
end

-- addendum: tau=t-t_cav
function lambda1(tau,d)
return 100*exp(-2*tau);
end
