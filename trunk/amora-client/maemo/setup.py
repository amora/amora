from distutils.core import setup

setup(name='amora',
       version='0.1',
       data_files = [
                    ('share/pixmaps',                       ['icons/26x26/amora.png']),
                    ('share/icons/hicolor/26x26/hildon',    ['icons/26x26/amora.png']),
                    ('share/icons/hicolor/40x40/hildon',    ['icons/40x40/amora.png']),
                    ('share/icons/hicolor/scalable/hildon', ['icons/scalable/amora.png']),
                    ('share/applications/hildon',           ['amora.desktop']),
                    ('share/amora',                         ['amora.edj']),
                    ('share/amora',                         ['controller.edj']),
					('share/amora',                         ['amora']),
					('share/amora',                         ['btio.py']),
					('share/amora',                         ['dbus_discover.py']),
					('share/amora',                         ['protocol.py']),
					('share/amora',                         ['controller.py']),
                    ]
      )
