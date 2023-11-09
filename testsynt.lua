if a == 5 then
    print("Average answer")
    print(a)
elseif a < 4 then
    print("Smaller answer")
    print(a)
elseif a > 6 then
    print("Greater answer")
    print(a)
else
    print("Unknown answer")
    print(a)
end

Account = {balance = 0}
    
    function Account:new (o)
      o = o or {}
      setmetatable(o, self)
      self.__index = self
      return o
    end
    
    function Account:deposit (v)
      self.balance = self.balance + v
    end
    
    function Account:withdraw (v)
      if v > self.balance then error"insufficient funds" end
      self.balance = self.balance - v
    end

    SpecialAccount = Account:new()
    obj = SpecialAccount:new{limit=1000.00}
    obj:deposit(100.00)