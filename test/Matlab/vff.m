function [im] = vff(pathToFile)
%   
% This program can be used to open VFF image file in Matlab.
%   
%   Shanrong Zhang, 12/17/2004
%   Department of Radiology, 
%   University of Washington
%   zhangs@u.washington.edu
% 
%   Modified on 06/07/2018
%       - can now give the path of the file in parameter 
%       - and it return the right image in uint8
%
%


if not(nargin == 1)
    [filename, pathname] = uigetfile('*.vff','Please select a fdf file');
    pathToFile = [pathname filename];
end

fid = fopen(pathToFile,'r','b');
disp(pathToFile)

num = 0;
done = false;

line = fgetl(fid);
disp(line);
while (~isempty(line) && ~done)
    line = fgetl(fid);
    disp(line);
    if strmatch('size=', line)
        [token, rem] = strtok(line,'size= ;');
        M(1) = str2num(token);
        M(2) = str2num(strtok(rem,' ;'));
    end
    
    if strmatch('bits=', line)
        bits = str2num(strtok(line,'bits=;'));
    end
    
    num = num + 1;
    
    if num >= 14
        done = true;
    end
end
status = fseek(fid, -M(1)*M(2)*bits/8,'eof');
im = fread(fid,[M(1),M(2)], 'uint8');
im = uint8(transpose(im));

fclose(fid);

% end of code
