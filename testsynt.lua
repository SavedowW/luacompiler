return function() print("5") end

a = 1 or 2 and 3
a = 1 or 2 and -3

b = 1 < 2*2
b = 5 > 1 or 2 
b = 4 >= 2*2
b = 2+5 <= 7
b = 2+2 == 2*2
b = 2 + 1 ~= 2

c = 2 | 3 > ~2
c = 2 | 3 & 2

d = i ^ (1 << j)

a = 2*3-5
a = 2-8/2
a = 2+3%5-5
a = 2*5%2

b = -5 * 6
b = not 5 ~= 5
b = 5 + #6
c = 4*5^2

a = 1 ~ ~2
a = ~1 ~ 2

::lbl::
goto lbl