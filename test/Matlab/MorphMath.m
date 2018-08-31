function [outimg] = MorphMath(img, op)
%MORPHMATH Summary of this function goes here
%   Detailed explanation goes here
%se = strel('diamond', 1);
nhood = [0,1,0; 0,1,1 ;0,1,0]
se = strel(nhood);

if isstring(op)
    op= char(op);
end



if strcmpi(op, 'Erosion')
    outimg = imerode(img, se);
    
elseif strcmpi(op, 'Dilatation')
    outimg = imdilate(img, se);
    
elseif strcmpi(op, 'Ouverture')
    outimg = imopen(img, se);
    
elseif strcmpi(op, 'Fermeture')
    outimg = imclose(img, se);
    
elseif strcmpi(op, 'Gradient')
    [outimg, ~] = imgradient(img);
    
elseif strcmpi(op, 'white-top-hat')
    outimg = imtophat(img, se);
    
elseif strcmpi(op, 'black-top-hat')
    outimg = imblackhat(img, se);
    


  
end

