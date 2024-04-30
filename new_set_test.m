clear;
n=8;
max_bound=8; % gcp max_bound = n

zmax=25;

ifrun = 40; % 找到的次數

temp_PAPR_p = 10 ;
temp_PAPR_q = 10 ;
temp_max_p = [];
temp_max_q = [];
temp_sum=[];
while ifrun >=0
    [z,p1,p2] = find_ZCP(n);
    if z >= max_bound
        [f1,f2] = turyn(p1,p2);  % 2N
        [ifrun,temp_max_p,temp_max_q,temp_PAPR_p,temp_PAPR_q,temp_sum] = newset(f1,f2,zmax,ifrun,temp_max_p,temp_max_q,temp_PAPR_p,temp_PAPR_q,temp_sum); %2N+2     
    end
 
    if ifrun==0
        for i = 1:length(temp_max_p)
            if temp_max_p(i) ==-1
                temp_max_p(i) = 0;
            end
        end
        for i = 1:length(temp_max_q)
            if temp_max_q(i) ==-1
                temp_max_q(i) = 0;
            end
        end
        disp(['p: ',num2str(temp_max_p)]);
        disp(['q: ',num2str(temp_max_q)]);
        disp(['autocorrelation: ',num2str(temp_sum)]);
        disp(['L: ',num2str(length(temp_max_p))]);
        zone_sum=check_zone(temp_sum);
        disp(['Zone: ',num2str(zone_sum)]);
        disp(['PAPR_p : ',num2str(temp_PAPR_p)]);
        disp(['PAPR_q: ',num2str(temp_PAPR_q)]);
        ifrun=ifrun-1;
    end
end

function [z,p1,p2] = find_ZCP(n)
    p1=[];
    p2=[];
%================  find GCP  =====================%
    for i = 1:n
        p1(i) = 2*round(rand(1)) - 1;
        p2(i) = 2*round(rand(1)) - 1;
    end
    y1 = xcorr(p1);
    ans1 = y1(n:-1:1);
    y2 = xcorr(p2);
    ans2 = y2(n:-1:1);
    ans3 = ans1+ans2;
    ans3 = round(ans3);
    z = check_zone(ans3);
end

function [f1,f2] = turyn(a,b)
    %GCP
    c = [1 1 ];%A
    d = [1 -1 ];%A
    c_flip = flip(c);
    d_flip = flip(d);
    L = length(a)*length(c); 
    f1 = [];
    f2 = [];   
    for i=1:length(c)
        for j=1:length(a)
           temp = c(i)*( a(j) + b(j) )/2 - d_flip(i)*( b(j) - a(j) )/2;
           f1 = [f1,temp];
           temp = d(i)*( a(j) + b(j) )/2 + c_flip(i)*( b(j) - a(j) )/2;
           f2 = [f2,temp];
        end
    end
    %disp(['f1: ',num2str(f1)]);
    %disp(['f2: ',num2str(f2)]);
    %f1_auto = xcorr(f1);
    %f2_auto = xcorr(f2);
    %sum = f1_auto(L:-1:1)+f2_auto(L:-1:1);
    %sum = round(sum);
    %zone = check_zone(sum);
    %disp(['autocorrelation:',num2str(sum)]);
    %disp(['L:',num2str(L)]);
    %disp(['Zone:',num2str(zone)]);
end

function [ifrun,temp_max_p,temp_max_q,temp_PAPR_p,temp_PAPR_q,temp_sum] = newset(a,b,zmax,ifrun,temp_max_p,temp_max_q,temp_PAPR_p,temp_PAPR_q,temp_sum)
    c = flip(b);
    d = -flip(a);
    p=[-1,a,c,-1];
    q=[1,b,d,-1];
    L = length(p);
    p_auto = xcorr(p);
    q_auto = xcorr(q);
    sum = p_auto(L:-1:1)+q_auto(L:-1:1);
    sum = round(sum);
    zone = check_zone(sum);
    if zone >= zmax
        L =length(p);
        X = linspace(0,1,10000);
        Y_1 = PAPR(p,X)/L;
        Y_2 = PAPR(q,X)/L;
        [argvalue_1, argmax_1] = max(Y_1);
        [argvalue_2, argmax_2] = max(Y_2);
        if argvalue_1 + argvalue_2 <= temp_PAPR_p + temp_PAPR_q
            temp_max_p = p;
            temp_max_q = q;
            temp_sum = sum;
            temp_PAPR_p = argvalue_1 ;
            temp_PAPR_q = argvalue_2 ;
        end
        ifrun = ifrun-1;
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

