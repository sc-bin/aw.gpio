from setuptools import setup, Extension

classifiers = [
        'Development Status :: 3 - Alpha',
        'Intended Audience :: Developers',
        'Topic :: Software Development :: Build Tools',
        'License :: OSI Approved :: MIT License',
        'Programming Language :: Python :: 3',
    ]

setup(
    name="aw.gpio",
    version="0.1",
    author="sc-bin",
    author_email="3335447573@qq.com",
    description="A module to control Allwinner chips GPIO",
    long_description = open('README.md').read() + open('CHANGELOG.md').read(),
    license          = 'MIT',
    url="https://github.com/sc-bin/aw.gpio", 
    packages=['aw','aw.gpio'],
    ext_modules = [Extension('aw._gpio', ['src/init.c']),
                   Extension('aw._h616', ['src/h616_gpio.c', 'src/h616_py.c', 'src/common_py.c']),
                   Extension('aw._pwm', ['src/softpwm.c', 'src/softpwm_py.c',  'src/h616_gpio.c']),
                   ],


)