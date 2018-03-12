function bw=imBw(img,radius)
img=im2double(uint8(img));
h=fspecial('average',5);
img=imfilter(img,h);

level=otsuGray(img); 
bw=imbinarize(img,level);    

se=strel('disk',5);
bw=imopen(bw,se);

area=fix(pi*radius*radius/3);
bw=imfill(bw,4,'holes');

bw=bwareaopen(bw,area);
