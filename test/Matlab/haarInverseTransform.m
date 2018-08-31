function [outputimg]  = haarInverseTransform(img, step)

    [w,h,d] = size(img);
    outputimg = zeros(size(img));

    H = inv(generate_haar(step)); 
    
    for c = 1:d
        for x = 1:step:w
            for y = 1:step:h
                   outputimg(x:x+step-1,y:y+step-1,c) =  H * double(img(x:x+step-1,y:y+step-1,c)) * H;
            end
        end
    end
    outputimg = uint8(outputimg);
end

