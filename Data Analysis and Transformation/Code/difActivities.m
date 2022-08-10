function difActivities(signal, exp, labels)
T = (1/50);
time = (0:T:T*length(signal)-T);
time = time.';
signal = [signal time];
totalx = [];
totaly = [];
totalz = [];
array = [];
cont = 1;
i = 1;

for act=1:12
    while labels(i,1) ~= exp
        i = i + 1;
    end
    while labels(i, 1) == exp
        while labels(i,3) ~= act
            i = i + 1;
        end
        if(labels(i,1) == exp)
            inicial_time = (labels(i,4)/50);
            final_time = (labels(i,5)/50);
            rows = 1;
            signalDFT = [];
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
            pico = max(pks);
            totalx = [totalx pico./2];
            
            hamming_win = hamming(numel(signalDFT(:,2)));
            dft =  abs(fftshift(fft(detrend(signalDFT(:,2)).*hamming_win)));
            [pks,~] = findpeaks(dft);
            pico = max(pks);
            totaly = [totaly pico./2];
            
            hamming_win = hamming(numel(signalDFT(:,3)));
            dft =  abs(fftshift(fft(detrend(signalDFT(:,3)).*hamming_win)));
            [pks,~] = findpeaks(dft);
            pico = max(pks);
            totalz = [totalz pico./2];
        end
        i = i + 1;
    end
    maximo = [max(totalx) max(totaly) max(totalz)];
    if numel(maximo) == 0
        maximo = [0 0 0];
    end
    array(cont,1) = maximo(:,1);
    array(cont,2) = maximo(:,2);
    array(cont,3) = maximo(:,3);
    totalx = [];
    totaly = [];
    totalz = [];
    i = 1;
    cont = cont + 1;
    
end
for x=1:12
    if(x <= 3)
        scatter3(array(x,1), array(x,2), array(x,3),'*r');
        hold on
    end
    if(x > 3 && x <= 6)
        scatter3(array(x,1), array(x,2), array(x,3),'og');
        hold on
    end
    if(x > 6)
        scatter3(array(x,1), array(x,2), array(x,3),'+b');
        hold on
    end
end
end
