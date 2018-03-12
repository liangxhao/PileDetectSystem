# PileDetectSystem
## A pile detection system based on UAV images.

Unmanned aerial images taken on a building site.

We need to measure the coordinates and radius of the circular pile for those images.

This software is a pile measurement system.

It includes camera calibration, image splicing, polynomial geometric correction, image segmentation and recognition, AutoCAD (DXF) data reading and writing, and so on.

The interface is written in Python3, based on the PyQt5 framework.

The algorithm uses Visual C++ 2017, and the image stitching algorithm is used for reference to opencv.

And the database adopts Sqlite3.

The version of v1.0 is rather chaotic and the image splicing is too slow, so I discarded it.

The PileDetect_v1.0 and PileDetect_v2.0 folders are part of the interface based on Python3.

The PileDetect_Modules folder is the main part of the algorithm based on Visual C++.

