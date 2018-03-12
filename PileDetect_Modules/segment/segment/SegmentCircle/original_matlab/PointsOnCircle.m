function [xCircle,yCircle]=PointsOnCircle(rx,ry,radius)
xCircle=zeros(4*radius,1);
yCircle=zeros(4*radius,1);
%先求1/4圆
%左侧起始点
xList=-radius:-1;
yList=-round(sqrt(radius^2-xList.^2));

xCircle(1:radius)=xList;
yCircle(1:radius)=yList;
xCircle(radius+1)=0;
yCircle(radius+1)=-radius;

% xList=-List;
xCircle(radius+2:2*radius+1)=-xList(end:-1:1);
yCircle(radius+2:2*radius+1)=yList(end:-1:1);

xCircle(2*radius+2:end)=xCircle(2*radius:-1:2);
yCircle(2*radius+2:end)=-yCircle(2*radius:-1:2);
xCircle=xCircle+rx;
yCircle=yCircle+ry;
