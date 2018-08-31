function [outputImg] = HistogramEqualize(Img)

[~,~,d] = size(Img);

for c = 1:d
    outputImg(:,:,c) = histeq(Img(:,:,c));
end
    outputImg=uint8(outputImg);
end

