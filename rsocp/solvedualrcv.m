function [soln, r, res] = solvedualrcv(Y, X, W)
param = [];

% Primal feasibility tolerance for the primal solution
param.MSK_DPAR_INTPNT_CO_TOL_PFEAS = 1.0e-5;

% Dual feasibility tolerance for the dual solution
param.MSK_DPAR_INTPNT_CO_TOL_DFEAS = 1.0e-5;

% Relative primal-dual gap tolerance.
param.MSK_DPAR_INTPNT_CO_TOL_REL_GAP = 1.0e-5;

param.MSK_IPAR_INTPNT_NUM_THREADS = 2; 
prob = genmosekdrcv(Y, X, W); 
disp(sprintf(' generated problem ')); 
d = size(X, 2);
n = size(X, 1); 
assert(length(Y) == n); 
[r, res] = mosekopt('maximize', prob, param); 
save mosekintermediate; 
soln = [];
soln.gamma = res.sol.itr.xx(n+1);
if(abs(res.sol.itr.slc(1)) > abs(res.sol.itr.suc(1)))
    soln.b = res.sol.itr.slc(1)
else
    soln.b = res.sol.itr.suc(1); 
end
soln.mu = res.sol.itr.xx(1:n);
soln.w = sparse(soln.gamma * sum(diag(soln.mu .* Y) * X, 1));