files = dir('*.csv');
for file = files'
    [M I] = ReadImgMatrix(file.name);
    output = strcat(file.name, '.png');
    imwrite(I, output); 
end