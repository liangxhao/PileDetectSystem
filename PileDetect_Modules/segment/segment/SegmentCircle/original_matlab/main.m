img=imread('1.jpg');

rx=492;
ry=334;
radius=58;
ratio=2;

img=rgb2gray(img);
img=double(img);

[objX,objY,objRadius,flag]=fitCircle(img,rx,ry,radius,ratio);


imshow(uint8(img))
hold on   
theta=0:0.1:2*pi;
x=objX+objRadius.*cos(theta);
y=objY+objRadius.*sin(theta);
plot(x,y,'r');
hold on
% 
plot(objX,objY,'*');