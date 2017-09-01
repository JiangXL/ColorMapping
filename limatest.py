from Lima import Andor3
from Lima import Core

# -------------------+--------------------------------------------+
#                    |                                            |
#                    v camlink config path                     v camera index
cam = Andor3.Camera('/users/blissadm/local/Andor3/andor/bitflow', 0)
hwint = Andor3.Interface(cam)
ct = Core.CtControl(hwint)

acq = ct.acquisition()

# configure some hw parameters
hwint.setTemperatureSP(-10)
hwint.setCooler(True)
#.... wait here for cooling

# set some low level configuration

hwint.setCooler(True)
hwint.setTemperatureSP(-55)
hwint.setFanSpeed(cam.Low)
hwint.setAdcGain(cam.b11_low_gain)
hwint.setAdcRate(cam.MHz100)
hwint.setElectronicShutterMode(cam.Rolling)
hwint.setOverlap(False)

# setting new file parameters and autosaving mode
saving=ct.saving()

pars=saving.getParameters()
pars.directory='/buffer/lcb18012/opisg/test_lima'
pars.prefix='test1_'
pars.suffix='.edf'
pars.fileFormat=Core.CtSaving.EDF
pars.savingMode=Core.CtSaving.AutoFrame
saving.setParameters(pars)

# set accumulation mode

acq_pars= acq.getPars()

#0-normal,1-concatenation,2-accumu
acq_pars.acqMode = 2
acq_pars.accMaxExpoTime = 0.05
acq_pars.acqExpoTime =1
acq_pars.acqNbFrames = 1

acq.setPars(acq_pars)
# here we should have 21 accumulated images per frame
print acq.getAccNbFrames()

# now ask for 2 sec. exposure and 10 frames
acq.setAcqExpoTime(2)
acq.setNbImages(10)

ct.prepareAcq()
ct.startAcq()

# wait for last image (#9) ready
lastimg = ct.getStatus().ImageCounters.LastImageReady
while lastimg !=9:
  time.sleep(1)
  lastimg = ct.getStatus().ImageCounters.LastImageReady

# read the first image
im0 = ct.ReadImage(0)
