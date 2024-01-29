function returner(param)
    return function()
        print(param)
    end
end

res1 = returner(5)
res2 = returner(99)

res1()
res2()