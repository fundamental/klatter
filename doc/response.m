%    sn    = sin(2*pi*f);
%    cs    = cos(2*pi*f);
%    alpha = sn*sinh(log(2)*bw*2*pi*f/(2*sn));
%    a     = [1 2*cs/(1+alpha) (1-alpha)/(1+alpha)];
%    b     = [1 0 -1]*alpha/(1+alpha);

Fs = 8000
T  = 1/Fs
bw = 200

for F=linspace(1,Fs/2,64)

    C = exp(-2*pi*bw*T);
    B = -2*exp(-2*pi*bw*T)*cos(2*pi*F*T);
    A = 1+B+C;

    %[H,W] = freqz(1,[1 B C]);
    [H,W] = freqz(1,[1 B C]);
    plot(W,H);
    drawnow
    sleep(0.1)
end
