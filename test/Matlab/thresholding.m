function [im] = thresholding(inImg, threshold1, threshold2, otsu)

[~,~,d] = size(inImg);


if d == 1
    workImg = inImg;    
else
    workImg = rgb2gray(inImg);
end

if otsu
    threshold1 = graythresh(workImg) * 255;
end

disp(threshold1);

im = (workImg > threshold1 )& (workImg < threshold2); 
im = uint8(im * 255);

figure;

subplot(1, 3, 1); imshow(inImg); title('originale');
subplot(1, 3, 2); imshow(workImg); title('Niveaux de gris');
subplot(1, 3, 3); imshow(im); title('Binaire');
