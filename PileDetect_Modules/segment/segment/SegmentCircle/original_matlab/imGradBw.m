function bw=imGradBw(img,radius)

img=double(img);
h=fspecial('average',5);
img=imfilter(img,h);

[fx,fy]=gradient(img);
gradgraph=sqrt(fx.^2+fy.^2);

level=otsuGray(gradgraph); 
bw=imbinarize(gradgraph,level);    

se=strel('disk',5);
bw=imopen(bw,se);

area=fix(pi*radius*radius/3);

bw=imfill(bw,4,'holes');
% bw=imfill(bw,4);
bw=bwareaopen(bw,area);

