
如果使用pyinstaller打包，请看以下说明

（main run)入口文件：PileOffset.py

打包方式：Pyinstaller

打包参数：pyisntaller.spec

打包后将 config、ezdxf、modules三个文件夹拷进去


pyinstaller3.3.1可能存在问题，打包完成后需要将PyQT5目录下的platforms拷贝至根目录


