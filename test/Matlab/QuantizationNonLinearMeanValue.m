function [outputImg] = QuantizationNonLinearMeanValue(inputImg, nb_values)
%QUANTIZATIONNONLINEARMEANVALUE Perform a Non linear quantization with mean
%value
outputImg = zeros(size(inputImg));
nb_thresh = nb_values -1;

values = zeros(nb_values,1);
levels = zeros(nb_thresh,1);

[h,w,d] = size(inputImg);
imageSize = h * w;

N_MAX_THRESHOLD  = 255;


for c = 1:d
    % non linear centered values 
    [histogram , ~] = imhist(inputImg(:,:,c));

    histogramSum = 0 ; 
    value  = 1;

    for i = 1:nb_thresh 
        percent = i  / nb_values;
        while(( percent * imageSize > histogramSum ) && ( value <= 255))
             histogramSum = histogramSum +  histogram(value);
             value = value + 1;

        end 
        levels(i) = value - 1;
    end 


    
    
    som_lum = 0;
    nb_points = 0;
    for j = 1 : levels(1)
            som_lum = som_lum + histogram(j) * j;
            nb_points = nb_points + histogram(j);
    end

    if(nb_points > 0) 
        values(1)= som_lum/nb_points;
    else
        values(1) =  levels(0) / 2;
    end

    for j = 2 : nb_thresh
        som_lum = 0;
        nb_points = 0;
        for i = levels(j-1) : levels(j)
               som_lum = som_lum + histogram(i)*i;          
               nb_points = nb_points + histogram(i);
        end      
        if nb_points > 0 
            values(j)= som_lum/nb_points;
        else
            values(j) = (levels(j)+levels(j+1))/2
        end


    end
    som_lum = 0;
    nb_points = 0;
    for j = levels(nb_thresh) : N_MAX_THRESHOLD
           som_lum = som_lum + histogram(j) * j;
           nb_points = nb_points + histogram(j);
    end
    
    if(nb_points > 0)
        values(nb_values) = som_lum/nb_points;
    else
        values(nb_values) = (levels(nb_thresh) +  N_MAX_THRESHOLD)/2 ;
    end
    
    outputImg(:,:,c) = imquantize(inputImg(:,:,c), levels(:), values);
end

outputImg = uint8(outputImg);
disp(levels) 
disp(values)

imshow(outputImg);


