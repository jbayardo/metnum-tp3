files = dir('*.tiff');
for file = files'
    file.name
    ImgToGray(file.name, strcat(file.name, '.csv'))
end