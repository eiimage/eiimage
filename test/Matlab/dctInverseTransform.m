function [outputimg] = dctInverseTransform(img, step)
%DCTINVERSETRANSFORM Summary of this function goes here
%   Detailed explanation goes here

    [w,h,d] = size(img);
    outputimg = zeros(size(img));
    for c = 1:d
        for x = 1:step:w
            for y = 1:step:h
                   outputimg(x:x+step-1,y:y+step-1,c) = idct2(img(x:x+step-1,y:y+step-1,c), [ step step ]);
            end
        end
    end
    outputimg = uint8(outputimg);
end

