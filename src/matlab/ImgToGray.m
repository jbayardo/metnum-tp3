function ImgToGray(input,output)
    RGB = imread(input);
    I = rgb2gray(RGB);
    csvwrite(output, I);
    imwrite(I,strcat('gray_',input));
end
