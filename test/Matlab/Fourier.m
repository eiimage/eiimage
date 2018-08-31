function [direct ,realF, imagF, absF, angleF] = Fourier(img)

F   = fft2(double(img));

realF = real(F);
imagF = imag(F);
absF = abs(F);
angleF = angle(F);


figure;
subplot(1,3,1);imshow(img);title('original');
subplot(2,3,2);imshow(realF);title('real');
subplot(2,3,3);imshow(imagF);title('imaginay');
subplot(2,3,5);imshow(absF); title('abs');
subplot(2,3,6);imshow(angleF);title('phase');

direct = F;

end

