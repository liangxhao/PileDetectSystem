function [objX,objY,objRadius,flag]=fitCircle(img,rx,ry,radius,ratio)
%��ȡԲ��Ŀ�겢���

%flagΪ0��ʾ���ʧ�ܣ�����Ĭ��ֵ
objX=rx;
objY=ry;
objRadius=radius;
flag=0;

[rows,cols]=size(img);
bw=imGradBw(img,radius);
%Ŀ������
[xCoords,yCoords]=ExtractObject(bw,rx,ry,radius,ratio);

if isempty(xCoords)
    bw=imBw(img,radius);
    [xCoords,yCoords]=ExtractObject(bw,rx,ry,radius,ratio);
    if isempty(xCoords)
        return
    end
end


%Ŀ������
objX=round(mean(xCoords));
objY=round(mean(yCoords));
%Ŀ���ֵͼ
indexObj=sub2ind([rows,cols],yCoords,xCoords);
mask=zeros(rows,cols);
mask(indexObj)=1;

range=floor(radius*0.5):floor(radius*1.5);
len=length(range);
numPixel=zeros(len,1);

for i=1:len
    %����Բ��������
    [xCircle,yCircle]=PointsOnCircle(objX,objY,range(i));
    [xCircleIn,yCircleIn]=PointsOnCircle(objX,objY,range(i)-1);
    xRing=[xCircle;xCircleIn];
    yRing=[yCircle;yCircleIn];
    %ȥ�����ܳ���ͼ��߽�Ĳ���
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

