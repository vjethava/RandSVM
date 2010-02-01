 clear; 
 load rcv1_test.mat;
 load rcv1_train.mat;
N = length(rcv1_topics_ty); 
getK = @(n, e) 16*log(4*n/0.1)/(e*e);
Yd = readCCAT(rcv1_topics_ty, rcv1_topics_tmap); 
Yt = readCCAT(rcv1_topics_y, rcv1_topics_map); 
k =  floor(getK(N, 0.2)); 
A = 1000;
[soln, iterns, totalT] = randcsocpdrcv(Yd, rcv1_tx, A, 2, k, 'small_rcv1'); 
[Viols, Acc] = classify(s.w, s.b, Yt, rcv1_x);
TT = 0;
for i=1:length(iteratns)
    TT = TT + iteratns(i).tsocp + iteratns(i).tactive;
end

