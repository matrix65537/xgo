#!/usr/bin/env lua

function fac(n)
    if n == 0 then
        return 1
    else
        return n * fac(n - 1)
    end
end
    
print(fac(1))
print(fac(2))
print(fac(3))
print(fac(4))
print(fac(5))
