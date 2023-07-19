import aw._pwm as _aw_pwm

class PWMOut_father:
    """Base Pulse Width Modulation Output Class"""
    _gpio_num = 0
    _chip = 0 
    def __init__(self, pin, *, frequency=500, duty_cycle=0, variable_frequency=False):
        _aw_pwm.switch_chip(self._chip)

        _aw_pwm.set_frequency(pin, frequency)
        _aw_pwm.set_duty_cycle(pin, duty_cycle)
        _aw_pwm.start(pin)
        self._gpio_num = pin

    def __del__(self):
        self.deinit()

    def __enter__(self):
        return self

    def __exit__(self, t, value, traceback):
        self.deinit()

    def deinit(self):
        _aw_pwm.stop(self._gpio_num)

    @property
    def duty_cycle(self):
        """The PWM's output duty cycle, 16-bit."""
        return _aw_pwm.get_duty_cycle(self._gpio_num)
        
    @duty_cycle.setter
    def duty_cycle(self, duty_cycle):
        _aw_pwm.set_duty_cycle(self._gpio_num, duty_cycle)

    @property
    def frequency(self):
        return _aw_pwm.get_frequency(self._gpio_num)

    @frequency.setter
    def frequency(self, frequency):
        _aw_pwm.set_frequency(self._gpio_num, frequency)

