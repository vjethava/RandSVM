% ringnorm.m
clear; 
getK = @(n, e) 16*log(4*n/0.1)/(e*e);
n = 10000;
normrnd('state', 0); 
a = 2/sqrt(20); 
d = 20; 
Y = [ones(n/2, 1); -1*ones(n/2, 1)]; 
X = [normrnd(a, 1, n/2, d); normrnd(-a, 1, n/2, d)];
W = 10; 
delta = 0.1; 
gamma = 0.1;
A = norm(max(X)); 
e1 = 0.1;
K = @gaussian_kernel; 
P = 1.0; 
t = 300; 
Yt = [ones(t/2, 1); -1*ones(t/2, 1)]; 
Xt = [normrnd(a, 1, t/2, d); normrnd(-a, 1, t/2, d)];

% dualprob = genmosekd(Y, X, W, @gaussian_kernel, 1);
[soln, iterns, totTime ] = randcsocpd(Y, X, W, 1.5, 3000, 'ringnorm10000' , K, 1.0); 
[Viols, AccRAND, Yc, Yp] = classifyk(soln.Y, soln.X, soln, Yt, Xt);  

Trand=0;
for i=1:length(iterns)
    Trand = Trand + iterns(i).tsocp;
end

tSOCP = cputime;
[soln0, r0, res0] = solvedual(Y, X, W, K, 1.0); 
tSOCP = cputime - tSOCP; 
[Viols0, AccSOCP, Yc0, Yp0] = classifyk(soln0.Y, soln0.X, soln0, Yt, Xt);

save ringnorm10000;

