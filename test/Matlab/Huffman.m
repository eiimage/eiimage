function [output] = Huffman(inImg)
%HUFFMAN Summary of this function goes here
%   Detailed explanation goes here
workImg = rgb2gray(inImg);

minV = min(workImg(:));
maxV = max(workImg(:));

symbols = minV:1:maxV;
counts = imhist(workImg(:));

p = double(counts(minV:maxV)) ./ sum(counts(minV:maxV));
[dict , avglen] = huffmandict(symbols,p) ;


disp(cellfun(@num2str, dict(:, 2), 'UniformOutput', false));
disp(avglen);
end

