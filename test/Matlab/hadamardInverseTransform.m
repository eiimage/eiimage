function [outputimg]  = hadamardInverseTransform(img, step)

    [w,h,d] = size(img);

    H = inv(hadamard(step)); 
    
    for c = 1:d
        for x = 1:step:w
            for y = 1:step:h
                   outputimg(x:x+step-1,y:y+step-1,c) =  H * img(x:x+step-1,y:y+step-1,c) * H / 8;
            end
        end
    end
end

