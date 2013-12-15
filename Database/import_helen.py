import json

db_directory = './Database/Helen/'

output = []

for filename in range(1, 2331):
	annotation = open(db_directory + 'annotation/' + str(filename) + '.txt', 'r')
	pic_name = annotation.readline()[:-2] + '.jpg'
	pic_path = db_directory + 'pictures/'
	#print pic_path + pic_name
	coord = annotation.read().split('\r\n')
	points = []
	for point in coord:
		if point:
			[x,y] = point.split(' , ')
			points.append((float(x), float(y)))
	data = { 'picture_path' : pic_path + pic_name, \
						'landmarks' : points }
	output.append(json.dumps(data, indent=2) + ",\n")	

f = open('./Database/Helen.json', 'w')
f.write('[')
f.write(''.join(output))
f.write(']')

