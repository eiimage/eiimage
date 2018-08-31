function [im] = QuantizationNonLinearCentered(inImg, nb_values)


im = zeros(size(inImg));
nb_tresh = nb_values -1;

values = zeros(nb_values,1);
levels = zeros(nb_tresh,1);

[h,w,d] = size(inImg);
imageSize = h * w;


for c = 1:d
% non linear centered values 
[histogram , ~] = imhist(inImg(:,:,c));

histogramSum = 0 ; 
value  = 1;

for i = 1:nb_tresh 
    percent = i  / nb_values;
    while(( percent * imageSize > histogramSum ) && ( value <= 255))
         histogramSum = histogramSum +  histogram(value);
         value = value + 1;
       
    end 
    levels(i) = value - 1;
end 


values(1) = levels(1)/2;
for i = 2:nb_values-1
    values(i) = (levels(i-1)+ levels (i) )/2;
end
values(nb_values) = (255 + levels(nb_tresh))/2;

    im(:,:,c) = imquantize(inImg(:,:,c), levels(:), values);
end

im = uint8(im);

imshow(im);