function [signalDFT] = plotDFTs(labels, activity_id, signal, exp, activity_label)
T = (1/50)/60;
time = (0:T:T*length(signal)-T);
time = time.';
signal = [signal time];
i = 1;
while labels(i, 1) ~= exp || labels(i,3) ~= activity_id
    i = i + 1;
end
inicial_time = (labels(i,4)/50)/60;
final_time = (labels(i,5)/50)/60;
rows = 1;
signalDFT = zeros;
for x=1:length(signal)
    if signal(x,4) >= inicial_time && signal(x,4) <= final_time
        signalDFT(rows,1) = signal(x,1);
        signalDFT(rows,2) = signal(x,2);
        signalDFT(rows,3) = signal(x,3);
        signalDFT(rows,4) = signal(x,4);
        rows = rows + 1;
    end
end
figure(1);
subplot(231);
plot(signalDFT(:,4), signalDFT(:,1));
tit = sprintf('Sinal Original (X) - Exp %d - %s' , exp, activity_label);
title(tit);
xlabel('t(s)');
ylabel('Amplitude');
calculateDFT_Detrend(signalDFT(:,1), 234);

subplot(232);
plot(signalDFT(:,4), signalDFT(:,2));
tit = sprintf('Sinal Original (Y) - Exp %d - %s' , exp, activity_label);
title(tit);
xlabel('t(s)');
ylabel('Amplitude');
calculateDFT_Detrend(signalDFT(:,2),235);

subplot(233);
plot(signalDFT(:,4), signalDFT(:,3));
tit = sprintf('Sinal Original (Z) - Exp %d - %s' , exp, activity_label);
title(tit);
xlabel('t(s)');
ylabel('Amplitude');
calculateDFT_Detrend(signalDFT(:,3),236);
end

