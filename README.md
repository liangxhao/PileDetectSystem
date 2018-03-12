# PileDetectSystem
A pile detection system based on UAV images.
Unmanned aerial images taken on a building site.
We need to measure the coordinates and radius of the circular pile for those images.
This software is a pile measurement system.
It includes camera calibration, image splicing, polynomial geometric correction, image segmentation and recognition, AutoCAD (DXF) data reading and writing, and so on.
The interface is written in Python, based on the PyQt5 framework.
The algorithm uses C++, and the image stitching algorithm is used for reference to opencv.
And the database adopts Sqlite3.

