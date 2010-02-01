function [V, Vx, Vy, T, Tx, Ty] = findActiveDrcv(Y, X, I, soln)
str = sprintf(' computing active set for next iteration '); disp(str);
  n = length(Y); 
  V = []; Vx = []; Vy = [];  
  T = []; Tx = []; Ty = []; 
  countsv = 0; 
   for i = 1:length(I)
       index= I(i); 
       if(soln.mu(i) > 1.0e-6)
           countsv = countsv + 1; 
           disp(sprintf(' added support vector %d (index: %d of %d)', countsv, i, length(I)));
           T = [T; index];
           Tx = [Tx; X(index, :)]; 
           Ty = [Ty; Y(index)];
              
       end
   end
  % Pick up new sampling of data set
    % for i=1:n
        %if(mod(i, 100) == 0) 
    %        str = sprintf('iteration %d of %d', i, n); disp(str); 
%        end
    %    [ismem, loc] = ismember(i, I); 
    %    if(ismem > 0)
    %        if(soln.mu(loc) > 1e-4)
    %            T =  [T; i]; 
    %            Tx = [Tx; X(i, :)];
    %            Ty = [Ty; Y(i)]; 
    %        end
    %    else
    %        if(Y(i) * ( X(i, :) * soln.w' + soln.b) < 1.0)
    %            V = [V; i];     
    %            Vx = [Vx; X(i, :)];
    %            Vy = [Vy; Y(i)]; 
    %        end
    %    end
    %  end
    % Alternative fast computation of active constraints;
    Z = Y .* (X*soln.w') +  soln.b*Y - 1.0;
    ZS = (-1*sign(Z)+1.0);
    [zi, zj, zs] = find(ZS); 
    disp(sprintf(' number of violators: %d', length(V))); 
    countv=0; 
    for ii=1:length(zi)
      [ismem, loc] = ismember(ii, T);
      if(ismem == 0)
          countv = countv+1; 
          V = [V; zi(ii)]; 
          str = sprintf('added violator %d iteration %d of %d', countv, ii, length(zi)); disp(str); 
          Vx = [Vx; X(ii, :)]; 
          Vy = [Vy; Y(ii)]; 
      end
    end
    assert(size(Tx, 1) == length(Ty));
    assert(size(Vx, 1) == length(Vy));
    disp(' findActiveDrcv done'); 