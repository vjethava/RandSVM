function [H] = computeH(X, K, P)
n = size(X, 1);  
H = zeros(n, n); 
 % kernel computation
 for i=1:n
    str = sprintf('Kernel Matrix: finished row  %d of %d', i, n); disp(str); 
    for j=1:n
        H(i,j) = K(X(i, :), X(j, :), P); 
    end
end