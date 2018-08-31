function [invFourier] = InverseFourier(Fourier)
%INVERSEFOURIER Summary of this function goes here
%   Detailed explanation goes here
invFourier = ifft2(Fourier);

invFourier = uint8(invFourier);
end

