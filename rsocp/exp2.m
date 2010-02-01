% twonorm classification SOCP 
clear;
normrnd('state', 0); 
a = 2/sqrt(20); 
n = 1000000;
d = 20; 
Y = [ones(n/2, 1); -1*ones(n/2, 1)]; 
X = [normrnd(a, 1, n/2, d); normrnd(-a, 1, n/2, d)];
W = 1; 

delta = 0.1; 
gamma = 0.1
A = norm(max(X)); 
e1 = 0.1;
% k = getSampleSizeSOCP(n, A, W, delta, gamma); 
k = 16*log(n*4/delta)/(e1*e1); 
c = 3; 
% test set 
t = 5000; 
Xt = [normrnd(a, 1, t, d); normrnd(-a, 1, t, d)];
Yt = [ones(t, 1); -1*ones(t, 1)]; 

% t0 = cputime;
% [s0] = solvemosek(Y, X, W); 
% t0 = cputime-t0;
% [Viols0, Acc0]=classify(s0.w, s0.b, Yt, Xt); 

% [Viols, Acc] = classify(s.w, s.b, Yt, Xt);
 
[s, iteratns, totalTime] = randcsocp(Y, X, W, c, k, 'exp2.log'); 
[Viols, Acc] = classify(s.w, s.b, Yt, Xt);


save exp2;

