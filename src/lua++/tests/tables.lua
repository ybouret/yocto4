
print(toto);

--- toto["H+"] = 1e-4;


-- print all values of array `a'
print("ipairs");
for i,v in ipairs(toto) do print( "toto[" .. i .. "] = " .. v) end

-- print all keys of table `t'
print("pairs");
for k in pairs(toto) do print( "toto[" .. k .. "]=" .. toto[k] ) end