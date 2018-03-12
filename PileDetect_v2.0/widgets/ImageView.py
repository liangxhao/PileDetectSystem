from PyQt5.QtWidgets import QGraphicsView,QGraphicsScene
from PyQt5.QtCore import Qt, QRectF,pyqtSignal
from PyQt5.QtGui import QImage, QPixmap,QTransform


class ImageView(QGraphicsView):


    # rightMouseButtonPressed = pyqtSignal(float, float)


    def __init__(self,parent=None):
        super(ImageView,self).__init__()

        self.scene=QGraphicsScene()
        self.setScene(self.scene)
        self._pixmapHandle = None
        self.aspectRatioMode = Qt.KeepAspectRatio

        # self.setDragMode(QGraphicsView.ScrollHandDrag)


        self.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
        self.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOff)


        # self.rightMouseButtonPressed.connect(self.handleRightClick)

        self.zoom=-1
      

    def hasImage(self):
        return self._pixmapHandle is not None


    def clearImage(self):
        """ Removes the current image pixmap from the scene if it exists.
        """
        if self.hasImage():
            self.scene.removeItem(self._pixmapHandle)
            self._pixmapHandle = None
            self.zoom=-1
            self.scene.clear()



    def pixmap(self):
        """ Returns the scene's current image pixmap as a QPixmap, or else None if no image exists.
        :rtype: QPixmap | None
        """
        if self.hasImage():
            return self._pixmapHandle.pixmap()
        return None



    def image(self):
        """ Returns the scene's current image pixmap as a QImage, or else None if no image exists.
        :rtype: QImage | None
        """
        if self.hasImage():
            return self._pixmapHandle.pixmap().toImage()
        return None

    def setImage(self, image):
        """ Set the scene's current image pixmap to the input QImage or QPixmap.
        Raises a RuntimeError if the input image has type other than QImage or QPixmap.
        :type image: QImage | QPixmap
            """
        if type(image) is QPixmap:
            pixmap = image
        elif type(image) is QImage:
            pixmap = QPixmap.fromImage(image)
        else:
            raise RuntimeError("ImageViewer.setImage: Argument must be a QImage or QPixmap.")
        if self.hasImage():
            self._pixmapHandle.setPixmap(pixmap)
        else:
            self._pixmapHandle = self.scene.addPixmap(pixmap)
        self.setSceneRect(QRectF(pixmap.rect()))  # Set scene size to image size.
        self.updateViewer()
    


    def updateViewer(self):
        """ Show current zoom (if showing entire image, apply current aspect ratio mode).
        """
        if not self.hasImage():
            return
        if self.zoom < 0:
            if self.size().width() > self.size().height():
                self.zoom = self.size().height() / self.scene.height()
            else:
                self.zoom = self.size().width() / self.scene.width()

        self.setTransform(QTransform().scale(self.zoom, self.zoom))


            

    def wheelEvent(self,event):
        moose=event.angleDelta().y()

        if moose>0:
            self.zoomIn()
        elif moose<0:
            self.zoomOut()


    def zoomIn(self):
        self.zoom *= 1.05
        self.updateViewer()

    def zoomOut(self):
        self.zoom /= 1.05
        self.updateViewer()   


    def mousePressEvent(self, event):

        if event.button() == Qt.RightButton:

            scenePos = self.mapToScene(event.pos())
            # self.rightMouseButtonPressed.emit(scenePos.x(), scenePos.y())
            self.handleRightClick(scenePos.x(), scenePos.y())

        elif event.button() == Qt.LeftButton:
            self.setDragMode(QGraphicsView.ScrollHandDrag)
            
        QGraphicsView.mousePressEvent(self, event)



    def mouseReleaseEvent(self, event):

        QGraphicsView.mouseReleaseEvent(self, event)

        if event.button() == Qt.LeftButton:
            self.setDragMode(QGraphicsView.NoDrag)
             

    
    def handleRightClick(self,x, y):
        # row = int(y)
        # col = int(x)
        # print("Clicked on image pixel (row="+str(row)+", column="+str(col)+")")
        pass



