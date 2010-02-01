function [k] = getSampleSizeSOCP(n, A, L, delta, gamma)
%% function [k] = getSampleSizeSOCP(A, L, delta, gamma)
% n - number of samples
% A - maximum ||w||
% L - maximum ||x_i||
% delta - probability error
% gamma - distortion factor 
k = 2*(( 2 + A^2 + L^2)/(gamma))^2 * log(4*n/delta);