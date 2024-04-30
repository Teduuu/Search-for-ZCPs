clc;
clear;
x=ones(1,42);
%==========================每份要改這裡跟檔名=========================================
%第一組
file=10;
x(1:5)=[-1,1,1,1,-1];
%=============================================================================
count_ones = sum(x(1:5) == 1);
x(6:42-(21-count_ones))=-1;
top=nchoosek(37,21-count_ones);
count=0;
data=zeros(10000000,43);
k=0;
while count<=top
    count=count+1;
    x=JUMP(x); %每個x都可以讓u=1是0
    %檢查一的數量要在18~25之內
    num_one=Num_one(x);
    if num_one>28||num_one<15
        continue;
    end
    % 檢查u=2 限制正負1
    u2_x = x(1:41) .* x(2:42);
    if abs(sum(u2_x))~=1
        continue;
    end
    %檢查u=3 corr要是0
    u3_x = x(1:40) .* x(2:41) .* x(3:42);
    if abs(sum(u3_x))~=0
        continue;
    end
    % 检查u=4 限制正負1
    u4_x = x(1:39) .* x(2:40) .* x(3:41) .* x(4:42);
    if abs(sum(u4_x)) ~=1
        continue;
    end
    % 检查u=5 corr要是0
    u5_x = x(1:38) .* x(2:39) .* x(3:40) .* x(4:41) .* x(5:42);
    if abs(sum(u5_x))~=0
        continue;
    end
    % 检查u=6 限制正負3以內
    u6_x = x(1:37) .* x(2:38) .* x(3:39) .* x(4:40) .* x(5:41) .* x(6:42);
    if abs(sum(u6_x))>3
        continue;
    end
    % 检查u=7 限制正負4以內
    u7_x = x(1:36) .* x(2:37) .* x(3:38) .* x(4:39) .* x(5:40) .* x(6:41) .* x(7:42);
    if abs(sum(u7_x))>4
        continue;
    end
    % 检查u=8 限制正負5以內
    u8_x = x(1:35) .* x(2:36) .* x(3:37) .* x(4:38) .* x(5:39) .* x(6:40) .* x(7:41) .* x(8:42);
    if abs(sum(u8_x))>5
        continue;
    end
    % 检查u=9 限制正負8以內
    u9_x = x(1:34) .* x(2:35) .* x(3:36) .* x(4:37) .* x(5:38) .* x(6:39) .* x(7:40) .* x(8:41) .* x(9:42);
    if abs(sum(u9_x))>8
        continue;
    else
        k=k+1;
        c=Generate_c(-1,x);
        c=(c+1)/2;
        data(k,:)=c;
    end
end


%建立csv
data=data(1:k,:);
csvFileName = ['x_search_' num2str(file) '.csv'];
dlmwrite(csvFileName, data, '-append', 'delimiter', ',');
load chirp %提醒聲
sound(y,Fs);


function c=Generate_c(c0,x)
% 利用(c0, x)組成完整的c序列，only for L=43, u=1
    c=zeros(1,43);
    c(1)=c0;
    for i=1:42
        c(i+1)=x(i)/c(i);
    end
end

function num_one = Num_one(x)
    % 計算序列中 1 的數量（假設序列一開始是-1，mapping到0）
    % 找到所有變號的位置
    sign_changes = (x == -1);
    % 計算累積變號次數
    odd_even_convert = cumsum(sign_changes);
    % 找到奇數變號次數的位置，這表示變號次數為奇數
    odd_positions = (mod(odd_even_convert, 2) == 1);
    % 計算奇數變號位置對應的 1 的數量
    num_one = sum(odd_positions);
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

function lastZeroIndex=Undercount(x,n)
    % 找到最后一个-1的位置
    lastZeroIndex = find(x == -1, 1, 'last');
    % 循环找到倒数第n个-1的位置
    for i = 1:n-1
        lastZeroIndex = find(x(1:lastZeroIndex-1) == -1, 1, 'last');
        if isempty(lastZeroIndex)
            % 处理未找到的情况
            break;
        end
    end
end

%檢查x的倒數位置有連續幾個-1
function numConsecutiveZeros=NumConsecutiveZeros(x)
    % 找到最后一个非零元素的位置
    lastNonZeroIndex = find(x ~= -1, 1, 'last');
    % 计算最后连续的-1的个数
    numConsecutiveZeros = length(x) - lastNonZeroIndex;
end