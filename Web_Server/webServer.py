from flask import Flask, render_template

mos = 2
tmp1 = 2
tmp2 = 2
hum1 = 2
hum2 = 2
lux = 2
tmpTarget = 30 #목표온도기준값
humTarget = 30 #목표습도기준값
manualControl = 1 #수동/자동제어
########################################
#수동용변수
fanStateCtrl = 2 #팬 온/오프
heatStateCtrl = 2 #히터 온/오프
leftWindowCtrl = 2 #왼쪽개폐기 온/오프
rightWindowCtrl = 2 #오른쪽개폐기 온/오프
########################################
#자동용변수
fanState = 2 #팬 온/오프
fanSpeed = 2 #팬속도제어
leftWindow = 2 #왼쪽개폐기 온/오프
rightWindow = 2 #오른쪽개폐기 온/오프
heaterState = 2 #히터 온/오프
########################################
#컨트롤러변수
ctrlLeftWindow = 0
ctrlRightWindow = 0
ctrlHeater = 0
ctrlFan = 0

app = Flask(__name__)

@app.route('/getAll')
def get_all():
	return '@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@END@' % (mos, tmp1, tmp2, hum1, hum2, lux, tmpTarget, manualControl, fanStateCtrl, fanSpeed, leftWindowCtrl, rightWindowCtrl, heatStateCtrl)

@app.route('/getState')
def get_state():
	return '%s/%s/%s/%s/%s/%s/%s/%s/' % (tmp1, tmp2, hum1, hum2,leftWindow, rightWindow, fanState, heaterState)

#우측 좌측 열풍기 팬
@app.route('/controllerSetter/<ctrlRightWindowIn>/<ctrlLeftWindowIn>/<ctrlHeaterIn>/<ctrlFanIn>')
def controllerSetter(ctrlRightWindowIn,ctrlLeftWindowIn,ctrlHeaterIn,ctrlFanIn):
	global ctrlRightWindow, ctrlLeftWindow, ctrlHeater, ctrlFan
	ctrlRightWindow = ctrlRightWindowIn
	ctrlLeftWindow = ctrlLeftWindowIn
	ctrlHeater = ctrlHeaterIn
	ctrlFan = ctrlFanIn
	return 'ok'

@app.route('/setTargetTmp/<tmp>')
def setTargetTmp(tmp):
	global tmpTarget
	tmpTarget = tmp
	return 'Ok'

@app.route('/getTargetTmp')
def getTargetTmp():
	return '@%s' % tmpTarget

@app.route('/setTargetHum/<hum>')
def setTargetHum(hum):
	global humTarget
	humTarget = hum
	return 'Ok'

@app.route('/getTargetHum')
def getTargetHum():
	return '@%s' % humTarget

@app.route('/tmp1')
def hello_tmp1():
	return '현재온도@%s' % tmp1

@app.route('/tmp2')
def hello_tmp2():
	return '현재온도@%s' % tmp2

@app.route('/tmpUp')
def tmpUp():
	global tmpTarget 
	tmpTarget = tmpTarget+1
	return '목표온도@%s'% tmpTarget

@app.route('/tmpDown')
def tmpDown():
        global tmpTarget
        tmpTarget = tmpTarget-1
        return '목표온도@%s'% tmpTarget

@app.route('/hum1')
def hello_hum1():
	return '현재습도@%s' % hum1

@app.route('/hum2')
def hello_hum2():
	return '현재습도@%s' % hum2

@app.route('/humUp')
def humUp():
	global humTarget
	humTarget = humTarget + 1
	return '목표습도@%s' % humTarget

@app.route('/humDown')
def humDown():
        global humTarget
        humTarget = humTarget - 1
        return '목표습도@%s' % humTarget

@app.route('/lux')
def hello_lux():
	return '현재조도 @%s' % lux

@app.route('/grd')
def hello_grd():
	return '현재토양 @%s' % mos


#수동/자동제어
@app.route('/setManualControl/<OnOff>')
def setManualControl(OnOff):
	global manualControl
	manualControl  = OnOff

@app.route('/getManualControl')
def getManualControl():
	return '@%s' %  manualControl

#####################################
#수동용 온오프 여부 표시 변수 추가
#팬 온/오프
@app.route('/setFanC/<OnOff>')
def setFanC(OnOff):
	global fanStateCtrl
	fanStateCtrl = OnOff
	return 'Ok'

@app.route('/getFanC')
def getFanC():
	return '@%s' % fanStateCtrl

#히터 온/오프
@app.route('/setHeatC/<OnOff>')
def setHeatC(OnOff):
	global heatStateCtrl
	heatStateCtrl = OnOff
	return 'Ok'

@app.route('/getHeatC')
def getHeatC():
	return '@%s' % heatStateCtrl

#왼쪽개폐기 온/오프
@app.route('/setLeftWindowC/<OnOff>')
def setLeftWindowC(OnOff):
	global leftWindowCtrl
	leftWindowCtrl = OnOff
	return 'Ok'

@app.route('/getLeftWindowC')
def getLeftWindowC():
	return '@%s' % leftWindowCtrl

#오른쪽개폐기 온/오프
@app.route('/setRightWindowC/<OnOff>')
def setRightWindowC(OnOff):
	global rightWindowCtrl
	rightWindowCtrl = OnOff
	return 'Ok'

@app.route('/getRightWindowC/')
def getRightWindowC():
	return '@%s' % rightWindowCtrl 

###################################3
#자동용
#팬 온/오프
@app.route('/setFanOnOff/<OnOff>')
def setFanOnOff(OnOff):
	global fanState
	fanState = OnOff

@app.route('/getFanState')
def getFanState():
	return '@%s' % fanState

#팬 속도제어
@app.route('/setFanSpeed/<speed>')
def setFanSpeed(speed):
	global fanSpeed
	fanSpeed = speed

@app.route('/getFanSpeed')
def getFanSpeed():
	return '@%s' % fanSpeed

#개폐기 좌우 온/오프
@app.route('/setLeftWindow/<OnOff>')
def setLeftWindow(OnOff):
	global leftWindow
	leftWindow = OnOff

@app.route('/setRightWindow/<OnOff>')
def setRightWindow(OnOff):
	global rightWindow
	rightWindow = OnOff

@app.route('/getLeftWindow')
def getLeftWindow():
	return '@%s' % leftWindow

@app.route('/getRightWindow')
def getRightWindow():
	return '@%s' % rightWindow

#히터 온/오프
@app.route('/setHeaterOnOff/<OnOff>')
def setHeaterOnOff(OnOff):
	global heaterState
	heaterState = OnOff

@app.route('/getHeaterState')
def getHeaterState():
	return '@%s' % heaterState

@app.route('/')
def home():
	return render_template("home.html", mosH=mos,tmp1H=tmp1,tmp2H=tmp2,hum1H=hum1,hum2H=hum2,luxH=lux)

@app.route('/<mosIn>/<tmp1In>/<tmp2In>/<hum1In>/<hum2In>/<luxIn>')
def home2(mosIn,tmp1In,tmp2In,hum1In,hum2In,luxIn):
	global mos,tmp1,tmp2,hum1,hum2,lux
	mos = mosIn
	tmp1 = tmp1In
	tmp2 = tmp2In
	hum1 = hum1In
	hum2 = hum2In
	lux = luxIn
	return render_template("change.html",mosH=mosIn,tmp1H=tmp1In,tmp2H=tmp2In,hum1H=hum1In,hum2H=hum2In,luxH=luxIn)

if __name__ == '__main__':
	app.run(host='0.0.0.0', port=80)
