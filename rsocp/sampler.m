function [sample] = sampler(n, P)
%% function [sample] = sampler(n, P)
I = []; 
F = exp(-P); 
Z = sum(F); 
for i=1:n
    u = rand();
    k=0;
    s=0;
    while(u > s/Z) 
        k = k+1;
        s = s+F(k);
    end
    I = [I; k];
end
sample = unique(I); 

