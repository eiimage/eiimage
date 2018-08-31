function [outputImg] = Filter(img, filter, option, doubleResult)
%FILTER Summary of this function goes here
%   X Input array values outside the bounds of the array are implicitly assumed to have the value X. When no boundary option is specified, the default is 0.
%   'symmetric'  Input array values outside the bounds of the array are computed by mirror-reflecting the array across the array border.
%   'replicate' Input array values outside the bounds of the array are assumed to equal the nearest array border value.
%   'circular' Input array values outside the bounds of the array are computed by implicitly assuming the input array is periodic.
if isstring(filter)
    filter = char(filter);
end


if  ischar(filter)
    if strcmp(filter, 'uniforme')
        newfilter = [ 1, 1, 1; 1, 1, 1; 1, 1, 1];  
    elseif strcmp(filter,'gaussien')
        newfilter = [ 1, 2, 1; 2, 3, 2; 1, 2, 1];
    elseif strcmp(filter,'SquareLaplacian')
        newfilter = [ 0, 1, 0; 1, -4, 1; 0, 1, 0];
    end
else 
    newfilter = filter;
end 

disp(newfilter)

[~,~,d] = size(img);

for c = 1:d  
    workImg(:,:,c) = imfilter(double(img(:,:,c)), double(newfilter), option);
end

if not(doubleResult)
    outputImg = uint8(workImg);
else
    outputImg = workImg;
end

