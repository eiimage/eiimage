function [outimage] = GaussianPyramid(img, level, filter)
%GAUSSIANPYRAMID Summary of this function goes here
%   Detailed explanation goes here
if isstring(filter)
    filter = char(filter);
end


if  ischar(filter)
    if strcmpi(filter, 'triangulaire')
        workfilter =  [0.0, 0.25, 0.5, 0.25, 0.0];
    elseif strcmpi(filter,'gaussien')
        workfilter = [0.05, 0.25, 0.4, 0.25, 0.05];
    elseif strcmpi(filter,'trimodal')
        workfilter = [ -0.05, 0.25 , 0.6, 0.25, -0.05];
    elseif strcmpi(filter,'rectangulaire')
        workfilter = [0.11, 0.11, 0.11, 0.11, 0.11, 0.11, 0.11, 0.11, 0.11];
    elseif strcmpi(filter,'qmf')
        workfilter = [0.01995, -0.04271, -0.05224, 0.29271, 0.56458, 0.29271, -0.05224, -0.04271, 0.01995];
    end
else 
    workfilter = filter;
end 

gaussPyramid = vision.Pyramid;
gaussPyramid.SeparableFilter = 'custom';
gaussPyramid.CustomSeparableFilter = workfilter;


J = img;
for i = 1:level
    tmp = J;
    J = step(gaussPyramid,tmp);
    release(gaussPyramid);
end

outimage = uint8(J);






end

