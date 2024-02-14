-- IO and Sort
-- Зависло после ввода всех
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

function bubblesort(t, CMP_)
    local swapsMade = false
    repeat
      swapsMade = false
      for i=1,#t-1 do
        -- print(t[i] .. "  " .. t[i+1])
        local a, b = t[i], t[i+1]
        if CMP_(a, b) then
          t[i], t[i+1] = b, a
          swapsMade = true
        end
      end
    -- break if no swaps were made
    until swapsMade == false
    return t
end

function comparator(a, b)
    return a < b
end

local a = {}
enter_array(a)
bubblesort(a, comparator)
print "\nSorted:"
print_array(a)