function [k] = gaussian_kernel(X1, X2, P)
assert(length(X1) == length(X2)); 
k = exp(-norm(X1-X2)/(2*P)); 