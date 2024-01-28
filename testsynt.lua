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