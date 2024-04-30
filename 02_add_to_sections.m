clear;
L = 43;
u = 9;

% Read sequence data 
filename = 'x_search_23.csv';
sequence_log = readmatrix(filename);
index_1 = length(sequence_log);

ACFf_log = zeros(index_1,u);
disp(filename);
disp(['num of sequences: ', num2str(index_1)]);

% Define filenames for section logs
section_filenames = cell(1, 72);
for i = 1:72
    section_filenames{i} = ['sequence_section_' num2str(i) '.csv'];
end

% Initialize section logs and indices
section_logs = cell(1, 72);
index_logs = ones(1, 72);

for i = 1:72
    section_logs{i} = zeros(100000,43);
end

% Calculate ACFf and classify sequences
for i = 1:index_1
    ACFf_log(i, :) = xcorr_2(sequence_log(i, :), u, L);
    section_index = find_section_index(ACFf_log(i, :));
    section_logs{section_index}(index_logs(section_index), :) = sequence_log(i, :);
    index_logs(section_index) = index_logs(section_index) + 1;
end

% Write section logs to files
for i = 1:72
    dlmwrite(section_filenames{i}, section_logs{i}(1:index_logs(i)-1,:), '-append', 'delimiter', ',');
    disp(['number of section' num2str(i) ': ', num2str(index_logs(i) - 1)]);
end

% Reminder sound
load chirp
sound(y, Fs);

function ACFf = xcorr_2(c1, u, L)
    ACFf = [];
    for i = 1:u
        c2 = (c1(i + 1:L) == c1(1:L - i));
        ones = sum(c2);
        negs = L - i - ones;
        ACFf(i) = ones - negs;
    end
end

function section_index = find_section_index(ACFf_row)
    if abs(ACFf_row(6)) == 1
        if ACFf_row(7) == 0
            if abs(ACFf_row(8)) == 1
                if abs(ACFf_row(9)) == 0
                    section_index = 1;
                elseif abs(ACFf_row(9)) == 2
                    section_index = 2;
                elseif abs(ACFf_row(9)) == 4
                    section_index = 3;
                else
                    section_index = 4;
                end
            elseif abs(ACFf_row(8)) == 3
                if abs(ACFf_row(9)) == 0
                    section_index = 5;
                elseif abs(ACFf_row(9)) == 2
                    section_index = 6;
                elseif abs(ACFf_row(9)) == 4
                    section_index = 7;
                else
                    section_index = 8;
                end
            else
                if abs(ACFf_row(9)) == 0
                    section_index = 9;
                elseif abs(ACFf_row(9)) == 2
                    section_index = 10;
                elseif abs(ACFf_row(9)) == 4
                    section_index = 11;
                else
                    section_index = 12;
                end
            end
        elseif abs(ACFf_row(7)) == 2
            if abs(ACFf_row(8)) == 1
                if abs(ACFf_row(9)) == 0
                    section_index = 13;
                elseif abs(ACFf_row(9)) == 2
                    section_index = 14;
                elseif abs(ACFf_row(9)) == 4
                    section_index = 15;
                else
                    section_index = 16;
                end
            elseif abs(ACFf_row(8)) == 3
                if abs(ACFf_row(9)) == 0
                    section_index = 17;
                elseif abs(ACFf_row(9)) == 2
                    section_index = 18;
                elseif abs(ACFf_row(9)) == 4
                    section_index = 19;
                else
                    section_index = 20;
                end
            else
                if abs(ACFf_row(9)) == 0
                    section_index = 21;
                elseif abs(ACFf_row(9)) == 2
                    section_index = 22;
                elseif abs(ACFf_row(9)) == 4
                    section_index = 23;
                else
                    section_index = 24;
                end
            end
        else
            if abs(ACFf_row(8)) == 1
                if abs(ACFf_row(9)) == 0
                    section_index = 25;
                elseif abs(ACFf_row(9)) == 2
                    section_index = 26;
                elseif abs(ACFf_row(9)) == 4
                    section_index = 27;
                else
                    section_index = 28;
                end
            elseif abs(ACFf_row(8)) == 3
                if abs(ACFf_row(9)) == 0
                    section_index = 29;
                elseif abs(ACFf_row(9)) == 2
                    section_index = 30;
                elseif abs(ACFf_row(9)) == 4
                    section_index = 31;
                else
                    section_index = 32;
                end
            else
                if abs(ACFf_row(9)) == 0
                    section_index = 33;
                elseif abs(ACFf_row(9)) == 2
                    section_index = 34;
                elseif abs(ACFf_row(9)) == 4
                    section_index = 35;
                else
                    section_index = 36;
                end
            end
        
        end
    else  %abs(ACFf_row(6)) == 3
        if ACFf_row(7) == 0
            if abs(ACFf_row(8)) == 1
                if abs(ACFf_row(9)) == 0
                    section_index = 37;
                elseif abs(ACFf_row(9)) == 2
                    section_index = 38;
                elseif abs(ACFf_row(9)) == 4
                    section_index = 39;
                else
                    section_index = 40;
                end
            elseif abs(ACFf_row(8)) == 3
                if abs(ACFf_row(9)) == 0
                    section_index = 41;
                elseif abs(ACFf_row(9)) == 2
                    section_index = 42;
                elseif abs(ACFf_row(9)) == 4
                    section_index = 43;
                else
                    section_index = 44;
                end
            else
                if abs(ACFf_row(9)) == 0
                    section_index = 45;
                elseif abs(ACFf_row(9)) == 2
                    section_index = 46;
                elseif abs(ACFf_row(9)) == 4
                    section_index = 47;
                else
                    section_index = 48;
                end
            end
        elseif abs(ACFf_row(7)) == 2
            if abs(ACFf_row(8)) == 1
                if abs(ACFf_row(9)) == 0
                    section_index = 49;
                elseif abs(ACFf_row(9)) == 2
                    section_index = 50;
                elseif abs(ACFf_row(9)) == 4
                    section_index = 51;
                else
                    section_index = 52;
                end
            elseif abs(ACFf_row(8)) == 3
                if abs(ACFf_row(9)) == 0
                    section_index = 53;
                elseif abs(ACFf_row(9)) == 2
                    section_index = 54;
                elseif abs(ACFf_row(9)) == 4
                    section_index = 55;
                else
                    section_index = 56;
                end
            else
                if abs(ACFf_row(9)) == 0
                    section_index = 57;
                elseif abs(ACFf_row(9)) == 2
                    section_index = 58;
                elseif abs(ACFf_row(9)) == 4
                    section_index = 59;
                else
                    section_index = 60;
                end
            end
        else
            if abs(ACFf_row(8)) == 1
                if abs(ACFf_row(9)) == 0
                    section_index = 61;
                elseif abs(ACFf_row(9)) == 2
                    section_index = 62;
                elseif abs(ACFf_row(9)) == 4
                    section_index = 63;
                else
                    section_index = 64;
                end
            elseif abs(ACFf_row(8)) == 3
                if abs(ACFf_row(9)) == 0
                    section_index = 65;
                elseif abs(ACFf_row(9)) == 2
                    section_index = 66;
                elseif abs(ACFf_row(9)) == 4
                    section_index = 67;
                else
                    section_index = 68;
                end
            else
                if abs(ACFf_row(9)) == 0
                    section_index = 69;
                elseif abs(ACFf_row(9)) == 2
                    section_index = 70;
                elseif abs(ACFf_row(9)) == 4
                    section_index = 71;
                else
                    section_index = 72;
                end
            end
        
        end
    end
end