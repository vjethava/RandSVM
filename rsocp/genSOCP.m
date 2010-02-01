function [At, b, c, K] = genSOCP(Y, X, W)
%% Function that generates the SOCP matrices corresponding to C-SOCP-1
% [At, b, c, K] = genSOCP(Y, X, W)
% C-SOCP-1: \min_{w, b, \xi} \sum_i \xi_i
%            s.t. y_i(w^T x_i + b ) + \ge  1 - \xi_i , ||w||_2 \le W,
%                 x_i \in R^d \forall i = 1...n
%
% Variables: [ b \xi_1 ... \xi_n \gamma_1 ... \gamma_n p w_1 ... w_d]
%   Free:   b                       (K.f = 1)
%   Bound:  p, \xi_1 ... \xi_n >= 0 (K.l=n*2)
%   Quad:   p = ||[w_1 ... w_d]||   (K.q=d+1)
% 
%        
% MULT:  b    \xi        \gamma        p            w
% *******************************************************************
% At = [ Y  eye(n)        eye(n)    zeros(n, 1)     Z;
%        0 zeros(1, n)   zeros(1,n)    1       zeros(1, d) ];
% 
% y_i b + \xi_i + \gamma_i + \sum_j x_ij y_i w_j = 1
% 
n = length(Y);
sx = size(X); 
d = sx(2); 
b = [ones(n, 1); W]; 
J = [1:n]'+1; I = ones(n, 1); S=ones(n,1);  
c = sparse(I, J, S, 1,  (2*n+d+2)); 
K.f = 1;
K.l = 2*n;
K.q = d+1;
Z = X;
for i=1:n
    Z(i, :) = Z(i, :).*Y(i); 
end
%%%%%% OBTAIN SPARSE REPRESENTATION OF At %%%%%%%%%%%%%%%%%%%%%
%At = [ Y zeros(n, 1)     speye(n)     zeros(n, 1)     Z;
%      0    1           zeros(1, n)     1       zeros(1, d) ];
I = []; J = []; S = []; nzmax = 0; 
ar = (n+1); ac = 2*n+d+2; 
current = 0; 
R=[1:n]';
% Add elements of Y
%for i=1:n
%    I = [I; i];
%    J = [J; 1];
%    S = [S; Y(i)];
%    nzmax = nzmax+1;
%end
I = [I; R]; 
J = [J; ones(n, 1)]; 
S = [S; Y]; 
nzmax = nzmax+n;
current = current+1; 
% Add elements of \xi
%for i=1:n
%    I = [I; i];
%    J = [J; 2+i];
%    S = [S; 1];
%    nzmax = nzmax+1;
%end
I = [I; R]; 
J = [J; current+R]; 
S = [S; ones(n, 1)]; 
nzmax = nzmax+n;
current = current+n;
% Add elements of \gamma
I = [I; R]; 
J = [J; current+R]; 
S = [S; ones(n, 1)]; 
nzmax = nzmax+n;
current = current+n;
% Add 1 corresponding to p (=||w||)
current = current+1;
I = [I; n+1];
J = [J; current];
S = [S; 1];
nzmax = nzmax+1;

% Add elements of Z
for k=1:d
%    for l=1:n
%        I = [I; l];
%        J = [J;  n+3+k];
%        S = [S; Z(l, k)];
%        nzmax = nzmax+1;
%    end
    I = [I; R]; 
    J = [J; ones(n,1)*(current+k)]; 
    S = [S; Z(:, k)]; 
    nzmax = nzmax+n;
end
current = current+d; 
% Add 1 corresponding to q \ge 0
% I = [I; n+1];
% J = [J; 2];
% S = [S; 1];
% nzmax = nzmax+1;

% Finally, compose At
At = sparse(I, J, S, ar, ac); 


