clear;
filename = 'sequence_data.csv';
n=40;
n_file=n;
z=1;
pre_space=100;

%% 
logs_num=0;
count_big_section=0;
%==================== 配對 ==============================

%讀取sections
csvFileName1 = ['sequence_data' num2str(n_file) '.csv'];

variableName1 = ['data' num2str(n)];
eval([variableName1 ' = readmatrix(csvFileName1);']);
sequence_logs_1 = eval(variableName1);

%計算ACF
pair_sequence=[];
L= length(sequence_logs_1(1,:));
u=L-1;
temp_PAPR = 10 ;
temp_max_p = zeros(1,L);
temp_max_q = zeros(1,L);
zmax = round(L/2);
index_1 = length(sequence_logs_1(:,1));
ACFf_log_1 = zeros(index_1,u);
disp(['L:',num2str(L)]);
match_layer=1; % =================================注意：match_layer不能設定比會出現的z還要大
total_section= factorial(match_layer+1);
factorials=zeros(1,match_layer+1);
for i=1:match_layer+1
    factorials(i)=factorial(i);
end
basis_data = cell(1, match_layer);
for i = 1:match_layer
    basis_data{i} = -i:2:i;
end
for i=1:index_1
    ACFf_logs_1(i,:) = xcorr_2(sequence_logs_1(i,:),u,L);
end



for i=1:index_1   
    for j=i:index_1
        flag=0;
        j_loop=true;
        for k = 1:match_layer
            if ACFf_logs_1(i,k) + ACFf_logs_1(j,k) == 0
            else
                j_loop=false;
                break;
            end
        end

        if j_loop==false
            continue;
        end
        %if上面能pass至少有match_layer個flag
        flag=match_layer;
        for k = match_layer+1:zmax
            if ACFf_logs_1(i,k) + ACFf_logs_1(j,k) == 0
                if ACFf_logs_1(i,L-k) + ACFf_logs_1(j,L-k) == 0
                    flag=flag+1; %算0區間          
                else
                    break;
                end
            else
                break;
            end
        end
        if flag == z
            p = sequence_logs_1(i,:);
            q = sequence_logs_1(j,:);
            [p,q] = change_0_1(p,q);
            X = linspace(0,1,10000);
            Y_1 = PAPR(p,X)/L;
            Y_2 = PAPR(q,X)/L;
            argvalue_1= max(Y_1);
            argvalue_2 = max(Y_2);
            if max(argvalue_1 , argvalue_2) < temp_PAPR
                temp_max_p = p;
                temp_max_q = q;
                temp_PAPR = max(argvalue_1,argvalue_2) ;
                disp(['p1=[',num2str(sequence_logs_1(i,:)),'];']);
                disp(['p2=[',num2str(sequence_logs_1(j,:)),'];']);
                disp(['PAPR: ',num2str(max(argvalue_1 , argvalue_2))]);
                disp(['z: ',num2str(flag)]);
            end
        elseif flag > z
            p = sequence_logs_1(i,:);
            q = sequence_logs_1(j,:);
            [p,q] = change_0_1(p,q);
            X = linspace(0,1,10000);
            Y_1 = PAPR(p,X)/L;
            Y_2 = PAPR(q,X)/L;
            argvalue_1= max(Y_1);
            argvalue_2 = max(Y_2);
            temp_max_p = p;
            temp_max_q = q;
            temp_PAPR = max(argvalue_1,argvalue_2) ;
            disp(['p1=[',num2str(sequence_logs_1(i,:)),'];']);
            disp(['p2=[',num2str(sequence_logs_1(j,:)),'];']);
            disp(['PAPR: ',num2str(max(argvalue_1 , argvalue_2))]);
            disp(['z: ',num2str(flag)]);
            z=flag;
        end
    end          
end


pair_sequence(1,:) = temp_max_p;
pair_sequence(2,:) = temp_max_q;
[f1,f2]=change_1_0(temp_max_p,temp_max_q);
disp(['p1=[',num2str(temp_max_p),'];']);
disp(['p2=[',num2str(temp_max_q),'];']);
disp(['[ ',num2str(f1),' ]']);
disp(['[ ',num2str(f2),' ]']);
disp(['PAPR: ',num2str(temp_PAPR)]);
disp(['z: ',num2str(z)]);
disp('finish...');
%writematrix(pair_sequence,filename,'WriteMode','append');


%==================================  function  ====================================


function ACFf = xcorr_2(c1, u, L)
    ACFf = zeros(1,u);
    for i = 1:u
        c2 = (c1(i + 1:L) == c1(1:L - i));
        ones = sum(c2);
        negs = L - i - ones;
        ACFf(i) = ones - negs;
    end
end


function section_index = find_section_index(ACF,match_layer,factorials,basis_data)

    basis_index=zeros(1,match_layer);
    flip_ACF=flip(ACF);

    for i = 1:match_layer
        % basis_index(i) = find(basis_data{i} == flip_ACF(i));
        for j=1:length(basis_data{i})
            if basis_data{i}(j) == flip_ACF(i)
                basis_index(i)=j;
                break;
            end
        end
    end

    section_index=1;
    for i=1:match_layer
        section_index=section_index+(basis_index(i)-1)*(factorials(match_layer+1) / factorials(i+1));
    end
end


function A = PAPR(a,x)
    y = 0;
    acf_1 = xcorr(a);
    for u=1:length(a)-1
        y = y + real(acf_1(u+length(a))*exp(sqrt(-1)*2*pi*x*u));
    end
    A = 2*y+length(a);
end


% 利用 x 組成完整的c序列
function c=Generate_c(x,L)
    c=zeros(1,L);
    c(1)=-1;
    top=L-1;
    for i=1:top
        c(i+1)=x(i)/c(i);
    end
    % c=(c+1)/2;
end

function x=JUMP(x)
%如果最後一個位置不是-1，則最後一個-1跟下一個換位置
    if x(length(x))~=-1
        lastZeroIndex = find(x == -1, 1, 'last');
        x(lastZeroIndex)=1;
        x(lastZeroIndex+1)=-1;
    else
        if x(length(x)-2)==-1 &&x(length(x)-1)==1&& x(length(x))==-1
            x(length(x)-2:length(x))=[1 -1 -1];
        else
            %檢查最後連續幾個-1，n個
            n=NumConsecutiveZeros(x);
            %找到倒數第(n+1)個-1的位置
            undercount=Undercount(x,n+1);
            %利用位置index調換-1 1
            x(undercount)=1;
            x(undercount+1:undercount+1+n)=-1;
            if length(x)-(n-1)>undercount+1+n
                x(length(x)-(n-1):length(x))=1;
            else
                x(undercount+2+n:length(x))=1;
            end
        end
    end
end

%檢查x的倒數位置有連續幾個-1
function numConsecutiveZeros=NumConsecutiveZeros(x)
    % 找到最後一個非-1元素的位置
    lastNonZeroIndex = find(x ~= -1, 1, 'last');
    % 計算最後連續的-1的個數
    numConsecutiveZeros = length(x) - lastNonZeroIndex;
end

function lastZeroIndex=Undercount(x,n)
    % 找到最後一個-1的位置
    lastZeroIndex = find(x == -1, 1, 'last');
    % 循環找到倒數第n個-1的位置
    for i = 1:n-1
        lastZeroIndex = find(x(1:lastZeroIndex-1) == -1, 1, 'last');
        if isempty(lastZeroIndex)
            % 處理未找到的情况
            break;
        end
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