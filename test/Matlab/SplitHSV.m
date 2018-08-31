function [H,S,V] = SplitHSV(img)
%SPLITHSV Summary of this function goes here
%   Detailed explanation goes here
HSV = rgb2hsv(img);
H = HSV(:,:,1);
S = HSV(:,:,2);
V = HSV(:,:,3);
end

