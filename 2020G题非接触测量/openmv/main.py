import sensor, image, time, math, os, tf, uos, gc
from pyb import UART
uart = UART(3, 115200)
laser_thresholds = (3, 150)
red_threshold = (10, 25, 21, 65, 17, 33)
green_threshold = (26, 64, -56, -34, -10, 57)
blue_threshold = (31, 70, -51, -15, -37, 4)
baeskball = (0, 14, -23, 39, -25, 25)
vallball = (2, 17, -8, 52, -2, 55)
color_thresholds = [red_threshold, green_threshold, blue_threshold]
color_thresholds_ball = [baeskball, vallball]
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=2000)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)
clock = time.clock()
laser_cx, laser_cy = None, None
start_flag = 0
run_model_flag = 0
net = None
labels = None
min_confidence = 0.5
try:
	net = tf.load("trained.tflite", load_to_fb=uos.stat('trained.tflite')[6] > (gc.mem_free() - (64*1024)))
except Exception as e:
	raise Exception('Failed to load "trained.tflite", did you copy the .tflite and labels.txt file onto the mass-storage device? (' + str(e) + ')')
try:
	labels = [line.rstrip('\n') for line in open("labels.txt")]
except Exception as e:
	raise Exception('Failed to load "labels.txt", did you copy the .tflite and labels.txt file onto the mass-storage device? (' + str(e) + ')')
colors = [
	(255,   0,   0),
	(  0, 255,   0),
	(255, 255,   0),
	(  0,   0, 255),
	(255,   0, 255),
	(  0, 255, 255),
	(255, 255, 255),
]
def detect_laser_spot(img):
	blobs = img.find_blobs([laser_thresholds], pixels_threshold=10, area_threshold=10, merge=True)
	if blobs:
		b = blobs[0]
		img.draw_rectangle(b.rect())
		img.draw_cross(b.cx(), b.cy())
		return b.cx(), b.cy()
	return None, None
def identify_color_and_shape(blob):
	for threshold in color_thresholds:
		stats = img.get_statistics(thresholds=[threshold], roi=(2,43,305,130))
		l_mode = stats.l_mode()
		a_mode = stats.a_mode()
		b_mode = stats.b_mode()
		if threshold[0] < l_mode < threshold[1] and threshold[2] < a_mode < threshold[3] and threshold[4] < b_mode < threshold[5]:
			color = ""
			if threshold == red_threshold:
				color = "Red"
			elif threshold == green_threshold:
				color = "Green"
			elif threshold == blue_threshold:
				color = "Blue"
			shape_density = blob.density()
			size = 0
			if shape_density > 0.75:
				shape = "Rectangle"
				size = int((blob.w() + blob.h()) / 2)
			elif shape_density > 0.60 :
				shape = "Circle"
				size = int((blob.w() + blob.h()) / 2)
			elif shape_density > 0.3 :
				shape = "Triangle"
				size = int((blob.h()+blob.w())/1.737)
			return color, shape, size
	return "Unknown", "Unknown", 0
def run_model():
	img = sensor.snapshot()
	sensor.set_windowing((240, 240))
	for i, detection_list in enumerate(net.detect(img, thresholds=[(math.ceil(min_confidence * 255), 255)])):
		if (i == 0): continue
		if (len(detection_list) == 0): continue
		for d in detection_list:
			[x, y, w, h] = d.rect()
			center_x = math.floor(x + (w / 2))
			center_y = math.floor(y + (h / 2))
			if center_x != 0x00 and center_y != 0x00:
				uart.write(b"\x10")
				uart.write("{:03d}{:03d}{:03d}{:03d}".format(center_x, center_y, laser_cx if laser_cx is not None else 0, laser_cy if laser_cy is not None else 0).encode())
				uart.write(b"\xEE")
				print('x %d\ty %d' % (center_x, center_y))
			if labels[i] == "篮球":
				uart.write(b"\x20")
				uart.write(b"\x5A")
				uart.write("{:03d}".format(int((w+h)/2)).encode())
				uart.write(b"\xDD")
				print("篮球")
				print(int((w+h)/2))
			elif labels[i] == "排球":
				uart.write(b"\x20")
				uart.write(b"\x5B")
				uart.write("{:03d}".format(int((w+h)/2)).encode())
				uart.write(b"\xDD")
				print("排球")
				print(int((w+h)/2))
while True:
	clock.tick()
	if uart.any() == 1:
		rxdata = uart.read(1).decode()
		if rxdata == 'S':
			start_flag = 1
		elif rxdata == 'F':
			start_flag = 0
		elif rxdata == 'B':
			run_model_flag = 1
	if run_model_flag == 1:
		laser_cx = 120
		laser_cy = 121
		run_model()
	if start_flag == 1:
		laser_cx = 160
		laser_cy = 121
		img = sensor.snapshot()
		for blob in img.find_blobs(color_thresholds, pixels_threshold=200, roi=(0, 0, 320, 240), area_threshold=200):
			color, shape, size = identify_color_and_shape(blob)
			if shape != "Unknown":
				shape_code_map = {
					("Red", "Circle"): 0x51,
					("Green", "Circle"): 0x52,
					("Blue", "Circle"): 0x53,
					("Red", "Rectangle"): 0x54,
					("Green", "Rectangle"): 0x55,
					("Blue", "Rectangle"): 0x56,
					("Red", "Triangle"): 0x57,
					("Green", "Triangle"): 0x58,
					("Blue", "Triangle"): 0x59
				}
				shape_code = shape_code_map.get((color, shape), 0x00)
				if shape_code != 0x00:
					uart.write(b"\x20")
					uart.write(bytes([shape_code]))
					uart.write("{:03d}".format(size).encode())
					uart.write(b"\xDD")
					shape_cx, shape_cy = blob.cx(), blob.cy()
					uart.write(b"\x10")
					uart.write("{:03d}{:03d}{:03d}{:03d}".format(shape_cx, shape_cy, laser_cx if laser_cx is not None else 0, laser_cy if laser_cy is not None else 0).encode())
					uart.write(b"\xEE")
					print(f"检测到{shape}，颜色为{color}，尺寸为{size}mm")
					print(f"图形中心坐标为 ({shape_cx}, {shape_cy})")
					if laser_cx is not None and laser_cy is not None:
						print(f"激光坐标为 ({laser_cx}, {laser_cy})")
					else:
						print("未检测到激光点")
				else:
					print(f"未识别的图形和颜色组合：{color} {shape}")
	print(clock.fps())