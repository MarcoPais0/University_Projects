function [] = calculateSTFT(signal)
spectre = [];
fs = 50;
N = numel(signal);
Ts = 1/fs;
t = N*Ts;
time_frame = 0.005*t;
time_overlap = time_frame/2;
overlap = round(time_overlap*fs);
frame = round(time_frame*fs);
h = hamming(frame);
f = linspace(-fs/2,fs/2,frame);
x =  find(f >= 0);
for i = 1:frame-overlap:N-frame
    x_frame = signal(i:i+frame-1).*h;
    X_frame = abs(fftshift(fft(x_frame)));
    spectre = horzcat(spectre,X_frame(x));
end
display(numel(spectre))
imagesc(10*log10(spectre))
set(gca,'YDir','normal')
xlabel('t [s]')
ylabel('f [Hz]')
title('Short-Time Fourier Transform')
colorbar
end