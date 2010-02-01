function [V, Vx, Vy, T, Tx, Ty] = findActiveD(Y, X, I, soln, Sy, Sx)
str = sprintf(' computing active set for next iteration '); disp(str);
  n = length(Y); 
  V = []; Vx = []; Vy = [];  
  T = []; Tx = []; Ty = []; 
    % Pick up new sampling of data set
    for i=1:n
        if(mod(i, 100) == 0) 
            str = sprintf('iteration %d of %d', i, n); disp(str); 
        end
        [ismem, loc] = ismember(i, I); 
        if(ismem > 0)
            if(soln.mu(loc) > 1e-4)
                T =  [T; i]; 
                Tx = [Tx; X(i, :)];
                Ty = [Ty; Y(i)]; 
            end
        else
            if(Y(i)*computeY_k(Sy, Sx, soln, X(i, :)) < 1.0)
                V = [V; i];     
                Vx = [Vx; X(i, :)];
                Vy = [Vy; Y(i)]; 
            end
        end
    end
    assert(size(Tx, 1) == length(Ty));
    assert(size(Vx, 1) == length(Vy));
    disp('done'); 