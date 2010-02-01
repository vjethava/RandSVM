function [prob] = genmosekdrcv(Y, X, W)
% formulates the dual variables: [ mu_1 .. mu_1 zeta t_1 ... t_d]
% Y - class labels
% X - data 
% W - max ||w||
% K - the kernel function
% kernel parameter P
% if(nargin == 4) 
%     P = 1;
% end
n = size(X, 1);
d = size(X, 2); 
assert(length(Y) == n); 
% if((size(H,1 ) == n) && size(H, 2))
%  H = computeHrcv(X); 
% end    
% str = sprintf('computing H^0.5'); disp(str); 
% Q = diag(Y)*H*diag(Y);
% R = sqrtm(Q); 
% cost 
prob.c = [ones(1, n) -W zeros(1, d)];
% bounds on free variables
prob.blx = [zeros(1, n) 0 -inf*ones(1, d)]; 
prob.bux = [ones(1, n) +inf +inf*ones(1, d)]; 
% constraint formulation
% prob.a = sparse([Y 0]);
disp(' Allocating genmosek A matrix'); 
% A = [ Y' 0 zeros(1, d);
%      X'*diag(Y)  zeros(d, 1) -1*speye(d) ]; 
% prob.a = sparse(A);
ai=[];
aj=[];
as=[];
% insert Y; 
ai = [ai; ones(n, 1)]; 
aj = [aj; (1:n)']; 
as = [as; Y];
% Z = X'*diag(Y); 
Yd = sparse(diag(Y)); 
[zi, zj, zs] = find(X' * Yd); 
zi = zi+1; 
as = [as; zs];
ai = [ai; zi];
aj = [aj; zj];
% insert speye
es = -1 * ones(d, 1); 
ej = n+1 + [1:d]'; 
ei = 1 + [1:d]'; 
ai = [ai; ei];
aj =  [aj; ej]; 
as = [ as; es]; 
prob.a = sparse(ai, aj, as, d+1, n+d+1); 
prob.blc = zeros(1+d, 1);
prob.buc = zeros(1+d, 1);
% conic constraint
prob.cones=cell(1, 1);
prob.cones{1}.type = 'MSK_CT_QUAD';
prob.cones{1}.sub = [n+1 (n+2):(n+d+1)]; 
str = sprintf('problem formulation done'); disp(str);


