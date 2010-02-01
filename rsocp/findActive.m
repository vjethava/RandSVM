function [V, Vx, Vy, T, Tx, Ty, Vz, Tz] = findActive(Y, X, I, soln)
  str = sprintf(' computing active set for next iteration '); disp(str);
  n = length(Y); 
  V = []; Vx = []; Vy = []; Tz = []; 
  T = []; Tx = []; Ty = []; Vz = []; 
    % Pick up new sampling of data set
 %    Mclassifier = Y.*(X*soln.w + soln.b*ones(n,1)); 

    for i=1:n
        if(mod(i, 100) == 0)  disp(sprintf('findActive: check active %d of %d', i, n));  end
        [ismem, loc] = ismember(i, I); 
        if(ismem > 0)
            if(soln.mu(loc) > 1e-4)
                T =  [T; i]; 
                Tx = [Tx; X(i, :)];
                Ty = [Ty; Y(i)]; 
                Tz = [Tz; X(i, :)*Y(i)]; 
            end
        else
            % if(Mclassifier(i) < 1.0)
            if(Y(i) * ( X(i, :) * soln.w + soln.b) < 1.0)
                V = [V; i];     
                Vx = [Vx; X(i, :)];
                Vy = [Vy; Y(i)]; 
                Vz = [Vz; X(i, :)*Y(i)]; 
                
            end
        end
    end
    assert(size(Tz, 1) == length(Ty));
    assert(size(Vz, 1) == length(Vy));
    disp('done'); 
    