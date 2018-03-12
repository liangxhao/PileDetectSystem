# 将带分割的圆的坐标信息写入文件
class SegmentInfoWriter(object):
    def __init__(self, file):
        self.file=file

    def setSegmentInfo(self,all_circles, needSegment_idx):
        self.all_circles = all_circles
        self.needSegment_idx = needSegment_idx

        self.__write()


    def __write(self):
        num=len(self.needSegment_idx)

        with open(self.file, 'w') as f:
            f.write(str(num) + '\n')

            for idx in self.needSegment_idx:
                px=int(self.all_circles[idx][0][0])
                py=int(self.all_circles[idx][0][1])
                pr=int(self.all_circles[idx][1])

                line=str(idx)+','+str(px)+','+str(py)+','+str(pr)+'\n'
                f.write(line)



