function [Yt] = readCCAT(rcv_topics_y, rcv_topics_map)
NT = length(rcv_topics_y); 
[I, J, S] = find(rcv_topics_y);
CCAT = 34;% rcv_topic_map has 34 at first place 
[TF, ID] = ismember(CCAT, rcv_topics_map); 
assert(TF == 1); 
% make 1 vs rest class
Yt = -1*ones(NT, 1);
for i=1:length(I)
    if(J(i) ==ID)
        Yt(I(i)) = 1;
    end
end
