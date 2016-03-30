from config import Configs # Contains all of the configs that are used in C++
from config import GraphicsConfig, BaseClass # Classes defined in C++

Configs.GraphicsConfig.setResolution(800, 600)

class derived2(BaseClass):
    def overrideMe(self):
        print("Asdf-asdf-asdf")

d = derived2();
#d.overrideMe()

# Default as defined in the C++ code
Configs.ClassConfig.overrideMe()

# Overwrite with our own class [derived2]
Configs.ClassConfig = d
Configs.ClassConfig.overrideMe() # This is the function as defined in derived2
