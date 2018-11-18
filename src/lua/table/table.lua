#!/usr/bin/env lua

function dump_table(t)
    for i = 1, 3 do
        io.write(t[i], " ")
    end
    io.write("\n")
end

function f(t)
    t[3] = 4
    dump_table(t)
end


t = {1, 2, 3}
dump_table(t)
f(t)
dump_table(t)
