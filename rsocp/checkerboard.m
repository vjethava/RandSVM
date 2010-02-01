n = 1000; 
x = unifrnd(0, 4, n, 2);
y = (mod(floor(x(:, 1)), 2) == mod(floor(x(:, 2)), 2)); 
w = 1;
