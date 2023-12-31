
class PIN_father:
    IN = 0
    OUT = 1
    LOW = 0
    HIGH = 1
    PULL_NONE = 0
    PULL_UP = 1
    PULL_DOWN = 2

    id = None
    _value = LOW
    _mode = IN

    # 需要在子类中定义这3个函数
    _setup = None
    _output = None
    _input = None

    def __init__(self, pin_name):
        if isinstance(pin_name, tuple):
            self.id = int(pin_name[1])
        else:
            self.id = int(pin_name)

    def __repr__(self):
        return str(self.id)

    def __eq__(self, other):
        return self.id == other

    def init(self, mode=IN, pull=None):
        """Initialize the Pin"""
        if mode is not None:
            if mode == self.IN:
                self._mode = self.IN
                # GPIO.setup(self.id, GPIO.IN)
                self._setup(self.id, self.IN)

            elif mode == self.OUT:
                self._mode = self.OUT
                # GPIO.setup(self.id, GPIO.OUT)
                self._setup(self.id, self.OUT)

            else:
                raise RuntimeError("Invalid mode for pin: %s" % self.id)
        if pull is not None:
            if self._mode != self.IN:
                raise RuntimeError("Cannot set pull resistor on output")
            if pull == self.PULL_UP:
                # GPIO.setup(self.id, GPIO.IN, pull_up_down=GPIO.PUD_UP)
                self._setup(self.id, self.IN, pull_up_down=self.PULL_UP)
                

            elif pull == self.PULL_DOWN:
                # GPIO.setup(self.id, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
                self._setup(self.id, self.IN, pull_up_down=self.PULL_DOWN)

                pass
            else:
                raise RuntimeError("Invalid pull for pin: %s" % self.id)
            
    def value(self, val=None):
            """Set or return the Pin Value"""
            if val is not None:
                if val == self.LOW:
                    self._value = val
                    # GPIO.output(self.id, val)
                    self._output(self.id, val)

                elif val == self.HIGH:
                    self._value = val
                    # GPIO.output(self.id, val)
                    self._output(self.id, val)

                else:
                    raise RuntimeError("Invalid value for pin")
                return None
            return self._input(self.id)