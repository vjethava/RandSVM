function [prob] = genmosekd(Y, X, W, H, K, P)
% formulates the dual variables: [ mu_1 .. mu_1 zeta t_1 ... t_n]
% Y - class labels
% X - data 
% W - max ||w||
% K - the kernel function
% kernel parameter P
if(nargin == 4) 
    P = 1;
end
n = size(X, 1);
d = size(X, 2); 
assert(length(Y) == n); 
if((size(H,1 ) == n) && size(H, 2))
   H = computeHrcv(X, K, P); 
end    
str = sprintf('computing H^0.5'); disp(str); 
Q = diag(Y)*H*diag(Y);
R = sqrtm(Q); 
% cost 
prob.c = [ones(1, n) -W zeros(1, n)];
% bounds on free variables
prob.blx = [zeros(1, n) 0 -inf*ones(1, n)]; 
prob.bux = [ones(1, n) +inf +inf*ones(1, n)]; 
% constraint formulation
% prob.a = sparse([Y 0]);
A = [ Y' 0 zeros(1, n);
      R  zeros(n, 1) -1*eye(n) ]; 
prob.a = sparse(A);
prob.blc = zeros(n+1, 1);
prob.buc = zeros(n+1, 1);
% conic constraint
prob.cones=cell(1, 1);
prob.cones{1}.type = 'MSK_CT_QUAD';
prob.cones{1}.sub = [n+1 (n+2):(2*n+1)]; 
str = sprintf('problem formulation done'); disp(str);


