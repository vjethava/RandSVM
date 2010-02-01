% twonorm classification SOCP 
clear;
normrnd('state', 0); 

a = 2/sqrt(20); 
getK = @(n, e) 16*log(4*n/0.1)/(e*e);
%mu1 = ones(1,20)*a; 
%mu2 = ones(-1,20)*a;
n = 1000;
d = 20; 
Y = [ones(n/2, 1); -1*ones(n/2, 1)]; 
X = [normrnd(a*0.1, 0.1, n/2, d); normrnd(-a*0.1, 0.1, n/2, d)];
W = 10; 
% [s, r, res] = solvemosek(Y, X, W);
% save mosek_twonorm.mat;
% clear; 
% load mosek_twonorm; 
t = 5000; 
delta = 0.1; 
gamma = 0.1
A = norm(max(X)); 
k = getSampleSizeSOCP(n, A, W, delta, gamma); 
Xt = [normrnd(a, 1, t, d); normrnd(-a, 1, t, d)];
Yt = [ones(t, 1); -1*ones(t, 1)]; 
% [Viols, Acc] = classify(s.w, s.b, Yt, Xt);
k = getK(10000, 0.2);
c = 2; 
[s, iteratns, totalTime] = randcsocp(Y, X, W, c, k, 'twonorm1LATEST'); 
[Viols, Acc] = classify(s.w, s.b, Yt, Xt);

TT = 0;
for i=1:length(iteratns)
    TT = TT + iteratns(i).tsocp + iteratns(i).tactive;
end

