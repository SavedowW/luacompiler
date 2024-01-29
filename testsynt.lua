-- String functions
print(string.lower("LoWeRuPpErCaSe"))
print(string.upper("LoWeRuPpErCaSe"))
print(string.sub("1234567890", 5))
print(string.sub("1234567890", -6, 9))

print(tostring(tbl1))
print(tostring(tbl2))


-- object creation
human = {age = 22}
function human:new(age_, name_)
    local mtbl = {__index = self, __name = name_}
    local newh = {age = age_}
    setmetatable(newh, mtbl)
    return newh
end

function human:printself()
    print(tostring(self).." ("..self.age..")")
end

child = human:new(5, "Bob")

human:printself()
child:printself()

-- Fibonacci
function fibonums(s, a, b)
    if (b < 15) then
        return fibonums(s..b..", ", b, a + b)
    else
        return s..b
    end
end

print(fibonums("0, ", 0, 1))



-- Pass as values / tables
function passAsValues(a, b, c)
    a = 5
    b = 1
    c = 7
    end

function passAsTable(tbl)
    tbl[1] = 5
    tbl[2] = 1
    tbl[3] = 7
    end

args = {0, 0, 0}
print(args[1], args[2], args[3])
passAsValues(args[1], args[2], args[3])
print(args[1], args[2], args[3])
passAsTable(args)
print(args[1], args[2], args[3])


-- Vararg test
function fVarargTest(lstname, ...)
    print(lstname..":")
    print(...)
end
fVarargTest("List 1", 1, 2, 3)


-- Metaoperator
function op1(tbl_, val_)
    return tbl_.id + val_
end

mtbl = {__add = op1}

tbl1 = {id = 10}
setmetatable(tbl1, mtbl)
val = 5

print(tbl1 + val)


-- Tables
N = 5
M = 5

mt = {}          -- create the matrix
for i=1,N do
  mt[i] = {}     -- create a new row
  for j=1,M do
    if (i == j) then
        mt[i][j] = "#"
    else
        mt[i][j] = "@"
    end
  end
end

for i=1,N do
    print(mt[i][1], mt[i][2], mt[i][3], mt[i][4], mt[i][5])
end


-- for loop
function begin1()
    print "begin"
    return 1
end

function end1()
    print "end"
    return 10
end

function step()
    print "step"
    return 1
end

for i = begin1(), end1(), step() do
    print(i)
end


-- Operators
print(1 + 2)
print(1 - 2)
print(5 * 2.5)
print(5 / 2)
print(5 % 2)
print(5 // 2)
print(5 ^ 2)
print(#"qwe")
print(#{1, 2, 3})
print(13 & 5)
print(13 | 5)
print(1 << 2)
print(16 >> 2)
print(true == true)
print(true ~= true)
print(5 <= 6)
print(5 >= 6)
print(5 < 6)
print(5 > 6)
print(true or false)
print(true and false)
print(not nil)
print(~13)
print(-13)


-- IO and Sort
N = 10

function print_array(a)
    local i = 1
    while a[i] do
      print(a[i])
      i = i + 1
    end
end

function enter_array(a)
    for i = 1, N do
        local val = nil
        while (not val) do
            print("["..i.."]: ")
            val = tonumber(io.read())
        end
        a[i] = val
    end
end

function bubblesort(t)
    local swapsMade = false
    repeat
      swapsMade = false
      for i=1,#t-1 do
        -- print(t[i] .. "  " .. t[i+1])
        local a, b = t[i], t[i+1]
        if a > b then
          t[i], t[i+1] = b, a
          swapsMade = true
        end
      end
    -- break if no swaps were made
    until swapsMade == false
    return t
end

local a = {}
enter_array(a)
bubblesort(a)
print "\nSorted:"
print_array(a)