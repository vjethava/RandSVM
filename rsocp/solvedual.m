function [soln, r, res] = solvedual(Y, X, W, H, K, P)
prob = genmosekd(Y, X, W, H, K, P); 
d = size(X, 2);
n = size(X, 1); 
assert(length(Y) == n); 
[r, res] = mosekopt('maximize', prob); 
soln = [];
soln.gamma = res.sol.itr.xx(n+1);
if(abs(res.sol.itr.slc(1)) > abs(res.sol.itr.suc(1)))
    soln.b = res.sol.itr.slc(1)
else
    soln.b = res.sol.itr.suc(1); 
end
soln.mu = res.sol.itr.xx(1:n);
soln.K = K; 
soln.P = P; 
soln.Y = Y
soln.X = X