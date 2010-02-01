% cqo1.m

clear prob;

% First the non conic part of the problem is specified.

prob.c   = [0 0 0 0 1 1];
prob.a   = sparse([1 1 1 1 0 0]);
prob.blc = 1;
prob.buc = 1;
prob.blx = [0 0 0 0 -inf -inf];
prob.bux = inf*ones(6,1);

% Next the cones are specified.

% First an empty cell array named
% cones is defined. It should contain
% one cell per cone.

prob.cones   = cell(2,1);

% The first cone is specified.

prob.cones{1}.type = 'MSK_CT_QUAD';
prob.cones{1}.sub  = [5 3 1];

% The subfield type specifies the cone type
% i.e. whether it is quadratic cone
% or rotated quadratic cone. The keys
% for the two cone types are MSK_CT_QUAD
% MSK_CT_RQUAD respectively.
%
% The subfield sub specifies the members
% of the cone. I.e. the above definition
% implies x(5) >= sqrt(x(3)^2+x(1)^2)

% The second cone is specified.

prob.cones{2}.type = 'MSK_CT_QUAD';
prob.cones{2}.sub  = [6 2 4];

% Finally, the problem is optimized.

[r,res]=mosekopt('minimize',prob);

% The primal solution is displayed.
