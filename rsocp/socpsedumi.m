function [soln] = solvesedumi(y, x, w)
n = length(y); 
d = size(x, 2); 
[At, b, c, K] = genSOCP(y, x, w); 
[primal, dual, info] = sedumi(At, b, c, K);
soln.primal = primal;
soln.b = primal(1);
soln.xi = primal(2:n+1); 
soln.mod_w = primal(2*n+2);
soln.w = primal(2*n+2+1:2*n+2+d); 
soln.alpha = dual(1:n);
soln.beta = dual(n+1); 
soln.info = info;