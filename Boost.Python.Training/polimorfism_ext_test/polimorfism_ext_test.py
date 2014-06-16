from polymorfism_ext import CppBase, CppDerived, print_hello, print_hello2

import pickle

#def CppDerived_getinitargs(self):
#    return (self.data_,)

#CppDerived.__getinitargs__ = CppDerived_getinitargs

cd = CppDerived('alin')
print cd
print cd.hello()

cd.data_ = "Alin Draghia"
cd_dump = pickle.dumps(cd)
cd_ = pickle.loads(cd_dump)
print_hello(cd_)
print_hello2(cd_)

class PyDerived(CppBase):
    def __init__(self, data):
        super(PyDerived, self).__init__()
        self.data = data
        self.x = 150
        return

    def hello(self):
        return 'Py: ' + self.data + ' x=' + str(self.x);

    #implement pickle support
    def __reduce__(self):
        initargs = (self.data,)
        state = {'x': self.x}
        return (PyDerived, initargs, state)

    def __setstate__(self, state):
        self.x = state['x']

pd = PyDerived('alin')
pd.x = 550
print pd
print pd.hello()


print_hello(cd)
print_hello(pd)



pd_dump = pickle.dumps(pd)
pd_ = pickle.loads(pd_dump)
print_hello(pd_)

print_hello2(cd)
print_hello2(pd)
print_hello2(pd_)



i=0