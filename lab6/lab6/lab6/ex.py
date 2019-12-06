from random import randint

T = [1000]

def test(tid, n):
  with open("tests.in" , "w") as f:
    f.write("%d\n" % n)
    for i in range(n):
      f.write("%d " % randint(1, 100))
    f.write("\n")
    for i in range(n):
      f.write("%d " % randint(1, 100))

for i, t in enumerate(T):
  print("%d %d" % (i, t))
  test(i, t)
