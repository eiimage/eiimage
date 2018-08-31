function [output] = CombineRGB(imgR, imgV, imgB)

output = uint8(cat(3, imgR, imgV, imgB));

imshow(output);