function calculateDFT_noDetrend(signal)
    N = numel(signal);
    fs = 50;
    Ts = (1/fs);
    signalDFT = fftshift(fft(signal));
    signalDFT(abs(signalDFT)<0.001) = 0;
    if (mod(N,2)==0)
        f = -fs/2:fs/N:fs/2-fs/N;
    else 
        f = -fs/2+fs/(2*N):fs/N:fs/2-fs/(2*N);
    end
    subplot(312);
    plot(f, abs(signalDFT));
    title('DFT No Detrend');
    xlabel('f(Hz)');
    ylabel('Magnitude');
end

