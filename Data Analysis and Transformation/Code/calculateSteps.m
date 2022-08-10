function [media_sec_x, media_sec_y, media_sec_z] = calculateSteps(signal, exp, activity_id, labels)
T = (1/50);
time = (0:T:T*length(signal)-T);
time = time.';
signal = [signal time];
totalx = [];
totaly = [];
totalz = [];
i = 1;
while labels(i,1) ~= exp
    i = i + 1;
end
while labels(i, 1) == exp
    while labels(i,3) ~= activity_id
        i = i + 1;
    end
    if(labels(i,1) == exp)
        inicial_time = (labels(i,4)/50);
        final_time = (labels(i,5)/50);
        rows = 1;
        signalDFT = zeros;
        for x=1:length(signal)
            if signal(x,4) >= inicial_time && signal(x,4) <= final_time
                signalDFT(rows,1) = signal(x,1);
                signalDFT(rows,2) = signal(x,2);
                signalDFT(rows,3) = signal(x,3);
                rows = rows + 1;
            end
        end
        
        hamming_win = hamming(numel(signalDFT(:,1)));
        dft =  abs(fftshift(fft(detrend(signalDFT(:,1)).*hamming_win)));
        [pks,~] = findpeaks(dft);
        passos = numel(pks);
        totalx = [totalx passos./2];
        
        hamming_win = hamming(numel(signalDFT(:,2)));
        dft =  abs(fftshift(fft(detrend(signalDFT(:,2)).*hamming_win)));
        [pks,~] = findpeaks(dft);
        passos = numel(pks);
        totaly = [totaly passos./2];
        
        hamming_win = hamming(numel(signalDFT(:,3)));
        dft =  abs(fftshift(fft(detrend(signalDFT(:,3)).*hamming_win)));
        [pks,~] = findpeaks(dft);
        passos = numel(pks);
        totalz = [totalz passos./2];
        
        i = i + 1;
    end
end

media = mean(totalx);
desvio_padrao = std(totalx);
media_sec_x = [num2str(media, '%.2f') ' +- ' num2str(desvio_padrao, '%.2f')];

media = mean(totaly);
desvio_padrao = std(totaly);
media_sec_y = [num2str(media, '%.2f') ' +- ' num2str(desvio_padrao, '%.2f')];

media = mean(totalz);
desvio_padrao = std(totalz);
media_sec_z = [num2str(media, '%.2f') ' +- ' num2str(desvio_padrao, '%.2f')];
end