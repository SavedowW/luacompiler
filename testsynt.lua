function f1()
    print "f1"
    return true
end

function f2()
    print "f2"
    return false
end

print(f1() or f2())
print(f1() and f2())
print(f2() or f1())
print(f2() and f1())