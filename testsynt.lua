function fibonums(s, a, b)
    if (b < 15) then
        return fibonums(s..b..", ", b, a + b)
    else
        return s..b
    end
end

print(fibonums("0, ", 0, 1))