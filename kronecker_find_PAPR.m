clear;
filename = 'sequence_data10.csv';

L=10;
u=9;

sequence_log_1 = readmatrix(filename);
index_1=length(sequence_log_1);
pair_sequence=[];
index_2 = 1;
ACFf_log = zeros(index_1,u);
disp(filename);
disp(['num of sequences: ',num2str(index_1)]);
temp_PAPR = 10 ;

temp_max_p = [];
temp_max_q = [];
temp_sum=[];

%============== correlation儲存 =============%
for i=1:index_1
    ACFf_log(i,:) = xcorr_2(sequence_log_1(i,:),u,L);
end
disp('start...');

%============== 配對 ========================%
for i=1:index_1    
    for j=i:index_1
        flag=1;
        for k = 1:u
            if ACFf_log(i,k) + ACFf_log(j,k) == 0
                flag=flag+1; %算0區間
            else
                break;
            end
        end
        if flag >= 10
            [p,q] = kronecker_product(sequence_log_1(i,:),sequence_log_1(j,:));
            [p,q] = kronecker_product(p,q); 
            L2 =length(p);
            X = linspace(0,1,10000);
            [p,q] = change_0_1(p,q);
            Y_1 = PAPR(p,X)/L2;
            Y_2 = PAPR(q,X)/L2;
            [argvalue_1, argmax_1] = max(Y_1);
            [argvalue_2, argmax_2] = max(Y_2);
            if max(argvalue_1 , argvalue_2) <= temp_PAPR
                temp_max_p = p;
                temp_max_q = q;
                temp_PAPR = max(argvalue_1,argvalue_2) ;
                disp(['f1: ',num2str(sequence_log_1(i,:))]);
                disp(['f2: ',num2str(sequence_log_1(j,:))]);
                disp(['p1: ',num2str(temp_max_p)]);
                disp(['p2: ',num2str(temp_max_q)]);
                disp(['PAPR1: ',num2str(argvalue_1)]);
                disp(['PAPR2: ',num2str(argvalue_2)]);
            end
            
        end
    end
        
end

[f1,f2]=change_1_0(temp_max_p,temp_max_q);
disp(['p1=[',num2str(temp_max_p),'];']);
disp(['p2=[',num2str(temp_max_q),'];']);
disp(['[ ',num2str(f1),' ]']);
disp(['[ ',num2str(f2),' ]']);
disp(['PAPR: ',num2str(temp_PAPR)]);
disp('finish...');



function [f1,f2] = kronecker_product(c,d)
    %GCP
    for i = 1:length(c)
        if c(i) ==0
            c(i) = -1;
        end
    end
    for i = 1:length(d)
        if d(i) ==0
            d(i) = -1;
        end
    end
    a = [1 1 ];
    b = [1 -1];
    a_flip = flip(a);
    b_flip = flip(b);
    L = length(a)*length(c); 
    f1 = [];
    f2 = [];   

    for i=1:length(c)
        for j=1:length(a)
           temp = c(i)*(a(j) + b(j))/2 + d(i)*(a(j)-b(j))/2;
           f1 = [f1,temp];
           temp = c(i)*(a_flip(j)-b_flip(j))/2 - d(i)*(a_flip(j)+b_flip(j))/2;
           f2 = [f2,temp];
        end
    end
end


function z = check_zone(a)
    zone = 1;
    check = 1;
    for i = 2:length(a)
        if check==1
            if a(i) == 0
               zone = zone + 1;
            else
                check = 0;
            end
        end
    end
    z = zone;
end

function A = PAPR(a,x)
    y = 0;
    acf_1 = xcorr(a);
    for u=1:length(a)-1
        y = y + real(acf_1(u+length(a))*exp(sqrt(-1)*2*pi*x*u));
    end
    A = 2*y+length(a);
end

function ACFf = xcorr_2(c1,u,L)
    ACFf = [];
    for i=1:u
        c2=(c1(i+1:L) == c1(1:L-i));
        ones = sum(c2);
        negs = L-i-ones;
        ACFf(i) = ones-negs;
    end
end
function [p,q] = change_0_1(p,q)
    for i = 1:length(p)
    if p(i) ==0
        p(i) = -1;
    end
    end
    for i = 1:length(q)
        if q(i) ==0
            q(i) = -1;
        end
    end
end
function [p,q] = change_1_0(p,q)
    for i = 1:length(p)
    if p(i) ==-1
        p(i) = 0;
    end
    end
    for i = 1:length(q)
        if q(i) ==-1
            q(i) = 0;
        end
    end
end