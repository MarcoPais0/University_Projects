function calculateDFT_Detrend(signal, i)
    N = numel(signal);
    fs = 50;
    Ts = (1/fs);
    hamming_win = hamming(numel(signal));
    signalDFT =  abs(fftshift(fft(detrend(signal).*hamming_win)));
    if (mod(N,2)==0)
        f = -fs/2:fs/N:fs/2-fs/N;
    else 
        f = -fs/2+fs/(2*N):fs/N:fs/2-fs/(2*N);
    end
    subplot(i);
    plot(f, signalDFT);
    title('DFT Hamming');
    xlabel('f(Hz)');
    ylabel('Magnitude');
    
    %{
    black_win = blackman(numel(signal));
    signalDFT =  abs(fftshift(fft(detrend(signal).*black_win)));
    subplot(223);
    plot(f, signalDFT);
    title('DFT Blackman');
    xlabel('f(Hz)');
    ylabel('Magnitude');
    
    hann_win = hann(numel(signal));
    signalDFT =  abs(fftshift(fft(detrend(signal).*hann_win)));
    subplot(224);
    plot(f, signalDFT);
    title('DFT Hann');
    xlabel('f(Hz)');
    ylabel('Magnitude');
    %}
end

