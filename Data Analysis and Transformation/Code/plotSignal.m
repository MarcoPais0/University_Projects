function plotSignal(signal, exp, labels, activity)
T = (1/50)/60;
time = (0:T:T*length(signal)-T);
time = time.';
signal = [signal time];
figure(1);
subplot(311);
plot(signal(:,4),signal(:,1),'-k');
xlabel('Time (min)');
ylabel('ACC_X');
hold on
subplot(312);
plot(signal(:,4),signal(:,2),'-k');
xlabel('Time (min)');
ylabel('ACC_Y');
hold on
subplot(313);
plot(signal(:,4),signal(:,3),'-k');
xlabel('Time (min)');
ylabel('ACC_Z');
hold on
ACC_X = zeros;
ACC_Y = zeros;
ACC_Z = zeros;
rows = 1;
i = 1;
while labels(i,1) ~= exp
    i = i + 1;
end
inicial_time = (labels(i,4)/50)/60;
final_time = (labels(i,5)/50)/60;
for x=1:length(signal)
    if signal(x,4) >= inicial_time && signal(x,4) <= final_time
        ACC_X(rows,1) = signal(x,1);
        ACC_X(rows,2) = signal(x,4);
        ACC_Y(rows,1) = signal(x,2);
        ACC_Y(rows,2) = signal(x,4);
        ACC_Z(rows,1) = signal(x,3);
        ACC_Z(rows,2) = signal(x,4);
        rows = rows + 1;
    end
    
    if signal(x,4) > final_time && labels(i, 1) == exp
        R = double(rand());
        G = double(rand());
        B = double(rand());
        color = [R G B];
        if mod(i, 2) == 0
            y_X = max(signal(:,1))+0.1;
            y_Y = max(signal(:,2))+0.1;
            y_Z = max(signal(:,3))+0.1;
        else
            y_X = min(signal(:,1))-0.1;
            y_Y = min(signal(:,2))-0.1;
            y_Z = min(signal(:,3))-0.1;
        end
        figure(1);
        subplot(311);
        plot(ACC_X(:,2), ACC_X(:,1), 'Color', color);
        text(inicial_time,y_X, activity{1}{labels(i,3)});
        subplot(312);
        plot(ACC_Y(:,2), ACC_Y(:,1), 'Color', color);
        text(inicial_time,y_Y, activity{1}{labels(i,3)});
        subplot(313);
        plot(ACC_Z(:,2), ACC_Z(:,1), 'Color', color);
        text(inicial_time,y_Z, activity{1}{labels(i,3)});
        if i + 1 <= length(labels)
            i = i + 1;
            inicial_time = (labels(i, 4)/50)/60;
            final_time = (labels(i, 5)/50)/60;
            ACC_X = zeros;
            ACC_Y = zeros;
            ACC_Z = zeros;
            rows = 1;
        else
            break;
        end
    end
end
end

