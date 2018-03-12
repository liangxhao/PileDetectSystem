开发环境：

- Visual Studio 2017   （use vc140）
- python 3.5

模块介绍：

- calibratorModule ：相机标定。通过棋盘格图像求解标定参数
- exifcoords：读取图像的经纬度坐标
- imageComposite：图像叠加（拼接）。已知图像的四参数（或者说地理坐标），将它们拼合成一张大图
- imgStitcher：图像拼接。stitching算法，通过图像连接点光束法平差求解相机外参数，再对图像校正、投影、曝光补偿、去拼接线，最终拼合成一张大图
- photosUndistort：图像畸变校正。通过导入相机标定得到的相机参数对图像进行校正。
- segment：图像上基桩的分割与识别（效果不好）

