function [soln, r, res] = solvemosek(Y, X, W)
n = length(Y); 
d = size(X, 2); 
assert (length(Y) == size(X, 1)); 
prob = genmosek(Y, X, W);
[r, res] = mosekopt('minimize', prob); 
soln.w = res.sol.itr.xx((n+1):(n+d));
soln.b = res.sol.itr.xx(n+d+1);
soln.mu = res.sol.itr.slc;

