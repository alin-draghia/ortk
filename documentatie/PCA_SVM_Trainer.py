from object_recognition_toolkit import Trainer
from sklearn.decomposition import PCA

class PCA_SVM_Trainer(Trainer):
    def __init__(self):
        super(Trainer, self).__init__()
        self.trainer = LinearSVM_Trainer(C=1.0)
        self.pca = PCA()
        return
        
    def Clone(self):
        return pickle.loads(pickle.dumps(self))
    
    def Train(self, X, y):
        X_redus = self.pca.fit_transform(X)
        cls = self.trainer.Train(X_redus, y)
        return cls
    
    #suport pentru serializare
    def __reduce__(self):
        initargs = tuple()
        state = {'trainer': self.trainer, 
                 'pca': self.pca}
        return (Trainer, initargs, state)
    
    def __setstate__(self, state):
        self.trainer = state['trainer']
        self.pca = state['pca']
        return