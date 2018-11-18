#!/usr/bin/env lua

N = 8

function dump_table(t)
    for i = 1, #t do
        io.write(t[i], " ")
    end
    io.write("\n")
end

function is_place_ok(a, n, c)
    for i = 1, n - 1 do
        if (a[i] == c) or (n - i == c - a[i]) or (i - n == c - a[i]) then
            return false
        end
    end
    return true
end

function print_solution(a)
    for i = 1, N do
        for j = 1, N do
            io.write(a[i] == j and "X" or "-", " ")
        end
        io.write("\n")
    end
    io.write("\n")
end

number = 1

function add_queue(a, n)
    if n > N then
        print(number)
        print("------------------------------------------\n")
        print_solution(a)
        number = number + 1
    else
        for c = 1, N do
            if is_place_ok(a, n, c) then
                a[n] = c
                --dump_table(a)
                add_queue(a, n + 1)
            end
        end
    end
end

add_queue({}, 1)
