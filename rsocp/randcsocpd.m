function [soln, iterns, totalT, Sy, Sx] = randcsocpd(Y, X, W, c, k, fname, K, P)
%% function [soln, iterns, totalT, Sy, Sx] = randcsocpd(Y, X, W, K, P)
% K - kernel function
% P - kernel parameter
str = sprintf('%s.log', fname); 
fid = fopen(str, 'w'); 
tb= cputime; 
r = c*k; 
n = size(X, 1);
% pre compute kernel matrix
H = computeH(X, K, P); 

assert (length(Y) == n); 
d = size(X, 2); 
I = floor(rand(r, 1)*(n-1) + 1); % selected index
I = unique(I); 
iterns = [];
Sx = zeros(length(I), d);
Sy = zeros(length(I), 1); 


for i=1:length(I) % sampled set 
    index = I(i); 
    Sx(i, :) = X(index, :);
    Sy(i) = Y(index); 
end
% compute kernel matrix for S 
Hs = zeros(length(I), length(I)); 
for i=1:length(I)
    for j=1:length(I)
        Hs(i,j) = H(I(i), I(j));
    end
end

CS = [];
t1 = cputime;
[soln] = solvedual(Sy, Sx, W, Hs, K, P);
t3 = cputime;
[V, Vx, Vy, T, Tx, Ty] = findActiveD(Y, X, I, soln, Sy, Sx);
t2 = cputime; 
CS.tsocp = t3 - t1;
CS.tactive = t2 -t3; 
CS.soln = soln; 
iterns = [iterns; CS]; 
nT = length(T); % support vectors
nV = length(V); % violators from unsampled points
nS = length(I); % sampled points
[msg] = sprintf('Support Vectors: %d of %d sampled point; unsampled violators: %d \n', nT, nS, nV);  disp(msg); 
fprintf(fid, '%s\n', msg); 
count = 0; 
save fname iterns; 
while(( nV > 0) & (nT < k))
    count = count + 1; 
    [msg] = sprintf(' Iteration: %d, nSV: %d SampleSize(V): %d SVsize(S): %d', count, nT , nS, nV);  disp(msg); 
    I = [V; T]; Sx = [Vx; Tx]; Sy = [Vy; Ty]; 
    CS = [];
    t1 = cputime;
    soln = solvedual(Sy, Sx, W, K, P); 
    t3 = cputime; 
    [V, Vx, Vy, T, Tx, Ty, Iv, It] = findActiveD(Y, X, I, soln, Sy, Sx);
    t2 = cputime; 
    CS.tsocp = t3 - t1;
    CS.tactive = t2 -t3; 
    CS.soln = soln; 
    iterns = [iterns; CS]; 
    nT = length(T); % support vectors
    nV = length(V); % violators from unsampled points
    nS = length(I); % sampled points
    [msg] = sprintf('Support Vectors: %d of %d sampled point; unsampled violators: %d \n', nT, nS, nV);  disp(msg); 
    fprintf(fid, '%s\n', msg); 
%     fflush(fid); 
    save fname iterns; 
end
tc=  cputime;
totalT = tc -tb; 
fclose(fid); 

