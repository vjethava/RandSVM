function [S] = computeY_k(Yd, Xd, soln, X)
S = 0; 
for i=1:length(Yd)
    S = S + 1/soln.gamma * soln.mu(i) * Yd(i)*soln.K(Xd(i, :), X , soln.P);
end
S = S + soln.b; 