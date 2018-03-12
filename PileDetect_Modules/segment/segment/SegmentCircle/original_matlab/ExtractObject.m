function [xCoords,yCoords]=ExtractObject(bw,rx,ry,radius,ratio)
[yCoords,xCoords]=find(bw);
dist2=(xCoords-rx).^2+(yCoords-ry).^2;
index=(dist2-(ratio*radius)^2)<0;

xCoords=xCoords(index);
yCoords=yCoords(index);
