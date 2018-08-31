function [outputImg] = QuantizationLloydMax(inputImg, nb_values)
%LLOYDMAX Summary of this function goes here
%   Detailed explanation goes here

    [w,h,d] = size(inputImg);
    
    for c = 1:d
        workImg = inputImg(1,:,c);
        for i = 2:w
            workImg = cat(2, workImg, inputImg(i,:,c));
        end
        [LloydMaxLevels, LloydMaxValues] = lloyds(double(workImg), nb_values);
        disp(LloydMaxLevels);
        disp(LloydMaxValues);
        outputImg(:,:,c) = imquantize(inputImg(:,:,c), LloydMaxLevels, LloydMaxValues);
    end
    
    outputImg = uint8(outputImg);
   

end

