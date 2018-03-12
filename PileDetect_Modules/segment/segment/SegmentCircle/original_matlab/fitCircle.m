function [objX,objY,objRadius,flag]=fitCircle(img,rx,ry,radius,ratio)
%提取圆形目标并拟合

%flag为0表示拟合失败，返回默认值
objX=rx;
objY=ry;
objRadius=radius;
flag=0;

[rows,cols]=size(img);
bw=imGradBw(img,radius);
%目标坐标
[xCoords,yCoords]=ExtractObject(bw,rx,ry,radius,ratio);

if isempty(xCoords)
    bw=imBw(img,radius);
    [xCoords,yCoords]=ExtractObject(bw,rx,ry,radius,ratio);
    if isempty(xCoords)
        return
    end
end


%目标中心
objX=round(mean(xCoords));
objY=round(mean(yCoords));
%目标二值图
indexObj=sub2ind([rows,cols],yCoords,xCoords);
mask=zeros(rows,cols);
mask(indexObj)=1;

range=floor(radius*0.5):floor(radius*1.5);
len=length(range);
numPixel=zeros(len,1);

for i=1:len
    %计算圆环的坐标
    [xCircle,yCircle]=PointsOnCircle(objX,objY,range(i));
    [xCircleIn,yCircleIn]=PointsOnCircle(objX,objY,range(i)-1);
    xRing=[xCircle;xCircleIn];
    yRing=[yCircle;yCircleIn];
    %去掉可能超过图像边界的部分
    tmp=(xRing<1)|(xRing>cols)|(yRing<1)|(yRing>rows);
    xRing(tmp)=[];
    yRing(tmp)=[];
    
    indexRing=sub2ind([rows,cols],yRing,xRing);
    numPixel(i)=sum(mask(indexRing));
    
end

[value,pos]=max(numPixel);
if value==0
    return
end
objRadius=range(pos(1));
flag=1;

