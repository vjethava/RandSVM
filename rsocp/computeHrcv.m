function [H] = computeHrcv(Y, X)
n = size(X, 1);  
H = zeros(n, n); 
 % kernel computation
 for i=1:n
    str = sprintf('Kernel Matrix: finished row  %d of %d', i, n); disp(str); 
    for j=1:n
        H(i,:) = Y(i) * X(i, :);
    end
 end
disp('finished computeHrcv'); 