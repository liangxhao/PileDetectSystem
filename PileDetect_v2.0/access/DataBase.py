# -*- coding: UTF-8 -*-
import sqlite3
import os

class DataBase(object):
    def __init__(self,dbName):
        #数据库名称
        self.__dbName=dbName

    def __initVariable(self):
        self.__sqlFile=None
        self.__connect=None
        self.__cursor=None

    #设置创建数据库所采用的sql文件路径
    def setSQLFile(self,file):
        self.__sqlFile=file

    #连接数据库
    def connectDB(self):
        self.__connect=sqlite3.connect(self.__dbName)
        self.__cursor=self.__connect.cursor()

    #关闭数据库
    def closeDB(self):
        self.__cursor.close()
        self.__connect.close()

    
    #创建数据库
    def createDB(self):
        #删除掉已有的数据库文件
        if os.path.isfile(self.__dbName):
            os.remove(self.__dbName)
        
        #读取sql脚本文件
        with open(self.__sqlFile,'r') as f:
            qry=f.read()

        sqlite3.complete_statement(qry)

        #创建数据库
        conn = sqlite3.connect(self.__dbName)
        cursor = conn.cursor()
        cursor.executescript(qry)

        cursor.close()
        conn.close()

    #插入图像
    def insertImage(self,images):
        insert_sql = "insert into project (image) values (?)"  #?为占位符
        for item in images:
            self.__cursor.execute(insert_sql,(item,))

        self.__connect.commit()


    #更新图像状态至
    def updateImageState(self,image_id,state_point=None,state_circle=None):
        if state_point is not None:
            update_sql="update project set state_point=(?),state_circle=(?) where id=(?)"
            self.__cursor.execute(update_sql,(state_point,0,image_id))
        else:
            update_sql="update project set state_circle=(?) where id=(?)"
            self.__cursor.execute(update_sql,(state_circle,image_id))

        self.__connect.commit()


    # 查询图像名称
    def getImageNameById(self,image_idx):
        select_sql = "select image from project where id=(?)"
        self.__cursor.execute(select_sql, (image_idx,))
        # 返回一个list，list中的对象类型为tuple（元组）
        date_set = self.__cursor.fetchall()
        name = date_set[0][0]

        return name



    # 改变几何校正模式时，重置所有图像状态
    def resetImageState(self, mode):
        update_sql = "update project set state_point=0,state_circle=0 where mode<>(?)"
        self.__cursor.execute(update_sql,(mode,))

        self.__connect.commit()



    

    #保存变换参数
    def updateTransformParms(self,image_id,parms):
        update_sql="update project set parms_m1=(?),parms_m2=(?),parms_x0=(?),parms_y0=(?) where id=(?)"
        self.__cursor.execute(update_sql, parms + [image_id])

        self.__connect.commit()

    # 保存1阶几何校正参数
    def updateGeo1PolyParms(self, image_id, poly1_parms):
        update_sql = "update project set c1=(?),c2=(?),c3=(?),c4=(?),c5=(?),c6=(?) where id=(?)"

        self.__cursor.execute(update_sql, poly1_parms + [image_id])


        self.__connect.commit()


    #保存2阶几何校正参数
    def updateGeo2PolyParms(self, image_id, A, B, Ainv, Binv):
        update_sql_1 = "update project set a1=(?),a2=(?),a3=(?),a4=(?),a5=(?),a6=(?),b1=(?),b2=(?),b3=(?),b4=(?),b5=(?),b6=(?) where id=(?)"
        update_sql_2 = "update project set a1_inv=(?),a2_inv=(?),a3_inv=(?),a4_inv=(?),a5_inv=(?),a6_inv=(?),b1_inv=(?),b2_inv=(?),b3_inv=(?),b4_inv=(?),b5_inv=(?),b6_inv=(?) where id=(?)"

        self.__cursor.execute(update_sql_1, A + B + [image_id])
        self.__cursor.execute(update_sql_2, Ainv + Binv + [image_id])

        self.__connect.commit()

    #获取图像的坐标转换参数
    def getTransformParms(self,image_id):
        select_sql = "select parms_m1,parms_m2,parms_x0,parms_y0 from project where id=(?)"
        self.__cursor.execute(select_sql,(image_id,))
        #返回一个list，list中的对象类型为tuple（元组）
        date_set = self.__cursor.fetchall()

        row=date_set[0]
        parms = row

        return parms

    # 获取2阶几何校正参数
    def getGeo1PloyParms(self, image_id):
        select_sql = "select c1,c2,c3,c4,c5,c6 from project where id=(?)"
        self.__cursor.execute(select_sql, (image_id,))
        # 返回一个list，list中的对象类型为tuple（元组）
        date_set = self.__cursor.fetchall()

        row = date_set[0]
        parms = row

        return parms

    # 获取2阶几何校正参数
    def getGeo2PloyParms(self, image_id):
        select_sql = "select a1,a2,a3,a4,a5,a6,b1,b2,b3,b4,b5,b6 from project where id=(?)"
        self.__cursor.execute(select_sql, (image_id,))
        # 返回一个list，list中的对象类型为tuple（元组）
        date_set = self.__cursor.fetchall()

        row = date_set[0]
        # A,B = [row[0], row[1], row[2], row[3],row[4],row[5]], [row[6], row[7], row[8], row[9],row[10],row[11]]
        A ,B = row[:6], row[6:]
        #########
        select_sql = "select a1_inv,a2_inv,a3_inv,a4_inv,a5_inv,a6_inv,b1_inv,b2_inv,b3_inv,b4_inv,b5_inv,b6_inv from project where id=(?)"
        self.__cursor.execute(select_sql, (image_id,))
        # 返回一个list，list中的对象类型为tuple（元组）
        date_set = self.__cursor.fetchall()

        row = date_set[0]
        # Ainv, Binv = [row[0], row[1], row[2], row[3],row[4],row[5]], [row[6], row[7], row[8], row[9],row[10],row[11]]
        Ainv, Binv =row[:6], row[6:]

        return [A,B,Ainv,Binv]



    #用于图像叠加的参数
    def getImageDesc(self):
        select_sql = "select image,parms_m1,parms_m2,p1_x,p1_y,p2_x,p2_y,p3_x,p3_y,p4_x,p4_y from project where state_point=1 and state_circle=1 order by avg_pe DESC"

        self.__cursor.execute(select_sql)
        
        #返回一个list，list中的对象类型为tuple（元组）
        date_set = self.__cursor.fetchall()

        images=[]
        parms=[]
        limit_x=[]
        limit_y=[]

        for row in date_set:
            images.append(row[0])

            parms.append([row[1],row[2]])
            limit_x.append([row[3],row[5],row[7],row[9]])
            limit_y.append([row[4],row[6],row[8],row[10]])
            
        return images,parms,limit_x,limit_y


    # 获取当前设置的几何校正模式
    def getCorrectMode(self):
        select_sql="select geoCorrectMode from correct_mode"

        self.__cursor.execute(select_sql)

        # 返回一个list，list中的对象类型为tuple（元组）
        date_set = self.__cursor.fetchall()

        return date_set[0][0]


    # 设置几何校正模式
    def setCorrectMode(self, mode):
        update_sql = "update correct_mode set geoCorrectMode=(?)"
        self.__cursor.execute(update_sql, (mode,))

        self.__connect.commit()



    
    #查询图像
    def getImages(self):
        select_sql = "select id,image,state_point,state_circle from project"
        self.__cursor.execute(select_sql)
        
        #返回一个list，list中的对象类型为tuple（元组）
        date_set = self.__cursor.fetchall()

        ids=[]
        images=[]
        states=[]

        for row in date_set:
            ids.append(row[0])
            images.append(row[1])
            states.append(row[2]+row[3])

        return ids,images,states


    def getImageTransformParmsByState(self):
        select_sql = "select id,image,parms_m1,parms_m2,parms_x0,parms_y0 from project where state_point=1 and state_circle=0"

        self.__cursor.execute(select_sql)      
        #返回一个list，list中的对象类型为tuple（元组）
        date_set = self.__cursor.fetchall()

        ids=[]
        images=[]
        parms=[]

        for row in date_set:
            ids.append(row[0])
            images.append(row[1])
            parms.append([row[2],row[3],row[4],row[5]])

        return ids,images,parms

    #更新图像的地理坐标范围
    def updateImageLimit(self,ids,limits):
        update_sql= "update project set p1_x=(?),p1_y=(?),p2_x=(?),p2_y=(?),p3_x=(?),p3_y=(?),p4_x=(?),p4_y=(?) where id=(?)"

        for idx,limit in zip(ids,limits):
            (p1,p2,p3,p4)=limit
            self.__cursor.execute(update_sql,(p1[0],p1[1],p2[0],p2[1],p3[0],p3[1],p4[0],p4[1],idx))

        self.__connect.commit()

    # 更新图像坐标转换参数的模式（0,1,2三种）
    def updateImageParmsMode(self, idx, mode):
        update_sql = "update project set mode=(?) where id=(?)"
        self.__cursor.execute(update_sql,(mode,idx))
        self.__connect.commit()



        #插入控制点
    def insertCPoints(self,names,points):
        insert_sql = "insert into cpoints (point_name,x,y) values (?,?,?)"  #?为占位符

        for name,point in zip(names,points):
            (x,y)=point
            self.__cursor.execute(insert_sql,(name,x,y))

        self.__connect.commit()       


    #查询控制点
    def getCPoints(self):
        select_sql = "select id,point_name,x,y from cpoints"
        self.__cursor.execute(select_sql)
        
        #返回一个list，list中的对象类型为tuple（元组）
        date_set = self.__cursor.fetchall()

        ids=[]
        names=[]
        points=[]

        for row in date_set:
            ids.append(row[0])
            names.append(row[1])
            points.append((row[2],row[3]))

        return ids,names,points



    #获取已选择像素控制点数据
    def getPixelCPoints(self,image_id):
        select_sql = "select cpoint_id,cpoint_name,px,py,p_error,g_error from select_cpoints_with_names where image_id=(?)"
        self.__cursor.execute(select_sql,(image_id,))
        
        #返回一个list，list中的对象类型为tuple（元组）
        date_set = self.__cursor.fetchall()

        ids=[]
        names=[]
        points=[]
        pErrors=[]
        gErrors=[]

        for row in date_set:
            ids.append(row[0])
            names.append(row[1])

            points.append([row[2],row[3]])

            pErrors.append(row[4])
            gErrors.append(row[5])


        return ids,names,points,pErrors,gErrors


    #插入已选择的控制点（某张图像的）
    def insertSelectCPoints(self,image_id,cpoints_id,points,pErrors,gErrors):
        #先删除原有数据
        del_sql="delete from select_cpoints where image_id=(?)"
        self.__cursor.execute(del_sql,(image_id,))

        #更新每张图片的平均误差
        if pErrors is not None:
            aveErrors=sum(pErrors)/len(cpoints_id)

            update_sql="update project set avg_pe=(?) where id=(?)"
            self.__cursor.execute(update_sql,(aveErrors, image_id))
            self.__connect.commit()

        else:
            pErrors=gErrors=[None]*len(cpoints_id)

        #插入新数据
        insert_sql = "insert into select_cpoints (image_id,cpoint_id,px,py,p_error,g_error) values (?,?,?,?,?,?)"  #?为占位符
        for cpoint_id,point,pe,ge in zip(cpoints_id,points,pErrors,gErrors):
            (px,py)=point
            self.__cursor.execute(insert_sql,(image_id, cpoint_id, px, py,pe,ge))

        self.__connect.commit()       


    #插入圆（所有的理论地理坐标）
    def insertCircles(self,names,locations,radius):
        insert_sql = "insert into circles (circle_name,x,y,radius) values (?,?,?,?)"  #?为占位符

        for (name,loc,rad) in zip(names, locations, radius):
            (x,y)=loc
            self.__cursor.execute(insert_sql,(name,x,y,rad))

        self.__connect.commit()  

    #查询圆（所有的理论地理坐标）
    def getCircles(self):
        select_sql = "select id,circle_name,x,y,radius from circles order by circle_name"
        self.__cursor.execute(select_sql)
        
        #返回一个list，list中的对象类型为tuple（元组）
        date_set = self.__cursor.fetchall()

        ids=[]
        names=[]
        locations=[]
        radius=[]

        for row in date_set:
            ids.append(row[0])
            names.append(row[1])
            locations.append((row[2],row[3]))
            radius.append(row[4])

        return ids,names,locations,radius


    #获取某个圆的坐标（理论地理坐标）
    def getCircleById(self,idx):
        select_sql = "select x,y,radius from circles where id=(?)"
        self.__cursor.execute(select_sql,(idx,))
        
        #返回一个list，list中的对象类型为tuple（元组）
        date_set = self.__cursor.fetchall()

        x=date_set[0][0]
        y=date_set[0][1]
        radius=date_set[0][1]

        return x,y,radius

    #删除某一个圆
    def deleteCircleById(self,idx):
        del_sql="delete from circles where id=(?)"
        self.__cursor.execute(del_sql,(idx,))

        self.__connect.commit()  




    #获取当前图像上每个圆
    def getCirclesByImage(self,image_id):
        select_sql = "select circle_id,circle_name,clc_px,clc_py,clc_pr,sel_px,sel_py,sel_pr, g_error from select_circles_with_names where image_id=(?)"
        self.__cursor.execute(select_sql,(image_id,))
        
        #返回一个list，list中的对象类型为tuple（元组）
        date_set = self.__cursor.fetchall()

        ids=[]
        names=[]
        clc_Points=[]
        clc_radiuses=[]
        sel_Points=[]
        sel_radiuses=[]
        gErrors=[]

        for row in date_set:
            ids.append(row[0])
            names.append(row[1])
            clc_Points.append([row[2],row[3]])
            clc_radiuses.append(row[4])
            
            #可能为None
            sel_Points.append([row[5],row[6]])
            sel_radiuses.append(row[7])
            gErrors.append(row[8])

        return ids,names,clc_Points,clc_radiuses,sel_Points,sel_radiuses,gErrors

    #更新某张图像上圆的选择的坐标
    def updateSelectCircleByImage(self,image_id,circle_id,pPoint,pr,gPoint,gr,gError):
        update_sql="update select_circles set sel_px=(?),sel_py=(?),sel_pr=(?), sel_gx=(?),sel_gy=(?),sel_gr=(?), g_error=(?) where image_id=(?) and circle_id=(?)"

        self.__cursor.execute(update_sql,(pPoint[0],pPoint[1],pr,gPoint[0],gPoint[1],gr, gError,image_id,circle_id))

        self.__connect.commit() 


    #获得图像的状态
    def getImageState(self,image_id):
        select_sql = "select state_point,state_circle from project where id=(?)"
        self.__cursor.execute(select_sql,(image_id,))    
        #返回一个list，list中的对象类型为tuple（元组）
        date_set = self.__cursor.fetchall()

        state_point=date_set[0][0]
        state_circle=date_set[0][1]
        return state_point,state_circle

    #插入计算出的待选择的圆
    def insertSelectCircle(self,image_id,circle_ids,pPoints,radius):
        #先删除原有数据
        del_sql="delete from select_circles where image_id=(?)"
        self.__cursor.execute(del_sql,(image_id,))


        insert_sql="insert into select_circles (image_id,circle_id,clc_px,clc_py,clc_pr) values (?,?,?,?,?)"

        for circle_id,point in zip(circle_ids,pPoints):
            self.__cursor.execute(insert_sql,(image_id,circle_id,point[0],point[1],radius))   

        self.__connect.commit() 

    #插入汇总的统计结果
    def updateCollectCircles(self,circle_ids,gPoints,grs):
        #先将之前的结果删除
        update_sql="update circles set gx=null,gy=null,gr=null,error_gx=null,error_gy=null,error_gr=null"
        self.__cursor.execute(update_sql)
        self.__connect.commit()

        update_sql="update circles set gx=(?),gy=(?),gr=(?) where id=(?)"
        for idx,gp,gr in zip(circle_ids,gPoints,grs):
            self.__cursor.execute(update_sql,(gp[0],gp[1],gr,idx))

        self.__connect.commit()


    #根据选择的圆汇总结果，并获取
    def getCollectCircles(self):
        select_sql="select circle_id,gx,gy,gr from collect_circles"

        self.__cursor.execute(select_sql)    
        #返回一个list，list中的对象类型为tuple（元组）
        date_set = self.__cursor.fetchall()

        ids=[]
        gPoints=[]
        grs=[]

        for row in date_set:
            ids.append(row[0])
            gPoints.append([row[1],row[2]])
            grs.append(row[3])

        return ids,gPoints,grs


    #获取最终的统计结果（
    def getReport(self,needNull=True):
        # if needNull:
        select_sql="select circle_name,x,y,radius,gx,gy,gr,error_gx,error_gy,error_gr from circles"
        # else:
        #     select_sql="select circle_name,x,y,gx,gy,error_gx,error_gy from circles where gx is not null"

        self.__cursor.execute(select_sql)    
        
        date_set = self.__cursor.fetchall()

        names=[]
        points=[]
        radiuses=[]
        gPoints=[]
        grs=[]
        errors=[]

        for row in date_set:
            names.append(row[0])

            points.append([row[1],row[2]])
            radiuses.append(row[3])
            if needNull==False:
                if row[4] is None:
                    continue
            gPoints.append([row[4],row[5]])
            grs.append(row[6])
            errors.append([row[7],row[8],row[9]])

        return names,points,radiuses,gPoints,grs,errors


    #获取最终的统计结果（不带坐标）
    def getReportWithoutCoords(self):
        select_sql="select circle_name,error_gx,error_gy,error_gr from circles"
        self.__cursor.execute(select_sql)    
        
        date_set = self.__cursor.fetchall()
        names=[]
        errors=[]

        for row in date_set:
            names.append(row[0])
            errors.append([row[1],row[2],row[3]])

        return names,errors

    #获取半径
    def getAnyRadius(self):
        radius=0

        select_sql="select radius from circles limit 1"
        self.__cursor.execute(select_sql)    

        
        date_set = self.__cursor.fetchall()
        
        if len(date_set)!=0:
            radius=date_set[0][0]

        return radius


    







        
    





            




        

        

