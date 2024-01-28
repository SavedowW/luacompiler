x = 0

function pp()
    local tempx = x
    x = x + 1
    return tempx
end

while (pp() < 5) do
    if (pp() == 1) then
        print "one"
    elseif (pp() == 2) then
        print "two"
    elseif (pp() == 3) then
        print "three"
    else
        print "different"
    end
end


print "end"

print(x)