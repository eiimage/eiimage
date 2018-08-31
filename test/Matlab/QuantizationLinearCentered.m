function [im, allvalues, alllevels] = QuantizationLinearCentered(inImg, nb_values)

im = zeros(size(inImg));

nb_tresh = nb_values -1;

values = zeros(nb_values,1);
levels = zeros(nb_tresh,1);

allvalues = zeros(nb_values,3);
alllevels = zeros(nb_tresh,3);
for c = 1:3
%linear centered values 

    for i = 1:nb_tresh 
        levels(i) = floor( i * 255 / nb_values + 0.5);
    end

    values(1) = levels(1)/2;
    for i = 2:nb_values-1
        values(i) = (levels(i-1)+ levels (i) )/2;
    end
    values(nb_values) = (255 + levels(nb_tresh))/2;

    im(:,:,c) = imquantize(inImg(:,:,c), levels(:), values);
    
    allvalues(:,c) = values(:);
    alllevels(:,c) = levels(:);
    
end

im = uint8(im);

imshow(im);





    