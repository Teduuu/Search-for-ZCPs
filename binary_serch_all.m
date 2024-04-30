%clear all;
filename = 'sequence_data.csv';
L=18; 
sequence = zeros(1,L);
sequence_log = [];
index_0 = 1;

%========== 搜尋可能的序列 ===============
for i=1:2^L-1
    sequence = binary_add(sequence,L);
    if sum(sequence)<=2 || sum(sequence)>=9 %篩掉1或0太多的可能
        continue;
    else
       sequence_log(index_0,:) = sequence;
       index_0 = index_0 + 1;
    end
end 
disp(['number of sequences: ',num2str(length(sequence_log))]);


writematrix(sequence_log,filename);


function flag = xcorr_1(c1,u,L)
    ACFf = [];
    for i=1:u
        flag = false;
        c2=(c1(i+1:L) == c1(1:L-i));
        ones = sum(c2);
        negs = L-i-ones;
        ACFf(i) = ones-negs;
        flag=true;     
    end
end

function new_sequence = binary_add(sequence,L)
    sequence(L) = sequence(L) + 1;
    for i=1:L
        if sequence(L+1-i) == 2
           sequence(L+1-i) =0;
           sequence(L-i) = sequence(L-i) + 1;      
        else
            break;
        end
    end
    new_sequence = sequence;
end
