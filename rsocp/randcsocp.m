function [soln, iterns, totalT] = randcsocp(Y, X, W, c, k, fname)
%% function [soln, iterns, totalT] = randcsocp(Y, X, W, c, k, fname)
%   Y - class labels (n \times 1)
%   X - data points  (n \times d)
%   W - ||w|| \le W
disp('starting randcsocp primal'); 
str = sprintf('%s.log', fname); 
str2 = sprintf('%s_tmp', fname); 
fid = fopen(str, 'w'); 
tb= cputime; 
r = c*k; 
n = size(X, 1);
assert (length(Y) == n); 
d = size(X, 2); 
I = floor(rand(r, 1)*(n-1) + 1); % selected index
I = unique(I); 
iterns = [];
Sx = sparse([], [], [], length(I), d);
Sy = zeros(length(I), 1); 
% [ix, jx, sx] = find(X); 
% isx=[]; jsx=[]; ssx=[]; ssy=[];
disp('started sampling points'); 
%for i=1:length(ix) % sampled set - sparsified version 
     % str = sprintf('sparse sample point %d of %d', i, length(ix));
%    disp(str);
%    rx = ix(i);
%    cx = jx(i); 
%    vx = sx(i); 
%    [tf, loc] = ismember(rx, I); 
%    if(tf == 1) 
%        isx = [isx; loc];
%        jsx = [jsx; cx]; 
%        ssx = [ssx; vx];
    %    ssy = [ssy; yy];
%    end
% end
Sz = [] ;
for i = 1:length(I)
    if(mod(i,100) == 0) str = sprintf('sparse sample point %d of %d', i, length(I)); disp(str); end
    index = I(i); 
    Sx(i, :) = X(index, :);
    Sy(i) = Y(index); 
    Sz = [Sz; X(index, :)*Y(index)] ; 
end
% for i=1:length(I)
%    index = I(i); 
%    Sy(i) = Y(index); 
% end
% Sx = sparse(isx, jsx, ssx, length(I), d); 
disp('finished sampling points'); 
CS = [];
t1 = cputime;
disp(' starting socp_solve iteration'); 
[soln] = solvemosek(Sy, Sx, W);
disp(' finished socp_solve iteration'); 
t3 = cputime;
[V, Vx, Vy, T, Tx, Ty, Vz, Tz] = findActive(Y, X, I, soln);
t2 = cputime; 
CS.tsocp = t3 - t1;
CS.tactive = t2 -t3; 
CS.soln = soln; 
iterns = [iterns; CS];
save str2 iterns soln; 
nT = length(T); % support vectors
nV = length(V); % violators from unsampled points
nS = length(I); % sampled points
[msg] = sprintf('Support Vectors: %d of %d sampled point; unsampled violators: %d \n', nT, nS, nV);  disp(msg); 
fprintf(fid, '%s\n', msg); 
count = 0; 
while(( nV > 0) & (nT < k))
    count = count + 1; 
    [msg] = sprintf(' Iteration: %d, nSV: %d SampleSize(V): %d SVsize(S): %d', count, nT , nS, nV);  disp(msg); 
    I = [V; T]; Sx = [Vx; Tx]; Sy = [Vy; Ty]; 
    CS = [];
    t1 = cputime;
    disp(' starting socp_solve iteration'); 
    [soln] = solvemosek(Sy, Sx, W);
    disp(' finished socp_solve iteration'); 
    % soln = solvemosek(Sy, Sx, W); 
    t3 = cputime; 
    
    [V, Vx, Vy, T, Tx, Ty] = findActive(Y, X, I, soln);
    t2 = cputime; 
    CS.tsocp = t3 - t1;
    CS.tactive = t2 -t3; 
    CS.soln = soln; 
    iterns = [iterns; CS]; 
    save str2 iterns soln; 
    nT = length(T); % support vectors
    nV = length(V); % violators from unsampled points
    nS = length(I); % sampled points
    [msg] = sprintf('Support Vectors: %d of %d sampled point; unsampled violators: %d \n', nT, nS, nV);  disp(msg); 
    fprintf(fid, '%s\n', msg); 
 %    fflush(fid); 
end
tc=  cputime;
totalT = tc -tb; 
fclose(fid); 

