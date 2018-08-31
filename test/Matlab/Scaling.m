function [out] = Scaling(img, x, y, interpol )
%SCALING Summary of this function goes here
%   Detailed explanation goes here
[w,h,~] = size(img);

newWidth =  floor(w*x);
newHeight = floor(h*y);

out = imresize(img, [newHeight newWidth ], interpol);

end

