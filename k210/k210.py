import sensor, time, image
import utime
import lcd
import time
import uos



sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
#sensor.set_auto_exposure(1)
#sensor.set_auto_exposure(0, exposure=120000)
sensor.set_auto_gain(0, gain_db = 1)
sensor.set_auto_whitebal(0, rgb_gain_db = (0,0,0))
sensor.set_brightness(-3)   #设置亮度
sensor.set_contrast(3) #对比度
sensor.set_vflip(False)
sensor.set_hmirror(False)
#sensor.set_windowing((224, 224))
#sensor.set_auto_exposure(False,1000)  #曝光速度
sensor.skip_frames(time = 900)

lcd.init()


# 识别激光确定位置
while True:
    #sensor.set_brightness(-3)   #设置亮度
    #sensor.set_contrast(3) #对比度
    #sensor.set_auto_exposure(False,1000)  #曝光速度
    print('识别激光确定位置中')
    img = sensor.snapshot()           # 获取图片
    red_x = 0
    red_y = 0
    #,(80, 100, -11, 0, 0, 10)
    for blobs in img.find_blobs([(96, 100, -128, 128, -128, 128)],
    x_stride = 1,y_stride = 1,pixels_threshold  = 2,
    area_threshold    =  2,
    merge = True, margin = 10):
        #if 50 < blobs.pixels():               # 找到面积最大的色块
        #print(blobs)
        #if blobs.code() == 1:
        red_x = blobs.x()
        red_y = blobs.y()
        img.draw_cross(blobs.cx(),blobs.cy(), color=(255,0,0), size = 8)

    lcd.display(img)
