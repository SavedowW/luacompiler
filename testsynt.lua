-- LOGIC TEST
function logtestf1()
    print "f1"
    return "hello"
end

function logtestf2()
    print "f2"
    return nil
end

print(logtestf1() or logtestf2())
print(logtestf1() and logtestf2())
print(logtestf2() or logtestf1())
print(logtestf2() and logtestf1())

logtesttbl = {}
logtestselect = logtesttbl or nil
logtestselect.name = "Named"
print(logtesttbl.name)

-- TYPE CONVERSION TEST
print(1)
print(1.0)
print(1.2e5)
print(1.2e+5)
print(1.2e-5)
print(-1)
print(1/2)
print(2/1)
print(2/0)
print(2*1)
print(2*0.5)
print(1.1 > 1)
print(1 < 1.1)
print(1.1 < 1)
print(1 > 1.1)
print(1 == 1.0)

-- STRING TEST
print("qwe".."rty".."uio")
print("qwe"..123)
print(string.lower("LoWeRuPpErCaSe"))
print(string.upper("LoWeRuPpErCaSe"))
print(string.sub("1234567890", 5))
print(string.sub("1234567890", -6, 9))

-- CLOSURES AND FREE FUNCTIONS TEST
globalvar = 5
function returner(param)
  local localvar = param
  return function()
    print(globalvar, localvar)
  end
end

returnedf1 = returner(2)
returnedf2 = returner(3)
returnedf1()
returnedf2()

globalvar = 9
returnedf1()
returnedf2()

local globalvar = 1
returnedf1()
returnedf2()

-- FIBONACCI
function fibonums(s, a, b)
    if (b < 15) then
        return fibonums(s..b..", ", b, a + b)
    else
        return s..b
    end
end
print(fibonums("0, ", 0, 1))


-- FOREACH / BREAK
tbl = {1, 2, 3, 7, 6, 5, 3}

function contains(tbl_, val_)
    local found = false
    for k, v in pairs(tbl_) do
        if (v == val_) then
            found = true
            break
        end
    end
    print (found)
end

contains(tbl, 7)
contains(tbl, 0)

for i = 1, 10, 2 do
    if i < 3 then
        print "Small number"
    elseif i < 7 then
        print "Average number"
    else
        print "Big number"
    end
end

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