import tests.test as underscore_test
print(underscore_test.underscore(["hello","world"]))
import tests.tf_test as tfmod

# callme = lambda a: tfmod.predict(a)
m = tfmod.Model()
print(tfmod.predictOne([1,2,5,4,5]))
print(tfmod.predictMany([[1,2,5,4,5]]))