import subprocess

class Attribute(object):
    #主工程文件名
    projectName='stitiching.pro'

    #附加数据文件夹名称
    attributeDirName='attribute'
    

    #图片坐标文件名
    imageLocationName=attributeDirName+'/'+'latlon.dat'

    #DXF导入的圆信息文件名
    dxfInfoName=attributeDirName+'/'+'circle.dat'

    #控制点文件名
    controlPointName=attributeDirName+'/'+'cpoints.dat'

    #拼接结果图片名
    resultImageName=attributeDirName+'/'+'resImage.tif'

    #像素控制点坐标系转地面控制点坐标系参数文件
    pixelToGroundParms=attributeDirName+'/'+'affParms.dat'

    #描述待分割图片
    segmentImageInfoFile=attributeDirName+'/'+'segmentInfo.dat'

    #分割的临时输入文件
    segmentInputFile=attributeDirName+'/'+'segmentInput.dat'

    #分割结果文件
    segmentOutputFile=attributeDirName+'/'+'segmentOutput.dat'

    #生成的报告文件
    reportExportFile=attributeDirName+'/'+'segmentReport.dat'

    #子线程的cmd窗口属性
    startupinfo = subprocess.STARTUPINFO()
    startupinfo.dwFlags |= subprocess.STARTF_USESHOWWINDOW
    startupinfo.wShowWindow = subprocess.SW_HIDE
