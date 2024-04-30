clear;

for file=45
    csvFileName = ['sequence_section_' num2str(file) '.csv'];
    variableName = ['data' num2str(file)];
    eval([variableName ' = readmatrix(csvFileName);']);
    sequence_log = eval(variableName);
     
    
    L=43;
    u=21; % 要算多長的ACF
    index_1=length(sequence_log);
    pair_sequence=[];
    index_2 = 1;
    ACFf_log = zeros(index_1,u);
    
    ACFf_log_1=zeros(index_1,u);
    ACFf_log_3=zeros(index_1,u);
    ACFf_log_5=zeros(index_1,u);
    ACFf_log_other=zeros(index_1,u);
    
    sequence_log_1=zeros(index_1,43);
    sequence_log_3=zeros(index_1,43);
    sequence_log_5=zeros(index_1,43);
    sequence_log_other=zeros(index_1,43);
    
    log_index_1=1;
    log_index_3=1;
    log_index_5=1;
    log_index_other=1;
    
    disp(csvFileName);
    disp(['num of sequences: ',num2str(index_1)]);
        %============== correlation儲存 =============%
    for i=1:index_1
        ACFf_log(i,:) = xcorr_2(sequence_log(i,:),u,L);
        if abs(ACFf_log(i,10)) == 1
            ACFf_log_1(log_index_1,:)=ACFf_log(i,:);
            sequence_log_1(log_index_1,:)=sequence_log(i,:);
            log_index_1=log_index_1+1;
        elseif abs(ACFf_log(i,10)) == 3
            ACFf_log_3(log_index_3,:)=ACFf_log(i,:);
            sequence_log_3(log_index_3,:)=sequence_log(i,:);
            log_index_3=log_index_3+1;
        elseif abs(ACFf_log(i,10)) == 5
            ACFf_log_5(log_index_5,:)=ACFf_log(i,:);
            sequence_log_5(log_index_5,:)=sequence_log(i,:);
            log_index_5=log_index_5+1;
        else 
            ACFf_log_other(log_index_other,:)=ACFf_log(i,:);
            sequence_log_other(log_index_other,:)=sequence_log(i,:);
            log_index_other=log_index_other+1;
        end
    end
    disp('start...');
    %============== 配對 ========================%
    pair_sequence1 = pair_search(sequence_log_1(1:log_index_1-1,:),ACFf_log_1(1:log_index_1-1,:),log_index_1-1);
    pair_sequence3 = pair_search(sequence_log_3(1:log_index_3-1,:),ACFf_log_3(1:log_index_3-1,:),log_index_3-1);
    pair_sequence5 = pair_search(sequence_log_5(1:log_index_5-1,:),ACFf_log_5(1:log_index_5-1,:),log_index_5-1);
    pair_sequenceother = pair_search(sequence_log_other(1:log_index_other-1,:),ACFf_log_other(1:log_index_other-1,:),log_index_other-1);
    
    filename_p = 'sequence_data.csv';   %存找到的pair
    writematrix(pair_sequence1,filename_p,'WriteMode','append');
    writematrix(pair_sequence3,filename_p,'WriteMode','append');
    writematrix(pair_sequence5,filename_p,'WriteMode','append');
    writematrix(pair_sequenceother,filename_p,'WriteMode','append');
    % load chirp %提醒聲
    % sound(y,Fs);
    % disp('finish...');

end

% load chirp %提醒聲
% sound(y,Fs);
 disp('finish...');

function ACFf = xcorr_2(c1,u,L)
    ACFf = zeros(1,u);
    for i=1:u
        c2=(c1(i+1:L) == c1(1:L-i));
        ones = sum(c2);
        negs = L-i-ones;
        ACFf(i) = ones-negs;
    end
end

function pair_sequence = pair_search(sequence_log_1,ACFf_log,index_1)
    index_2=1;
    pair_sequence=zeros(1000,43);
    for i=1:index_1-1   
        for j=i+1:index_1

            % 優先判斷corrrelation of u=19~2 (逆向判斷）
            continue_j_loop = false;  % 初始化
            for k=1:18
                if ACFf_log(i,20-k) + ACFf_log(j,20-k) ~= 0
                    continue_j_loop = true;
                    break;
                end
            end

            if continue_j_loop  %前u=19~2只要有不符合相加是0
                continue; % 跳到下一個j的迴圈
            end

            zone = 20 ;
            if ACFf_log(i, 20)+ ACFf_log(j, 20) == 0
                zone=21;
                if ACFf_log(i, 21)+ ACFf_log(j, 21) == 0
                    zone=22;
                end
            end

            pair_sequence(index_2,:) = sequence_log_1(i,:);
            pair_sequence(index_2+1,:) = sequence_log_1(j,:);
            disp(['p1=[',num2str(pair_sequence(index_2,:)),']']);
            disp(['p2=[',num2str(pair_sequence(index_2+1,:)),']']);
            disp(['zone: ',num2str(zone)]);
            index_2 = index_2 + 2;          
            if zone>=22
                load gong
                sound(y,Fs);
            end
            
        end
    end
    pair_sequence=pair_sequence(1:index_2-2,:);
end