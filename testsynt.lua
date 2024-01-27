x = 1

function f1(param)
    local x = param
    return function ()
        x = x + 1
        print(x)
    end
end

res1 = f1(5)
res1()
res1()

res2 = f1(100)
res2()
res2()

print "\n\n"
res1()
res2()