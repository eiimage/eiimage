function [outputimg]  = hadamardTransform(img, step)
%HADAMARDTRANSFORM Summary of this function goes here
%   Detailed explanation goes here
 [w,h,d] = size(img);
 
 H = hadamard(step); 
 
    addcol = 0;
    addrow = 0;
     if not (mod(w, step) == 0)
        addcol = step - mod(w, step);
     end
     if not (mod(h, step) == 0)
        addrow = step - mod(h, step); 
     end
     
    
    outputimg = zeros([w+ addcol,h+ addrow,d]);
    for c = 1:d
        for x = 1:w+addcol
            for y = 1:h+addrow
                 outputimg(x,y,c) = img(min([x,w]),min([y,h]),c);
            end
        end
    end
    
    for c = 1:d
        for x = 1:step:w+addcol
            for y = 1:step:h+addrow
                   outputimg(x:x+step-1,y:y+step-1,c) =  H * outputimg(x:x+step-1,y:y+step-1,c) * H /8;
            end
        end
    end
end