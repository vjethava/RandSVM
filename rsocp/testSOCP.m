clear; 
%[At, b, c, K] = genSOCP(y,x,w);
%soln = socpsolve(y, x, w); 
load twonorm1.mat; 
W = 1.0; 
prob = genmosek(Y, X, W); 
[r,res]=mosekopt('minimize',prob);