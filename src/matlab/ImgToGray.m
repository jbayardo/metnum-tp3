function ImgToGray(input,output)
    RGB = imread(input);
    [rows columns numberOfColorChannels] = size(RGB);
    I = RGB;
    if numberOfColorChannels > 1
        I = rgb2gray(RGB);
    end
    csvwrite(output, I);
    imwrite(I,strcat('gray_',input));
end
