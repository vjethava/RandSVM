function [soln, iterns, totalT, Sy, Sx] = randcsocpdrcv(Y, X, W, c, k, fname)
%% function [soln, iterns, totalT, Sy, Sx] = randcsocpd(Y, X, W, K, P)
% K - kernel function
% P - kernel parameter
str = sprintf('%s.log', fname); 
fid = fopen(str, 'w'); 
tb= cputime; 
r = c*k; 
n = size(X, 1);
% pre compute kernel matrix
disp('calling computeHrcv'); 
% H = computeHrcv(Y, X); 
L = 100; 
assert (length(Y) == n); 
d = size(X, 2); 
I = floor(rand(floor(r/L), 1)*(n-1-L) + 1); % selected index
I = unique(I); 
iterns = [];
Sx = sparse([], [], [], length(I)*L, d);
Sy = sparse([], [], [], length(I)*L, 1); 

for i=1:length(I) % sampled set 
    disp(sprintf(' sampling %d of %d', i, length(I)));
    index = I(i); 
    Sx(((i-1)*L+1):(i*L), :) = X((index+1):(index+L), :);
    Sy(((i-1)*L+1):(i*L)) = Y((index+1):(index+L));
end
% compute kernel matrix for S 
%Hs = zeros(length(I), length(I)); 
%for i=1:length(I)
%    for j=1:length(I)
%        disp(str(' computing Hs(%d,%d) of %d^2', i, j, length(I));
%        Hs(i,j) = H(I(i), I(j));
%    end
%end



CS = [];
t1 = cputime;
disp('calling solvedualrcv'); 
%[soln] = solvedualrcv(Sy, Sx, W, Hs, K, P);
[soln] = solvedualrcv(Sy, Sx, W);
nnz(soln.mu)
pause(2);
t3 = cputime;
disp('calling findActiveDrcv'); 
[V, Vx, Vy, T, Tx, Ty] = findActiveDrcv(Y, X, I, soln);
t2 = cputime; 
CS.tsocp = t3 - t1;
CS.tactive = t2 -t3; 
CS.soln = soln; 
iterns = [iterns; CS]; 
nT = length(T); % support vectors
nV = length(V); % violators from unsampled points
nS = length(I); % sampled points
[msg] = sprintf('Support Vectors: %d of %d sampled point; unsampled violators: %d \n', nT, nS, nV);  disp(msg);
pause(2); 
fprintf(fid, '%s\n', msg); 
count = 0; 
save fname iterns; 
while(( nV > 0) & (nT < k))
    count = count + 1; 
    [msg] = sprintf(' Iteration: %d, nSV: %d SampleSize(V): %d SVsize(S): %d', count, nT , nS, nV);  disp(msg); 
    I = [V; T]; Sx = [Vx; Tx]; Sy = [Vy; Ty]; 
    CS = [];
    t1 = cputime;
    disp('calling solvedualrcv'); 
    % soln = solvedualrcv(Sy, Sx, W, K, P); 
    soln = solvedualrcv(Sy, Sx, W); 
    t3 = cputime; 
    disp('calling findActiveDrcv'); 
    [V, Vx, Vy, T, Tx, Ty] = findActiveDrcv(Y, X, I, soln);
    t2 = cputime; 
    CS.tsocp = t3 - t1;
    CS.tactive = t2 -t3; 
    CS.soln = soln; 
    iterns = [iterns; CS]; 
    nT = length(T); % support vectors
    nV = length(V); % violators from unsampled points
    nS = length(I); % sampled points
    [msg] = sprintf('Support Vectors: %d of %d sampled point unsampled violators: %d \n', nT, nS, nV);  disp(msg); 
    pause(2); 
    fprintf(fid, '%s\n', msg); 
%     fflush(fid); 
    save fname iterns; 
end
tc=  cputime;
totalT = tc -tb; 
fclose(fid); 

