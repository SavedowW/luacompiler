print "coo\0lstr";
print "part1\"part2";
print 'part1\npart2';
print(print(a))
a,b,c = 1,2,3
a,b = b,a
print( 
    "part1" .. "part2"
 )
a = nil
a = true
a = false
return a+b

a = true and true
a = true or false
a = true and not false

local varname = 5

if (true) then
    print("here1")
elseif (5) then
    print("here2")
else
    print("here3")
end

if (true) then
    print("here1")
end

while (true) do
    a = a + 1
    print(a)
end

repeat a = a + 1 until a or b

for varname=1,10,1 do
    print("hi")
end

for varname=1,10 do
    print("hi")
end

f = function (a)
    return a*a
end

for i,k in gettable() do
    print(i,k)
    show"output"
    show{foo = "bar", [123] = 456}
end

a[123] = 5
a = {}
t = {["foo"] = "bar", [123] = 456}

a.b()
a:b()
a = a ^ b
a = a % b