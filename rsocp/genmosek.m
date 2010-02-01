function [prob] = genmosek(Y, X, W)
% generate parameters for mosek 
% [xi_1 ... xi_n w_1 ,,, w_d b t]
% t \ge ||w||
% t = W
%
disp('generating mosek primal problem'); 
n = length(Y);
d = size(X, 2); 
%if nargin == 3
%    Z = sparse(X);
%    for i=1:n
    %    disp(sprintf('genmosek: generating Z(i=%d) of %d', i, n)); 
%        Z(i, :) = Z(i, :)*Y(i); 
%    end
%else
    disp('genmosek: Using factory generated Z');    
% end
Z = diag(Y)*X; 
prob = []; 
% set-up problem cost multiplier c
prob.c = [ones(1, n) zeros(1, d+2)];
disp('genmosek: generated c');    
% set-up equality constraint as b \le Ax \le b
prob.blc = [ones(1, n)];
disp('genmosek: generated blc');    
prob.buc = [inf*ones(1, n)];
disp('genmosek: generated buc');    
% set-up limits on the variables 
prob.bux = [+inf*ones(n+d+1,1); W]';
disp('genmosek: generated bux');    
prob.blx = [zeros(n,1); -inf*ones(d,1); -inf; W]';
disp('genmosek: generated blc');    
% set-up the multiplier 
I =[];
J =[]; 
S =[]; 
nz = 0; 
%prob.a = sparse([ speye(n)  Z Y zeros(n, 1);
%                  sparse([zeros(1, n+1+d) 1])]); 
% add I_n
R = [1: n]';
% speye(n)
I = [I; R]; 
J = [J; R]; 
S = [S; ones(n,1)]; 
% add Z
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% for k=1:d
%    I = [I; R]; 
%    J = [J; (n+k)*ones(n,1)];
%    S = [S; Z(:, k)];
% end
% Faster code to avoid loop
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 I = [I; reshape(R*ones(1, d), n*d, 1) ]; 
 J1 = n*ones(n*d, 1); 
 J2 = reshape(([1:d]'*ones(1, n))', n*d, 1); 
 J = [J; J1+J2]; 
 assert(size(Z, 1) == n); 
 assert(size(Z, 2) == d); 
 
 S = [S; reshape(Z , n*d, 1)]; 
% add Y
I =[I; R];
J =[J; (n+d+1)*ones(n,1)];
S =[S; Y];
prob.a = sparse(I, J, S, n, n+d+2); 
% set-up the conic constriant
prob.cones   = cell(1,1);
prob.cones{1}.type = 'MSK_CT_QUAD';
prob.cones{1}.sub  = [n+d+2 (n+1):(n+d)];
disp('generating mosek primal problem ... done'); 