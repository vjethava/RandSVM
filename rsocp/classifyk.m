function [Viols, Acc, Yc, Yp] = classifyk(Yd, Xd, soln, Yt, Xt)
n= length(Yd); 
Yp = [];
Yc = [];
for t=1:length(Yt)
    S = computeY_k(Yd, Xd, soln, Xt(t,:));   
    Yc = [Yc; sign(S)];
    Yp = [Yp; S]; 
end
Ysign = abs(sign(Yc - Yt)); 
numWrong = nnz(Ysign); 
Viols = Ysign; 
Acc = 100.0 - (numWrong*100.0)/n;
% Margin = Ys-1.0;