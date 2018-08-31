function [outputImg] = hough2(inImg, rhoStep, thetaStep)
%HOUGH Summary of this function goes here
%   Detailed explanation goes here
[outputImg, ~,~] = hough(inImg, 'RhoResolution',rhoStep,'Theta',-90:thetaStep:90-thetaStep); 


outputImg = transpose(outputImg);
end

