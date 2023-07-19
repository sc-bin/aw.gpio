VERSION = '0.1'

from aw._gpio import select as _select
import  aw._common as _chips 
_now_chip = _select()
if _now_chip == _chips.CHIP_H616 :
    from aw.gpio.H616 import *
