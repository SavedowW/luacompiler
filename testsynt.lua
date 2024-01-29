human = {age = 22}
function human:new(age_)
    mtbl = {__index = self}
    newh = {age = age_}
    setmetatable(newh, mtbl)
    return newh
end

function human:printage()
    print(self.age)
end

child = human:new(5)

human:printage()
child:printage()