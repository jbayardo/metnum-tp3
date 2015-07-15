function [ matrix img ] = ReadImgMatrix(input)
    matrix = csvread(input);
    img = mat2gray(matrix);
end

